#include "svpch.h"
#include "strvy/Renderer/Model.h"


namespace strvy {



	void Model::draw(Ref<Shader> shader)
	{
		for (uint32_t i = 0; i < m_meshes.size(); ++i)
			m_meshes[i]->draw(shader);
	}

	void Model::loadModel(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		m_directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> textures;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 UV;
				UV.x = mesh->mTextureCoords[0][i].x;
				UV.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = UV;
			}
			else
				vertex.texCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		// process indices

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Ref<Texture2D>> diffuseMaps = loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Ref<Texture2D>> specularMaps = loadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh::create(vertices, indices, textures);
	}

	std::vector<Ref<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Ref<Texture2D>> textures;
		for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (uint32_t j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j]->getPath().data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Ref<Texture2D> texture = Texture2D::create("assets/objects/backpack/" + (std::string)str.C_Str(), typeName);
				textures.push_back(texture);
			}
		}
		return textures;
	}

}
