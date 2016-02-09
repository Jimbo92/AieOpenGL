#include "Shader.h"

using namespace std;

Shader::Shader()
{

}

string Shader::ReadShader(const char *filePath)
{
	std::string content;

	std::ifstream file_Shader;

	file_Shader.open(filePath, std::ios::in);

	if (!file_Shader.is_open())
	{
		cout << "Could not read shader file" << endl;
		return "";
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

void Shader::LoadShader()
{
	
}

Shader::~Shader()
{
	
}
