#pragma once
#include "glCheck.hpp"
#include "RenderManager.hpp"
#include "GLShader.hpp"
#include "GLVertexArray.hpp"
#include "GLUniformBuffer.hpp"
#include "GLImGuiManager.hpp"

class GLRenderManager : public RenderManager
{
public:
	GLRenderManager() = default;
	~GLRenderManager();
	void Initialize(
		SDL_Window* window_, SDL_GLContext context_
	);

	void BeginRender(glm::vec3 bgColor) override;
	void EndRender() override;
private:
	void GLDrawIndexed(const GLVertexArray& vertex_array) noexcept
	{
		glCheck(glDrawElements(GL_TRIANGLES, vertex_array.GetIndicesCount(), GL_UNSIGNED_INT, 0));
	}

	void GLDrawVertices(const GLVertexArray& vertex_array) noexcept
	{
		glCheck(glDrawArrays(GL_TRIANGLES, 0, vertex_array.GetVerticesCount()));
	}

	GLVertexArray vertexArray;
	GLShader gl2DShader;
	GLShader gl3DShader;
	GLImGuiManager* imguiManager;
public:
	//--------------------Common--------------------//
	void DeleteWithIndex(int id) override;

	//--------------------2D Render--------------------//
	void LoadQuad(glm::vec4 color_, float isTex_, float isTexel_) override;

	//--------------------3D Render--------------------//
	void LoadMesh(MeshType type, const std::filesystem::path& path, glm::vec4 color, int stacks, int slices, float metallic = 0.3f, float roughness = 0.3f) override;
private:
	//--------------------Common--------------------//
	GLVertexBuffer* vertexBuffer{ nullptr };
	GLIndexBuffer* indexBuffer{ nullptr };

	//--------------------2D Render--------------------//
	GLUniformBuffer<TwoDimension::VertexUniform>* vertexUniform2D{ nullptr };
	GLUniformBuffer<TwoDimension::FragmentUniform>* fragmentUniform2D{ nullptr };

	//--------------------3D Render--------------------//
	GLUniformBuffer<ThreeDimension::VertexUniform>* vertexUniform3D{ nullptr };
	GLUniformBuffer<ThreeDimension::FragmentUniform>* fragmentUniform3D{ nullptr };
	GLUniformBuffer<ThreeDimension::Material>* fragmentMaterialUniformBuffer{ nullptr };

	//Lighting
	GLUniformBuffer<ThreeDimension::DirectionalLightUniform>* directionalLightUniformBuffer{ nullptr };
	GLUniformBuffer<ThreeDimension::PointLightUniform>* pointLightUniformBuffer{ nullptr };
};
