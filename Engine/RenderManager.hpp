#pragma once
#define NOMINMAX

#include <filesystem>
#include "Material.hpp"
#include "Window.hpp"

constexpr float EPSILON = 0.00001f;
constexpr float PI = 3.14159f;
constexpr float HALF_PI = 0.5f * PI;
constexpr int   XINDEX = 0;
constexpr int   YINDEX = 1;
constexpr int   ZINDEX = 2;

enum class RenderType
{
	TwoDimension,
	ThreeDimension,
};

enum class PolygonType
{
	FILL,
	LINE,
};

enum class MeshType
{
	PLANE,
	CUBE,
	SPHERE,
	TORUS,
	CYLINDER,
	CONE,
	OBJ,
};

class RenderManager
{
public:
	//--------------------Common--------------------//
	virtual void BeginRender(glm::vec3 bgColor) = 0;
	virtual void EndRender() = 0;
	virtual void DeleteWithIndex(int id) = 0;
	void SetRenderType(RenderType type) { rMode = type; };
	void SetPolygonType(PolygonType type) { pMode = type; };
	RenderType GetRenderType() { return rMode; };

	//--------------------2D Render--------------------//
	virtual void LoadQuad(glm::vec4 color_, float isTex_, float isTexel_) = 0;

	std::vector<TwoDimension::VertexUniform>* GetVertexUniforms2D() { return &vertexUniforms2D; };
	std::vector<TwoDimension::FragmentUniform>* GetFragmentUniforms2D() { return &fragUniforms2D; };

	//--------------------3D Render--------------------//
	virtual void LoadMesh(MeshType type, const std::filesystem::path& path, glm::vec4 color, int stacks, int slices, float metallic = 0.3f, float roughness = 0.3f) = 0;
	void AddDirectionalLight(const ThreeDimension::DirectionalLightUniform& light)
	{
		directionalLightUniforms.push_back(light);
	}
	void DeleteDirectionalLights()
	{
		directionalLightUniforms.clear();
		directionalLightUniforms.shrink_to_fit();
	}
	void AddPointLight(const ThreeDimension::PointLightUniform& light)
	{
		pointLightUniforms.push_back(light);
	}
	void DeletePointLights()
	{
		pointLightUniforms.clear();
		pointLightUniforms.shrink_to_fit();
	}
	std::vector<ThreeDimension::DirectionalLightUniform>& GetDirectionalLightUniforms() { return directionalLightUniforms; };
	std::vector<ThreeDimension::PointLightUniform>& GetPointLightUniforms() { return pointLightUniforms; };

	std::vector<ThreeDimension::VertexUniform>* GetVertexUniforms3D() { return &vertexUniforms3D; };
	std::vector<ThreeDimension::FragmentUniform>* GetFragmentUniforms3D() { return &fragUniforms3D; };
protected:
	//--------------------Common--------------------//
	RenderType rMode = RenderType::TwoDimension;
	PolygonType pMode = PolygonType::FILL;
	unsigned int quadCount{ 0 };
	std::vector<uint32_t> indices;

	//--------------------2D Render--------------------//
	std::vector<TwoDimension::Vertex> vertices2D;
	std::vector<TwoDimension::VertexUniform> vertexUniforms2D;
	std::vector<TwoDimension::FragmentUniform> fragUniforms2D;

	//--------------------3D Render--------------------//
	static bool DegenerateTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		return (glm::distance(v0, v1) < EPSILON || glm::distance(v1, v2) < EPSILON || glm::distance(v2, v0) < EPSILON);
	}
	static float RoundDecimal(float input) { return std::floor(input * 10000.0f + 0.5f) / 10000.0f; }
	static glm::vec3 RoundDecimal(const glm::vec3& input)
	{
		return { RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]) };
	}

	void CreateMesh(MeshType type, const std::filesystem::path& path, int stacks, int slices);

	//Lighting
	std::vector<ThreeDimension::DirectionalLightUniform> directionalLightUniforms;
	std::vector<ThreeDimension::PointLightUniform> pointLightUniforms;

	std::vector<ThreeDimension::Vertex> vertices3D;
	std::vector<ThreeDimension::VertexUniform> vertexUniforms3D;
	std::vector<ThreeDimension::FragmentUniform> fragUniforms3D;
	std::vector<ThreeDimension::Material> fragMaterialUniforms3D;

	std::vector<unsigned int> verticesPerMesh;
	std::vector<unsigned int> indicesPerMesh;
private:
	static void BuildIndices(const std::vector<ThreeDimension::Vertex>& tempVertices, std::vector<uint32_t>& tempIndices, const unsigned int verticesCount, const int stacks, const int slices);
};
