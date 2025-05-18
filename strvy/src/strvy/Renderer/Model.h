#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



namespace strvy {

	class Model
	{
	public:
		Model(char* path, bool gamma = false)
			: m_gammaCorrection(gamma)
		{
			loadModel(path);
		}
		void draw(Shader& shader);
	private:
		// model data
		std::vector<Ref<Mesh>> m_meshes;
		std::vector<Ref<Texture>> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		std::string m_directory;

		bool m_gammaCorrection;


		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

}