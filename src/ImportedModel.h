#include <vector>

class ImportedModel
{
private:
	int numVertices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normalVecs;
	bool hasTexture;
	void init(const char* filePath, const bool hasTexture);
public:
	ImportedModel();
	ImportedModel(const char* filePath);
	ImportedModel(const char* filePath, const bool hasTexture);
	int getNumVertices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTextureCoords();
	std::vector<glm::vec3> getNormals();
	const bool isTextured();
};

class ModelImporter
{
private:
	std::vector<float> vertVals;
	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> stVals;
	std::vector<float> normals;
	std::vector<float> normVals;
public:
	ModelImporter();
	void parseOBJ(const char* filePath, const bool HasTexture);
	int getNumVertices();
	std::vector<float> getVertices();
	std::vector<float> getTextureCoordinates();
	std::vector<float> getNormals();
	
};