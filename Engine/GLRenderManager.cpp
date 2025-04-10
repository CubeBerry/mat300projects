#include "Engine.hpp"
#include <span>

GLRenderManager::~GLRenderManager()
{
	//Delete ImGui
	delete imguiManager;

	//Destroy Buffers
	delete vertexBuffer;
	delete indexBuffer;
	delete vertexUniform2D;
	delete fragmentUniform2D;
	delete vertexUniform3D;
	delete fragmentUniform3D;
	delete fragmentMaterialUniformBuffer;
	delete pointLightUniformBuffer;
}

void GLRenderManager::Initialize(
	SDL_Window* window_, SDL_GLContext context_
)
{
	vertexArray.Initialize();

	gl2DShader.LoadShader({ { GLShader::VERTEX, "./shaders/2D.vert" }, { GLShader::FRAGMENT, "./shaders/2D.frag" } });
	gl3DShader.LoadShader({ { GLShader::VERTEX, "./shaders/3D.vert" }, { GLShader::FRAGMENT, "./shaders/3D.frag" } });

	vertexUniform2D = new GLUniformBuffer<TwoDimension::VertexUniform>();
	vertexUniform2D->InitUniform(gl2DShader.GetProgramHandle(), 0, "vUniformMatrix", 0, nullptr);
	fragmentUniform2D = new GLUniformBuffer<TwoDimension::FragmentUniform>();
	fragmentUniform2D->InitUniform(gl2DShader.GetProgramHandle(), 1, "fUniformMatrix", 0, nullptr);

	vertexUniform3D = new GLUniformBuffer<ThreeDimension::VertexUniform>();
	vertexUniform3D->InitUniform(gl3DShader.GetProgramHandle(), 2, "vUniformMatrix", 0, nullptr);
	fragmentUniform3D = new GLUniformBuffer<ThreeDimension::FragmentUniform>();
	fragmentUniform3D->InitUniform(gl3DShader.GetProgramHandle(), 3, "fUniformMatrix", 0, nullptr);
	fragmentMaterialUniformBuffer = new GLUniformBuffer<ThreeDimension::Material>();
	fragmentMaterialUniformBuffer->InitUniform(gl3DShader.GetProgramHandle(), 4, "fUniformMaterial", 0, nullptr);

	//Lighting
	directionalLightUniformBuffer = new GLUniformBuffer<ThreeDimension::DirectionalLightUniform>();
	directionalLightUniformBuffer->InitUniform(gl3DShader.GetProgramHandle(), 5, "fDirectionalLightList", 0, nullptr);
	pointLightUniformBuffer = new GLUniformBuffer<ThreeDimension::PointLightUniform>();
	pointLightUniformBuffer->InitUniform(gl3DShader.GetProgramHandle(), 6, "fPointLightList", 0, nullptr);

	imguiManager = new GLImGuiManager(window_, context_);
}

