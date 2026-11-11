#include<iostream>
#include<vector>
#include<random>
#include<assimp/scene.h>
#include<assimp/Importer.hpp>
#include<assimp/postprocess.h> 
#include"Mesh.h"

// 모델 로드 함수 생성자 혹은 메서드
bool Mesh::LoadModel(std::string_view path) {
	Assimp::Importer importer;

	// 파일을 읽으며 삼각형화, UV 뒤집기, 정점 최적화 수행
	const aiScene* scene = importer.ReadFile(path.data(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices
	);

	// 로드 실패 처리
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ASSIMP ERROR:: " << importer.GetErrorString() << std::endl;
		return false;
	}

	processNode(scene->mRootNode, scene);
	return true;
}

// 노드 계층 구조를 재귀적으로 순회하는 함수
void Mesh::processNode(aiNode* node, const aiScene* scene) {
	// 현재 노드에 포함된 모든 메시 처리
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	
	// 자식 노드가 있다면 재귀적으로 탐색
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

void Mesh::processMesh(aiMesh* mesh, const aiScene* scene) {

	// 정점 데이터 추출
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		if(mesh->HasNormals()) {
			vertices.push_back(mesh->mNormals[i].x);
			vertices.push_back(mesh->mNormals[i].y);
			vertices.push_back(mesh->mNormals[i].z);

		}
		
		// 텍스처 좌표 (Texture Coordinates / UV)
		if (mesh->mTextureCoords[0]) { // 0번 채널에 UV가 존재하는지 확인
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	// 인덱스(페이스) 데이터 추출
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
}