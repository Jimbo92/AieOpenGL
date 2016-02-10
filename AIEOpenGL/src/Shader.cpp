#include "Shader.h"

using namespace std;

Shader::Shader(const char *VertexShaderPath, const char *FragmentShaderPath, Texture* TextureFile, Texture* NormalMap)
{
	m_textureFile = TextureFile;
	m_textureNormal = NormalMap;

	m_VertShader = LoadShader(VertexShaderPath);
	m_FragShader = LoadShader(FragmentShaderPath);

	const char* VertShaderSource = m_VertShader.c_str();
	const char* FragShaderSource = m_FragShader.c_str();

	//Compile Shader
	int success = GL_FALSE;
	unsigned int VertShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int FragShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Compile vertex Shader
	glShaderSource(VertShader, 1, (const char**)&VertShaderSource, 0);
	glCompileShader(VertShader);

	//Compile Fragment Shader
	glShaderSource(FragShader, 1, (const char**)&FragShaderSource, 0);
	glCompileShader(FragShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, VertShader);
	glAttachShader(m_programID, FragShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(VertShader);
	glDeleteShader(FragShader);
}

void Shader::DrawShader(Camera* CurrentCamera, glm::vec3 location, glm::vec3 scale, glm::vec3 RotationAxis, float RotationAmount)
{
	//bind shader
	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "ProjectionView");

	if (RotationAxis != glm::vec3(0))
	{
		RotMatrix = glm::rotate(RotationAmount, RotationAxis);
	}

	ScaleMatrix[0][0] = scale.x;
	ScaleMatrix[1][1] = scale.y;
	ScaleMatrix[2][2] = scale.z;

	LocalMatrix = ScaleMatrix * RotMatrix;

	LocalMatrix[3] = glm::vec4(location, 1);

	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(CurrentCamera->getProjectionView() * LocalMatrix));

	//set texture slot
	if (m_textureFile != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureFile->m_texture);
	}
	projectionViewUniform = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(projectionViewUniform, 0);

	//Set normal map
	if (m_textureNormal != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_textureNormal->m_texture);
	}
	projectionViewUniform = glGetUniformLocation(m_programID, "normal");
	glUniform1i(projectionViewUniform, 1);

	unsigned int timeUniform = glGetUniformLocation(m_programID, "time");
	glUniform1f(timeUniform, glfwGetTime());

	unsigned int lightdirUniform = glGetUniformLocation(m_programID, "lightdirection");
	glUniform3f(lightdirUniform, m_light->m_lightDir.x, m_light->m_lightDir.y, m_light->m_lightDir.z);

	unsigned int heightScaleUniform = glGetUniformLocation(m_programID, "heightScale");
	glUniform1f(heightScaleUniform, 1.1);
}

std::string Shader::LoadShader(const char *filePath)
{
	std::string content;

	std::ifstream file_Shader;

	file_Shader.open(filePath, std::ios::in);

	if (!file_Shader.is_open())
	{
		cout << "Could not read shader file" << filePath << endl;
		return "";
		assert(file_Shader);
	}

	string m_line = "";

	while (!file_Shader.eof())
	{
		std::getline(file_Shader, m_line);
		content.append(m_line + "\n");
	}

	file_Shader.close();
	return content;
}

Shader::~Shader()
{
	
}
