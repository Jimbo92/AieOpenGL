#pragma once


#include "tiny_obj_loader.h"
#include <string.h>
#include "gl_core_4_4.h"
#include <glm\mat4x4.hpp>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <map>

using namespace std;

class TextureManager
{
private:
	map<string, uint32> m_textureMap;
	static TextureManager* sm_singleton;
	TextureManager() {};
	uint32 getTextureIDPrivate(const string& textureName);
public:
	static void create();

	static uint32 getTextureID(const string& textureName);
	void loadTexture(uint32& m_texture, const string& path);


};
