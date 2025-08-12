#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



namespace strvy {

	class Model
	{
	public:
	public:
		Model() = default;
		Model(const std::string& path, bool gamma = false)
			: m_gammaCorrection(gamma)
		{
			loadModel(path);
		}
		void draw(Ref<Shader> shader);
		void loadModel(const std::string& path);
	private:
		// model data
		std::vector<Ref<Mesh>> m_meshes;
		std::vector<Ref<Texture2D>> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		std::string m_directory;

		bool m_gammaCorrection;


		void processNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

}