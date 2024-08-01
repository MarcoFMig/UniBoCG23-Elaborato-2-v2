#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "Strutture.h"

/**
 * Attempts to load a mesh given a path.
 * @param path		the path to the mesh
 * @param mymesh	the empty mesh object structure
 * @returns			true if the operation was done without errors
 */
bool loadAssImp(const char* path, vector<MeshObj>& mymesh) {

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh* mesh;

	int num_meshes = scene->mNumMeshes;
	mymesh.resize(num_meshes);
	for (unsigned int nm = 0; nm < num_meshes; nm++) {
		mesh = scene->mMeshes[nm];
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D color;
		float value;
		// Read mtl file vertex data

		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
			mymesh[nm].materiale.ambient = glm::vec3(color.r, color.g, color.b);
		}	else {
			printf("Unable to import ambient\n");
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
			mymesh[nm].materiale.diffuse = glm::vec3(color.r, color.g, color.b);
		}	else {
			mymesh[nm].materiale.diffuse = glm::vec3(1.0, 0.2, 0.1);
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
			mymesh[nm].materiale.specular = glm::vec3(color.r, color.g, color.b);
		}	else {
			printf("Unable to import specular\n");
			mymesh[nm].materiale.specular = glm::vec3(0.5, 0.5, 0.5);
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, value)) {
			mymesh[nm].materiale.shininess = value;
		}	else {
			printf("Unable to import shinyness\n");
			mymesh[nm].materiale.shininess = 50.0f;
		}

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D pos = mesh->mVertices[i];
			mymesh[nm].vertici.push_back(glm::vec3(pos.x, pos.y, pos.z));
		}
		// Fill vertices texture coordinates
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			mymesh[nm].texCoords.push_back(glm::vec2(0.0, 0.0));
			mymesh[nm].colori.push_back(vec4(1.0, 0.0, 0.0, 1.0));
		}
		// Fill vertices normals
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D n = mesh->mNormals[i];
			mymesh[nm].normali.push_back(glm::vec3(n.x, n.y, n.z));
		}
		// Fill face indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			// Assume the model has only triangles.
			mymesh[nm].indici.push_back(mesh->mFaces[i].mIndices[0]);
			mymesh[nm].indici.push_back(mesh->mFaces[i].mIndices[1]);
			mymesh[nm].indici.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}
	return true;
}