void GLRenderManager::BeginRender(glm::vec3 bgColor)
{
	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glDepthFunc(GL_LEQUAL));
	switch (pMode)
	{
	case PolygonType::FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case PolygonType::LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	glCheck(glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.f));
	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	switch (rMode)
	{
	case RenderType::TwoDimension:
		glCheck(glDisable(GL_CULL_FACE));

		gl2DShader.Use(true);

		if (vertexUniform2D != nullptr)
		{
			vertexUniform2D->UpdateUniform(vertexUniforms2D.size() * sizeof(TwoDimension::VertexUniform), vertexUniforms2D.data());
		}
		if (fragmentUniform2D != nullptr)
		{
			fragmentUniform2D->UpdateUniform(fragUniforms2D.size() * sizeof(TwoDimension::FragmentUniform), fragUniforms2D.data());
		}
		break;
	case RenderType::ThreeDimension:
		glCheck(glEnable(GL_CULL_FACE));
		glCheck(glCullFace(GL_BACK));

		gl3DShader.Use(true);

		if (vertexUniform3D != nullptr)
		{
			vertexUniform3D->UpdateUniform(vertexUniforms3D.size() * sizeof(ThreeDimension::VertexUniform), vertexUniforms3D.data());
		}
		if (fragmentUniform3D != nullptr)
		{
			fragmentUniform3D->UpdateUniform(fragUniforms3D.size() * sizeof(ThreeDimension::FragmentUniform), fragUniforms3D.data());
			fragmentMaterialUniformBuffer->UpdateUniform(fragMaterialUniforms3D.size() * sizeof(ThreeDimension::Material), fragMaterialUniforms3D.data());
		}
		if (pointLightUniformBuffer != nullptr)
		{
			glCheck(glUniform1i(glGetUniformLocation(gl3DShader.GetProgramHandle(), "activePointLights"), static_cast<GLint>(pointLightUniforms.size())));
			pointLightUniformBuffer->UpdateUniform(pointLightUniforms.size() * sizeof(ThreeDimension::PointLightUniform), pointLightUniforms.data());
		}
		if (directionalLightUniformBuffer != nullptr)
		{
			glCheck(glUniform1i(glGetUniformLocation(gl3DShader.GetProgramHandle(), "activeDirectionalLights"), static_cast<GLint>(directionalLightUniforms.size())));
			directionalLightUniformBuffer->UpdateUniform(directionalLightUniforms.size() * sizeof(ThreeDimension::DirectionalLightUniform), directionalLightUniforms.data());
		}
		break;
	}

	vertexArray.Use(true);
	GLDrawIndexed(vertexArray);
	vertexArray.Use(false);

	switch (rMode)
	{
	case RenderType::TwoDimension:
		gl2DShader.Use(false);
		break;
	case RenderType::ThreeDimension:
		gl3DShader.Use(false);
		break;
	}

	imguiManager->Begin();
}

void GLRenderManager::EndRender()
{
	imguiManager->End();

	SDL_GL_SwapWindow(Engine::Instance().GetWindow().GetWindow());
}

