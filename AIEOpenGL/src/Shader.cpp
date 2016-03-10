#include "Shader.h"

using namespace std;

Shader::Shader(const char *VertexShaderPath, const char *FragmentShaderPath, Texture* TextureFile, Texture* NormalMap, Texture* SpecMap)
{
	m_noisemap = nullptr;

	m_textureFile = TextureFile;
	m_textureNormal = NormalMap;
	m_textureSpecmap = SpecMap;

	m_Textures.push_back(TextureFile);
	m_Textures.push_back(NormalMap);
	m_Textures.push_back(SpecMap);

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

void Shader::UpdateBones(FBXSkeleton* skeleton)
{
	if (skeleton != nullptr)
	{
		skeleton->updateBones();

		unsigned int boneLocationUniform = glGetUniformLocation(m_programID, "bones");
		glUniformMatrix4fv(boneLocationUniform, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);
	}
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

	unsigned int LocalMatrixUniform = glGetUniformLocation(m_programID, "LocalLocation");
	glUniform4f(LocalMatrixUniform, LocalMatrix[3].x, LocalMatrix[3].y, LocalMatrix[3].z, LocalMatrix[3].w);

	//set texture slot
	if (m_Textures[0] != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Textures[0]->m_texture);
	}
	projectionViewUniform = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(projectionViewUniform, 0);

	//Set normal map
	if (m_Textures[1] != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Textures[1]->m_texture);
	}
	projectionViewUniform = glGetUniformLocation(m_programID, "normal");
	glUniform1i(projectionViewUniform, 1);

	//Set spec map
	if (m_Textures[2] != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_Textures[2]->m_texture);
	}
	projectionViewUniform = glGetUniformLocation(m_programID, "specmap");
	glUniform1i(projectionViewUniform, 2);

	//Set Noise map
	if (m_noisemap != nullptr)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_noisemap->m_texture);
	}
	projectionViewUniform = glGetUniformLocation(m_programID, "noisemap");
	glUniform1i(projectionViewUniform, 3);

	//Set extra map 1
	if (m_extramap1 != nullptr)
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_extramap1->m_texture);
	}
	projectionViewUniform = glGetUniformLocation(m_programID, "extramap1");
	glUniform1i(projectionViewUniform, 5);

	//RenderTarget map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_rendertargetTexture);
	unsigned int RenderTargetUniform = glGetUniformLocation(m_programID, "rendertargetmap");
	glUniform1i(RenderTargetUniform, 4);

	unsigned int timeUniform = glGetUniformLocation(m_programID, "time");
	glUniform1f(timeUniform, (float)glfwGetTime());

	unsigned int alphaUniform = glGetUniformLocation(m_programID, "alpha");
	glUniform1f(alphaUniform, m_alpha);

	unsigned int specpowUniform = glGetUniformLocation(m_programID, "specpow");
	glUniform1f(specpowUniform, m_specpow);

	unsigned int foamIntensityUniform = glGetUniformLocation(m_programID, "foamIntensity");
	glUniform1f(foamIntensityUniform, m_foamIntensity);

	unsigned int fogstartUniform = glGetUniformLocation(m_programID, "fogStart");
	glUniform1f(fogstartUniform, m_fogStart);

	glm::vec4 camPos = CurrentCamera->getTransform()[3];
	unsigned int cameraposUniform = glGetUniformLocation(m_programID, "camerapos");
	glUniform3f(cameraposUniform, camPos.x, camPos.y, camPos.z);

	if (m_light != nullptr)
	{
		unsigned int lightdirUniform = glGetUniformLocation(m_programID, "lightdirection");
		glUniform3f(lightdirUniform, m_light->m_lightDir.x, m_light->m_lightDir.y, m_light->m_lightDir.z);

		unsigned int lightposUniform = glGetUniformLocation(m_programID, "lightposition");
		glUniform3f(lightposUniform, m_light->m_lightPos.x, m_light->m_lightPos.y, m_light->m_lightPos.z);

		unsigned int lightcolorUniform = glGetUniformLocation(m_programID, "lightcolor");
		glUniform4f(lightcolorUniform, m_light->m_lightColor.r, m_light->m_lightColor.g, m_light->m_lightColor.b, m_light->m_lightColor.a);

		unsigned int lightrangeUniform = glGetUniformLocation(m_programID, "lightrange");
		glUniform1f(lightrangeUniform, m_light->m_lightRange);
	}

	unsigned int ambientcolorUniform = glGetUniformLocation(m_programID, "ambient");
	glUniform3f(ambientcolorUniform, m_ambientlight.r, m_ambientlight.g, m_ambientlight.b);

	unsigned int heightScaleUniform = glGetUniformLocation(m_programID, "heightScale");
	glUniform1f(heightScaleUniform, 1.1f);
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
