#include "Model.h"



Model::Model(const char* FilePath, unsigned int modeltype, bool isAnimated, glm::vec3 InitialLocation, glm::vec3 InitialScale)
{
	if (modeltype == 0) //OBJ load
	{
		std::string ObjPath = FilePath;
		ObjPath.append(".obj");

		bool successful = tinyobj::LoadObj(shapes, materials, err, ObjPath.c_str());
		if (!successful)
		{
			std::cout << "Error Loading Model" << FilePath << std::endl;
		}
		assert(successful);

		std::ifstream texturestream;

		//load material ID's
		std::string MtlPath = FilePath;
		MtlPath.append(".mtl");
		texturestream.open(MtlPath, std::ios::in);
		tinyobj::LoadMtl(m_TextureMap, materials, texturestream);

		for each (tinyobj::material_t mat in materials)
		{
			auto itter = m_TextureMap.find(mat.name);
			if (itter != m_TextureMap.end())
			{
				Texture* tempTexture = new Texture(mat.diffuse_texname.c_str());
				m_Textures.push_back(tempTexture->m_texture);
			}
		}

		createOpenGLBuffers(shapes);
	}
	else if (modeltype == 1) //FBX Load
	{
		m_FBXModel = new FBXFile();
		m_FBXModel->load(FilePath);
		CreateFBX(m_FBXModel);

		for (unsigned int i = 0; i < m_FBXModel->getMeshCount(); i++)
		{
			if (!isAnimated)
			{
				Shader* TempShader = new Shader("./data/Shaders/vs_texture.vert", "./data/Shaders/fs_texture_norm_spec.frag");
				TempShader->m_Textures.clear();
				TempShader->m_specpow = 1.5f;

				for (unsigned int i = 0; i < m_FBXModel->getTextureCount(); i++)
				{
					Texture* TempText = new Texture(m_FBXModel->getTextureByIndex(i)->path.c_str());
					TempShader->m_Textures.push_back(TempText);
				}

				ModelShaders.push_back(TempShader);
			}
			else
			{
				Shader* TempShader = new Shader("./data/Shaders/vs_animation.vert", "./data/Shaders/fs_texture_norm_spec.frag");
				TempShader->m_Textures.clear();
				TempShader->m_specpow = 1.5f;

				for (unsigned int i = 0; i < m_FBXModel->getTextureCount(); i++)
				{
					Texture* TempText = new Texture(m_FBXModel->getTextureByIndex(i)->path.c_str());
					TempShader->m_Textures.push_back(TempText);
				}

				ModelShaders.push_back(TempShader);
			}
		}
	}

	m_Location = InitialLocation;
	m_Scale = InitialScale;
}

void Model::CreateFBX(FBXFile* fbx)
{
	m_gl_info.resize(fbx->getMeshCount());

	//create GL VAO/VBO/IBO buffers
	for (unsigned int meshIndex = 0; meshIndex < fbx->getMeshCount(); meshIndex++)
	{
		glGenVertexArrays(1, &m_gl_info[meshIndex].m_VAO);
		glGenBuffers(1, &m_gl_info[meshIndex].m_VBO);
		glGenBuffers(1, &m_gl_info[meshIndex].m_IBO);

		//bind vertex array
		glBindVertexArray(m_gl_info[meshIndex].m_VAO);

		FBXMeshNode* mesh = fbx->getMeshByIndex(meshIndex);
		auto vertexData = mesh->m_vertices;

		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[meshIndex].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(FBXVertex), vertexData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[meshIndex].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);


		m_gl_info[meshIndex].m_index_count = mesh->m_indices.size();

		glEnableVertexAttribArray(0); //position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1); //normal
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (char*)0 + FBXVertex::NormalOffset);

		glEnableVertexAttribArray(2); //Tangent
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)0 + FBXVertex::TangentOffset);

		glEnableVertexAttribArray(3); //BiTangent
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (char*)0 + FBXVertex::BiNormalOffset);

		glEnableVertexAttribArray(4); //TexCoords
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)0 + FBXVertex::TexCoord1Offset);

		//Skinning
		glEnableVertexAttribArray(5); //bone weight
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)0 + FBXVertex::WeightsOffset);

		glEnableVertexAttribArray(6); //bone index
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char*)0 + FBXVertex::IndicesOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Model::CleanUpFBX(FBXFile* fbx)
{
	//clean up the vertex data of each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}
}

void Model::createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{
	m_gl_info.resize(shapes.size());

	//ModelShaders.resize(shapes.size());

	for (unsigned int mesh_index = 0; mesh_index < shapes.size(); ++mesh_index)
	{
		glGenVertexArrays(1, &m_gl_info[mesh_index].m_VAO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_VBO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_IBO);
		glBindVertexArray(m_gl_info[mesh_index].m_VAO);

		unsigned int float_count = shapes[mesh_index].mesh.positions.size();
		float_count += shapes[mesh_index].mesh.normals.size();
		float_count += shapes[mesh_index].mesh.texcoords.size();


		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.positions.begin(), shapes[mesh_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.normals.begin(), shapes[mesh_index].mesh.normals.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.texcoords.begin(), shapes[mesh_index].mesh.texcoords.end());

		m_gl_info[mesh_index].m_index_count = shapes[mesh_index].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int), shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);


		glEnableVertexAttribArray(0); //position
		unsigned int offset = 0;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

		glEnableVertexAttribArray(2); //normal
		offset += (sizeof(float) * shapes[mesh_index].mesh.positions.size());
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)offset);

		glEnableVertexAttribArray(3); //Texture Coords
		offset += (sizeof(float) * shapes[mesh_index].mesh.normals.size());
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);


		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}

void Model::Update(float DeltaTime)
{
	//grab the skeleton and animation
	FBXSkeleton* skeleton = m_FBXModel->getSkeletonByIndex(0);
	FBXAnimation* animation = m_FBXModel->getAnimationByIndex(0);

	//evaluate the animation to update bones
	skeleton->evaluate(animation, glfwGetTime());

	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; bone_index++)
	{
		skeleton->m_nodes[bone_index]->updateGlobalTransform();
	}

	//update uniform within shader
	for (int i = 0; i < ModelShaders.size(); i++)
	{
		ModelShaders[i]->UpdateBones(skeleton);
	}
}

void Model::Draw(Camera* camera)
{
	//if (ModelShader != nullptr)
	//{
	//	ModelShader->DrawShader(camera, m_Location, m_Scale, m_RotAxis, m_RotAmount);
	//}

	//if (m_FBXModel != nullptr) //Draw FBX
	//{
	//	ModelShaders[0]->DrawShader(camera, m_Location, m_Scale, m_RotAxis, m_RotAmount);
	//
	//	for (unsigned int i = 0; i < m_FBXModel->getMeshCount(); i++)
	//	{
	//		FBXMeshNode* mesh = m_FBXModel->getMeshByIndex(i);
	//
	//		unsigned int* glData = (unsigned int*)mesh->m_userData;
	//
	//
	//		glBindVertexArray(glData[0]);
	//		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	//	}
	//}
	//else //Draw OBJ
	//{
		for (unsigned int i = 0; i < m_gl_info.size(); ++i)
		{
			ModelShaders[0]->DrawShader(camera, m_Location, m_Scale, m_RotAxis, m_RotAmount);

			glBindVertexArray(m_gl_info[i].m_VAO);
			glDrawElements(GL_TRIANGLES, m_gl_info[i].m_index_count, GL_UNSIGNED_INT, 0);
		}
	//}
}


Model::~Model()
{
	delete m_FBXModel;
	CleanUpFBX(m_FBXModel);
}