void GLRenderManager::LoadQuad(glm::vec4 color_, float isTex_, float isTexel_)
{
	vertices2D.push_back(TwoDimension::Vertex(glm::vec3(-1.f, 1.f, 1.f), quadCount));
	vertices2D.push_back(TwoDimension::Vertex(glm::vec3(1.f, 1.f, 1.f), quadCount));
	vertices2D.push_back(TwoDimension::Vertex(glm::vec3(1.f, -1.f, 1.f), quadCount));
	vertices2D.push_back(TwoDimension::Vertex(glm::vec3(-1.f, -1.f, 1.f), quadCount));

	if (quadCount > 0)
		delete vertexBuffer;
	vertexBuffer = new GLVertexBuffer;
	vertexBuffer->SetData(static_cast<GLsizei>(sizeof(TwoDimension::Vertex) * vertices2D.size()), vertices2D.data());

	uint64_t indexNumber{ vertices2D.size() / 4 - 1 };
	indices.push_back(static_cast<uint32_t>(4 * indexNumber));
	indices.push_back(static_cast<uint32_t>(4 * indexNumber + 1));
	indices.push_back(static_cast<uint32_t>(4 * indexNumber + 2));
	indices.push_back(static_cast<uint32_t>(4 * indexNumber + 2));
	indices.push_back(static_cast<uint32_t>(4 * indexNumber + 3));
	indices.push_back(static_cast<uint32_t>(4 * indexNumber));
	if (indexBuffer != nullptr)
		delete indexBuffer;
	indexBuffer = new GLIndexBuffer(&indices);

	quadCount++;

	//Attributes
	GLAttributeLayout position_layout;
	position_layout.component_type = GLAttributeLayout::Float;
	position_layout.component_dimension = GLAttributeLayout::_3;
	position_layout.normalized = false;
	position_layout.vertex_layout_location = 0;
	position_layout.stride = sizeof(TwoDimension::Vertex);
	position_layout.offset = 0;
	position_layout.relative_offset = offsetof(TwoDimension::Vertex, position);

	GLAttributeLayout index_layout;
	index_layout.component_type = GLAttributeLayout::Int;
	index_layout.component_dimension = GLAttributeLayout::_1;
	index_layout.normalized = false;
	index_layout.vertex_layout_location = 1;
	index_layout.stride = sizeof(TwoDimension::Vertex);
	index_layout.offset = 0;
	index_layout.relative_offset = offsetof(TwoDimension::Vertex, index);

	vertexArray.AddVertexBuffer(std::move(*vertexBuffer), sizeof(TwoDimension::Vertex), {position_layout, index_layout});
	vertexArray.SetIndexBuffer(std::move(*indexBuffer));

	TwoDimension::VertexUniform mat;
	mat.model = glm::mat4(1.f);
	mat.view = glm::mat4(1.f);
	mat.projection = glm::mat4(1.f);
	vertexUniforms2D.push_back(mat);
	vertexUniforms2D.back().color = color_;
	vertexUniforms2D.back().isTex = isTex_;
	vertexUniforms2D.back().isTexel = isTexel_;

	TwoDimension::FragmentUniform tIndex;
	tIndex.texIndex = 0;
	fragUniforms2D.push_back(tIndex);

	//if (vertexUniform2D != nullptr)
	//	delete vertexUniform2D;
	//vertexUniform2D = new GLUniformBuffer<TwoDimension::VertexUniform>();
	//vertexUniform2D->InitUniform(gl2DShader.GetProgramHandle(), 0, "vUniformMatrix", vertexUniforms2D.size() * sizeof(TwoDimension::VertexUniform), vertexUniforms2D.data());

	//if (fragmentUniform2D != nullptr)
	//	delete fragmentUniform2D;
	//fragmentUniform2D = new GLUniformBuffer<TwoDimension::FragmentUniform>();
	//fragmentUniform2D->InitUniform(gl2DShader.GetProgramHandle(), 1, "fUniformMatrix", fragUniforms2D.size() * sizeof(TwoDimension::FragmentUniform), fragUniforms2D.data());
}

