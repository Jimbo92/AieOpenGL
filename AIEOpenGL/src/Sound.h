#pragma once

#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <fmod.hpp>
#include <fmod_errors.h>

class Sound
{
public:
	Sound(const char* File);
	~Sound();

	void Update();

	FMOD::System* m_pFmodSystem;
	FMOD_RESULT m_fmodresult;

	FMOD::Sound* pSound;
	FMOD::Channel* pChannel;

	void PlaySound();

private:
	bool onCreate();
};