#include "TextureManager.h"


void TextureManager::loadTexture(uint32& m_texture, const string& path)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* data = stbi_load(path.c_str(), &imageWidth, &imageHeight, &imageFormat, STBI_default);
	//	assert(data != nullptr);
	if (data != nullptr)
	{
		glGenTextures(1, &(GLuint)m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		switch (imageFormat)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imageWidth, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, imageWidth, imageHeight, 0, GL_R8, GL_UNSIGNED_BYTE, data);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		stbi_image_free(data);
	}
	else
	{
		cout << "can't load texture: " << path << endl;
		m_texture = 0; //set the texture to 0 so it has somethine sensible in it.  Good idea to preload texture 0 with pink
	}
}

TextureManager* TextureManager::sm_singleton = nullptr;

void TextureManager::create() {
	if (sm_singleton == nullptr)
		sm_singleton = new TextureManager();
}

uint32 TextureManager::getTextureIDPrivate(const string& textureName)
{
	auto itter = m_textureMap.find(textureName);
	if (itter == m_textureMap.end())
	{
		uint32 textureHandle;
		//load texture
		loadTexture(textureHandle,textureName);
		m_textureMap.insert(std::pair<std::string, int>(textureName, static_cast<uint32>(textureHandle) ));
		return textureHandle;
	}
	else
	{
		return m_textureMap[textureName];
	}
}

//static function to get texture ID from singleton class
uint32 TextureManager::getTextureID(const string& textureName)
{
	if (sm_singleton != nullptr)
	{
		return sm_singleton->getTextureIDPrivate(textureName);
	}
	TextureManager::create();
	return sm_singleton->getTextureIDPrivate(textureName);
}