void GLRenderManager::DeleteWithIndex(int id)
{
	quadCount--;

	if (quadCount == 0)
	{
		switch (rMode)
		{
		case RenderType::TwoDimension:
			vertices2D.erase(end(vertices2D) - 4, end(vertices2D));
			break;
		case RenderType::ThreeDimension:
			vertices3D.erase(end(vertices3D) - *verticesPerMesh.begin(), end(vertices3D));
			break;
		}
		delete vertexBuffer;
		vertexBuffer = nullptr;

		switch (rMode)
		{
		case RenderType::TwoDimension:
			indices.erase(end(indices) - 6, end(indices));
			break;
		case RenderType::ThreeDimension:
			indices.erase(end(indices) - *indicesPerMesh.begin(), end(indices));
			break;
		}
		delete indexBuffer;
		indexBuffer = nullptr;

		if (rMode == RenderType::ThreeDimension)
		{
			verticesPerMesh.erase(verticesPerMesh.begin());
			indicesPerMesh.erase(indicesPerMesh.begin());
		}

		switch (rMode)
		{
		case RenderType::TwoDimension:
			vertexUniforms2D.erase(end(vertexUniforms2D) - 1);
			//delete vertexUniform2D;
			//vertexUniform2D = nullptr;
			break;
		case RenderType::ThreeDimension:
			vertexUniforms3D.erase(end(vertexUniforms3D) - 1);
			//delete vertexUniform3D;
			//vertexUniform3D = nullptr;
			break;
		}

		switch (rMode)
		{
		case RenderType::TwoDimension:
			fragUniforms2D.erase(end(fragUniforms2D) - 1);
			//delete fragmentUniform2D;
			//fragmentUniform2D = nullptr;
			break;
		case RenderType::ThreeDimension:
			fragUniforms3D.erase(end(fragUniforms3D) - 1);
			//delete fragmentUniform3D;
			//fragmentUniform3D = nullptr;

			fragMaterialUniforms3D.erase(end(fragMaterialUniforms3D) - 1);
			//delete fragmentMaterialUniformBuffer;
			//fragmentMaterialUniformBuffer = nullptr;
			break;
		}

		return;
	}

	switch (rMode)
	{
	case RenderType::TwoDimension:
		vertices2D.erase(end(vertices2D) - 4, end(vertices2D));
		indices.erase(end(indices) - 6, end(indices));
		//glCheck(glNamedBufferSubData(vertexBuffer->GetHandle(), 0, static_cast<GLsizei>(sizeof(TwoDimension::Vertex) * vertices2D.size()), vertices2D.data()));
		//glCheck(glNamedBufferSubData(indexBuffer->GetHandle(), 0, sizeof(uint32_t) * indices.size(), indices.data()));
		break;
	case RenderType::ThreeDimension:
		unsigned int beginCount{ 0 };
		for (int v = 0; v < id; ++v)
		{
			beginCount += verticesPerMesh[v];
		}
		vertices3D.erase(begin(vertices3D) + beginCount, begin(vertices3D) + beginCount + verticesPerMesh[id]);
		for (auto it = vertices3D.begin() + beginCount; it != vertices3D.end(); ++it)
		{
			it->index--;
		}

		beginCount = 0;
		for (int v = 0; v < id; ++v)
		{
			beginCount += indicesPerMesh[v];
		}
		indices.erase(begin(indices) + beginCount, begin(indices) + beginCount + indicesPerMesh[id]);
		for (auto it = indices.begin() + beginCount; it != indices.end(); ++it)
		{
			(*it) = (*it) - static_cast<unsigned short>(verticesPerMesh[id]);
		}

		glCheck(glNamedBufferSubData(vertexBuffer->GetHandle(), 0, static_cast<GLsizei>(sizeof(ThreeDimension::Vertex) * vertices3D.size()), vertices3D.data()));
		glCheck(glNamedBufferSubData(indexBuffer->GetHandle(), 0, sizeof(uint32_t) * indices.size(), indices.data()));
		indexBuffer->SetCount(static_cast<int>(indices.size()));
		vertexArray.SetIndexBuffer(std::move(*indexBuffer));
		break;
	}

	if (rMode == RenderType::ThreeDimension)
	{
		verticesPerMesh.erase(verticesPerMesh.begin() + id);
		indicesPerMesh.erase(indicesPerMesh.begin() + id);
	}

	switch (rMode)
	{
	case RenderType::TwoDimension:
		vertexUniforms2D.erase(end(vertexUniforms2D) - 1);
		break;
	case RenderType::ThreeDimension:
		vertexUniforms3D.erase(end(vertexUniforms3D) - 1);
		break;
	}

	switch (rMode)
	{
	case RenderType::TwoDimension:
		fragUniforms2D.erase(end(fragUniforms2D) - 1);
		break;
	case RenderType::ThreeDimension:
		fragUniforms3D.erase(end(fragUniforms3D) - 1);
		fragMaterialUniforms3D.erase(end(fragMaterialUniforms3D) - 1);
		break;
	}
}

