#include "Model.h"



Model::Model(const char* FilePath, glm::vec3 InitialLocation, glm::vec3 InitialScale)
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

	m_Location = InitialLocation;
	m_Scale = InitialScale;
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
		float_count += shapes[mesh_index].mesh.texcoords.size();
		float_count += shapes[mesh_index].mesh.normals.size();


		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.positions.begin(), shapes[mesh_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.texcoords.begin(), shapes[mesh_index].mesh.texcoords.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.normals.begin(), shapes[mesh_index].mesh.normals.end());

		m_gl_info[mesh_index].m_index_count = shapes[mesh_index].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int), shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);


		glEnableVertexAttribArray(0); //position
		unsigned int offset = 0;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

		glEnableVertexAttribArray(1); //Texture Coords
		offset += (sizeof(float) * shapes[mesh_index].mesh.positions.size());
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);

		glEnableVertexAttribArray(2); //normal
		offset += (sizeof(float) * shapes[mesh_index].mesh.texcoords.size());
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)offset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}

void Model::Update(float DeltaTime)
{

}

void Model::Draw(Camera* camera)
{
	//if (ModelShader != nullptr)
	//{
	//	ModelShader->DrawShader(camera, m_Location, m_Scale, m_RotAxis, m_RotAmount);
	//}

	for (unsigned int i = 0; i < m_gl_info.size(); ++i)
	{
		ModelShaders[i]->DrawShader(camera, m_Location, m_Scale, m_RotAxis, m_RotAmount);

		glBindVertexArray(m_gl_info[i].m_VAO);
		glDrawElements(GL_TRIANGLES, m_gl_info[i].m_index_count, GL_UNSIGNED_INT, 0);
	}
}


Model::~Model()
{
}
