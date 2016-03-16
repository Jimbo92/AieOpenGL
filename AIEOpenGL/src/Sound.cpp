#include <Sound.h>
#include <iostream>

Sound::Sound(const char* File)
{
	onCreate();
	m_fmodresult = m_pFmodSystem->createSound(File, FMOD_DEFAULT, 0, &pSound);
}

Sound::~Sound()
{
	pSound->release();

	m_pFmodSystem->close();
	m_pFmodSystem->release();
}

void Sound::PlaySound()
{
	m_fmodresult = m_pFmodSystem->playSound(pSound, 0, false, &pChannel);
}

void Sound::Update()
{
	m_pFmodSystem->update();
}

bool Sound::onCreate()
{
	//init FMOD
	m_pFmodSystem = NULL;

	//create the main system object
	m_fmodresult = FMOD::System_Create(&m_pFmodSystem);

	if (m_fmodresult != FMOD_OK)
	{
		std::cout << "FMOD error!" << m_fmodresult << FMOD_ErrorString(m_fmodresult) << std::endl;
		return false;
	}

	//init FMOD with 512 channels
	m_fmodresult = m_pFmodSystem->init(512, FMOD_INIT_NORMAL, 0);

	if (m_fmodresult != FMOD_OK)
	{
		std::cout << "FMOD error!" << m_fmodresult << FMOD_ErrorString(m_fmodresult) << std::endl;
		return false;
	}

	return true;
}