void GLRenderManager::LoadMesh(MeshType type, const std::filesystem::path& path, glm::vec4 color, int stacks, int slices, float metallic, float roughness)
{
	CreateMesh(type, path, stacks, slices);

	if (quadCount > 0)
		delete vertexBuffer;
	vertexBuffer = new GLVertexBuffer;
	vertexBuffer->SetData(static_cast<GLsizei>(sizeof(ThreeDimension::Vertex) * vertices3D.size()), vertices3D.data());

	if (indexBuffer != nullptr)
		delete indexBuffer;
	indexBuffer = new GLIndexBuffer(&indices);

	quadCount++;

	//Attributes
	GLAttributeLayout position_layout;
	position_layout.component_type = GLAttributeLayout::Float;
	position_layout.component_dimension = GLAttributeLayout::_3;
	position_layout.normalized = false;
	position_layout.vertex_layout_location = 0;
	position_layout.stride = sizeof(ThreeDimension::Vertex);
	position_layout.offset = 0;
	position_layout.relative_offset = offsetof(ThreeDimension::Vertex, position);

	GLAttributeLayout normal_layout;
	normal_layout.component_type = GLAttributeLayout::Float;
	normal_layout.component_dimension = GLAttributeLayout::_3;
	normal_layout.normalized = false;
	normal_layout.vertex_layout_location = 1;
	normal_layout.stride = sizeof(ThreeDimension::Vertex);
	normal_layout.offset = 0;
	normal_layout.relative_offset = offsetof(ThreeDimension::Vertex, normal);

	GLAttributeLayout uv_layout;
	uv_layout.component_type = GLAttributeLayout::Float;
	uv_layout.component_dimension = GLAttributeLayout::_2;
	uv_layout.normalized = false;
	uv_layout.vertex_layout_location = 2;
	uv_layout.stride = sizeof(ThreeDimension::Vertex);
	uv_layout.offset = 0;
	uv_layout.relative_offset = offsetof(ThreeDimension::Vertex, uv);

	GLAttributeLayout index_layout;
	index_layout.component_type = GLAttributeLayout::Int;
	index_layout.component_dimension = GLAttributeLayout::_1;
	index_layout.normalized = false;
	index_layout.vertex_layout_location = 3;
	index_layout.stride = sizeof(ThreeDimension::Vertex);
	index_layout.offset = 0;
	index_layout.relative_offset = offsetof(ThreeDimension::Vertex, index);

	vertexArray.AddVertexBuffer(std::move(*vertexBuffer), sizeof(ThreeDimension::Vertex), { position_layout, normal_layout, uv_layout, index_layout });
	vertexArray.SetIndexBuffer(std::move(*indexBuffer));

	ThreeDimension::VertexUniform mat;
	mat.model = glm::mat4(1.f);
	mat.view = glm::mat4(1.f);
	mat.projection = glm::mat4(1.f);
	vertexUniforms3D.push_back(mat);
	vertexUniforms3D.back().color = color;

	ThreeDimension::FragmentUniform tIndex;
	tIndex.texIndex = 0;
	fragUniforms3D.push_back(tIndex);

	ThreeDimension::Material material;
	material.metallic = metallic;
	material.roughness = roughness;
	fragMaterialUniforms3D.push_back(material);

	//if (vertexUniform3D != nullptr)
	//	delete vertexUniform3D;
	//vertexUniform3D = new GLUniformBuffer<ThreeDimension::VertexUniform>();
	//vertexUniform3D->InitUniform(gl3DShader.GetProgramHandle(), 0, "vUniformMatrix", vertexUniforms3D.size(), vertexUniforms3D.data());

	//if (fragmentUniform3D != nullptr)
	//	delete fragmentUniform3D;
	//fragmentUniform3D = new GLUniformBuffer<ThreeDimension::FragmentUniform>();
	//fragmentUniform3D->InitUniform(gl3DShader.GetProgramHandle(), 1, "fUniformMatrix", fragUniforms3D.size(), fragUniforms3D.data());

	//if (fragmentMaterialUniformBuffer != nullptr)
	//	delete fragmentMaterialUniformBuffer;
	//fragmentMaterialUniformBuffer = new GLUniformBuffer<ThreeDimension::Material>();
	//fragmentMaterialUniformBuffer->InitUniform(gl3DShader.GetProgramHandle(), 2, "fUniformMaterial", fragMaterialUniforms3D.size(), fragMaterialUniforms3D.data());
}
