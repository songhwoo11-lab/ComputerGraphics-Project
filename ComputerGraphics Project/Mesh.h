#pragma once

struct Mesh {
public:
	bool LoadModel(std::string_view path);

private:
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);

public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};


