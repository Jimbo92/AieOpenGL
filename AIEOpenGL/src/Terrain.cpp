#include "Terrain.h"



Terrain::Terrain(Camera* camera)
{
	m_CurrentCamera = camera;
}

void Terrain::generateGrid()
{
	m_TerrainShader = new Shader("./data/Shaders/vs_rendertarget.vert", "./data/Shaders/fs_rendertarget.frag");

	float vertexData[] = 
	{
		-5, 0, -5, 1, 0, 0,
		5, 0, -5, 1, 1, 0,
		5, 10, -5, 1, 1, 1,
		-5, 10, -5, 1, 0, 1,
	};

	unsigned int indexData[] = 
	{
		0, 1, 2,
		0, 2, 3,
	};

	//Generates GL Buffers
	//Generate them together
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Add the following to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//create and bind buffers to a vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//delete[] vertexData;
	//delete[] indexData;
}

void Terrain::generateGrid(unsigned int rows, unsigned int cols)
{
	Texture* TerrainGrass = new Texture("./data/toontexture/sand.tga");
	Texture* TerrainDirt = new Texture("./data/toontexture/dirt2.tga");
	m_TerrainShader = new Shader("./data/Shaders/vs_terrain.vert", "./data/Shaders/fs_terrain.frag", TerrainGrass, TerrainDirt);

	//generate perlin noise
	int dims = cols;

	int octaves = 6;

	perlin_data = new float[dims * dims];
	float scale = (1.f / dims) * 6;
	for (int x = 0; x < dims; x++)
	{
		for (int y = 0; y < dims; y++)
		{
			float amplitude = 1.0f;
			float persistence = 0.3f;
			perlin_data[y * dims + x] = 0;

			for (int o = 0; o < octaves; o++)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(glm::vec2(x, y) * scale * freq) * 0.5f + 0.5f;
				perlin_data[y * dims + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}
	//Create texture with perlin data
	m_TerrainNoise = new Texture(dims, dims, perlin_data);
	//insert into shader
	m_TerrainShader->m_noisemap = m_TerrainNoise;

	//generate terrian grid
	m_rows = rows;
	m_cols = cols;

	Vertex* aoVertices = new Vertex[rows * cols];

	//generate displacement on cpu instead of vertex shader to calculate normals
	for (unsigned int x = 0; x < rows; ++x)
	{
		for (unsigned int y = 0; y < cols; ++y)
		{
			aoVertices[x * cols + y].position += glm::vec4((float)x, perlin_data[x * dims + y] * m_TerrainIntensity, (float)y, 1);
		}
	}

	//calc texture coords
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			glm::vec3 color = glm::vec3((float)sin((glfwGetTime() + aoVertices[r * cols + c].position.x) * 1.5f) * 1.f);
			glm::vec3 color2 = glm::vec3((float)sin((glfwGetTime() + aoVertices[r * cols + c].position.z) * 1.5f) * 1.f);

			aoVertices[r * cols + c].TexCoord = glm::vec2((float)c / (rows * 0.1), (float)r / (cols * 0.1));
		}
	}

	//calc normals
	for (unsigned int r = 0; r < rows - 1; ++r)
	{
		for (unsigned int c = 0; c < cols - 1; ++c)
		{
			glm::vec3 U = glm::normalize(aoVertices[r * cols + (c + 1)].position.xyz - aoVertices[r * cols + c].position.xyz);
			glm::vec3 V = glm::normalize(aoVertices[(r + 1) * cols + c].position.xyz - aoVertices[r * cols + c].position.xyz);

			aoVertices[r * cols + c].normal = glm::vec4(glm::normalize(glm::cross(U, V)), 0);
		}
	}

	//defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = (r * cols) + (c + 1);
		}
	}

	//Generates GL Buffers
	//Generate them together
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Add the following to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//create and bind buffers to a vertex array object
	//glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), ((char*)0) + 16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 32);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//IBO 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(UINT32), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	delete[] auiIndices;
	delete[] aoVertices;
}

void Terrain::Draw()
{
	//ReGenerates the terrain if tweak changes
	if (m_RefreshTerrain)
	{
		generateGrid(m_rows, m_cols);
		m_RefreshTerrain = false;
	}


	m_TerrainShader->DrawShader(m_CurrentCamera, glm::vec3(-32, -15, -32), glm::vec3(0.5f, 0.5f, 0.5f));

	glBindVertexArray(m_VAO);
	unsigned int indexCount = (m_rows - 1) * (m_cols - 1) * 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Terrain::DrawSingle()
{
	m_TerrainShader->DrawShader(m_CurrentCamera, glm::vec3(0, 5, 0));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Terrain::~Terrain()
{
	delete m_TerrainShader;
	delete m_TerrainNoise;
}
