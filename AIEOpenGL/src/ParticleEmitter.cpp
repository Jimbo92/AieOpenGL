#include "ParticleEmitter.h"



ParticleEmitter::ParticleEmitter() : m_Particles(nullptr), m_firstDead(0), m_maxParticles(0), m_position(0,0,0), m_vao(0), m_vbo(0), m_ibo(0), m_vertexData(nullptr)
{

}

void ParticleEmitter::initalise(unsigned int a_maxParticles,
	unsigned int a_emitRate,
	float a_lifetimeMin, float a_lifetimeMax,
	float a_velocityMin, float a_velocityMax,
	float a_startSize, float a_endSize,
	const glm::vec4& a_startColor, const glm::vec4& a_endColor)
{
	//setup shader
	Texture* Particle_texture = new Texture("./data/explosion.png");
	m_ParticleShader = new Shader("./data/Shaders/vs_particle.vert", "./data/Shaders/fs_particle.frag", Particle_texture);

	//Set up emit timers
	m_emitTimer = 0;
	m_emitRate = 1.f / a_emitRate;

	//store all the variables passed in
	m_startColor = a_startColor;
	m_endColor = a_endColor;
	
	m_startSize = a_startSize;
	m_endSize = a_endSize;

	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;

	m_lifespanMin = a_lifetimeMin;
	m_lifespanMax = a_lifetimeMax;

	m_maxParticles = a_maxParticles;

	//create particle array
	m_Particles = new Particle[m_maxParticles];
	m_firstDead = 0;

	//create the array of verts for particles
	//4 verts per particle for quad
	//will be filled during update
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	for (unsigned int i = 0; i < m_maxParticles; i++)
	{
		m_vertexData[i * 4 + 0].textcoords = glm::vec2(0.5f, 0.5f);

		m_vertexData[i * 4 + 1].textcoords = glm::vec2(-0.5f, 0.5f);

		m_vertexData[i * 4 + 2].textcoords = glm::vec2(-0.5f, -0.5f);

		m_vertexData[i * 4 + 3].textcoords = glm::vec2(0.5f, -0.5f);
	}

	//create the index buffer data for particles
	//6 indices per quad of 2 triangles
	//fill it now as it never changes
	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; ++i)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	//create opengl buffer
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(ParticleVertex), m_vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // color
	glEnableVertexAttribArray(2); // TextureCoords

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), ((char*)0) + 16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), ((char*)0) + 32);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}

void ParticleEmitter::emit()
{
	//only emit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
		return;

	//resurrect the first dead particle
	Particle& Resparticle = m_Particles[m_firstDead++];

	//assign its starting position
	Resparticle.position = m_position;

	//randomise its lifespan
	Resparticle.lifetime = 0;
	Resparticle.lifespan = (rand() / (float)RAND_MAX) * (m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	//set starting size and color
	Resparticle.color = m_startColor;
	Resparticle.size = m_startSize;

	//randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * (m_velocityMax - m_velocityMin) + m_velocityMin;

	Resparticle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	Resparticle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	Resparticle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	Resparticle.velocity = glm::normalize(Resparticle.velocity) * velocity;
}

void ParticleEmitter::update(float DeltaTime, const glm::mat4& a_cameraTransform)
{

	//spawn particles
	m_emitTimer += DeltaTime;
	while (m_emitTimer > m_emitRate)
	{
		emit();
		m_emitTimer -= m_emitRate;
	}

	unsigned int quad = 0;

	//update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < m_firstDead; i++)
	{
		Particle* currParticle = &m_Particles[i];

		currParticle->lifetime += DeltaTime;
		if (currParticle->lifetime >= currParticle->lifespan)
		{
			//swap last alive with this one
			*currParticle = m_Particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			//move particle
			currParticle->position += currParticle->velocity * DeltaTime;

			//size particle
			currParticle->size = glm::mix(m_startSize, m_endSize, currParticle->lifetime / currParticle->lifespan);

			//color particle
			currParticle->color = glm::mix(m_startColor, m_endColor, currParticle->lifetime / currParticle->lifespan);

			//make quad the correct size and color
			float halfsize = currParticle->size * 0.5f;

			m_vertexData[quad * 4 + 0].position = glm::vec4(halfsize, halfsize, 0, 1);
			m_vertexData[quad * 4 + 0].color = currParticle->color;

			m_vertexData[quad * 4 + 1].position = glm::vec4(-halfsize, halfsize, 0, 1);
			m_vertexData[quad * 4 + 1].color = currParticle->color;

			m_vertexData[quad * 4 + 2].position = glm::vec4(-halfsize, -halfsize, 0, 1);
			m_vertexData[quad * 4 + 2].color = currParticle->color;

			m_vertexData[quad * 4 + 3].position = glm::vec4(halfsize, -halfsize, 0, 1);
			m_vertexData[quad * 4 + 3].color = currParticle->color;

			//create billboard transform
			glm::vec3 zAxis = glm::normalize(glm::vec3(a_cameraTransform[3]) - currParticle->position);
			glm::vec3 xAxis = glm::cross(glm::vec3(a_cameraTransform[1]), zAxis);
			glm::vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(glm::vec4(xAxis, 0),
				glm::vec4(yAxis, 0),
				glm::vec4(zAxis, 0),
				glm::vec4(0, 0, 0, 1));

			for (unsigned int index = 0; index < 4; index++)
			{
				m_vertexData[quad * 4 + index].position = billboard * m_vertexData[quad * 4 + index].position + glm::vec4(currParticle->position, 0);
			}

			++quad;
		}
	}
}

void ParticleEmitter::draw(Camera* currentCamera)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	//draw shader
	m_ParticleShader->DrawShader(currentCamera, glm::vec3(3, 1, 3));

	//sync the particle vertex buffer
	//based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	//draw particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}


ParticleEmitter::~ParticleEmitter()
{
	delete[] m_Particles;
	delete[] m_vertexData;

	delete m_ParticleShader;

	//delete buffers
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}
