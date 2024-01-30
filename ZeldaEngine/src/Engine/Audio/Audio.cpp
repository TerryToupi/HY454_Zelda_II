#include "Audio.h" 
#include <algorithm>  
#include <audio.h>

#define MAX_AVAILABLE_AUDIO_DEVICES 12

namespace Engine
{   
	AudioManager AudioManager::s_instance; 

	void AudioManager::Init()
	{     
		AudioData dummy;
		if (SDL_LoadWAV("Assets/Sounds/DummySound/dummy.wav", &dummy.wavSpec, &dummy.wavBuffer, &dummy.wavLength) == nullptr)
			ENGINE_CORE_ERROR("Not valid Dummy file for sound settings");

		s_instance.m_head = new DeviceChain();
		s_instance.m_head->deviceID = SDL_OpenAudioDevice(NULL, 0, &dummy.wavSpec, NULL, 0); 
		s_instance.m_head->next = nullptr; 
		s_instance.m_itr = s_instance.m_head;

		for (int i = 0; i < MAX_AVAILABLE_AUDIO_DEVICES; ++i)
		{ 
			DeviceChain* next = new DeviceChain();  
			next->deviceID = SDL_OpenAudioDevice(NULL, 0, &dummy.wavSpec, NULL, 0); 
			next->next = nullptr; 
			s_instance.m_itr->next = next;
			s_instance.m_itr = next; 
		} 

		s_instance.m_itr->next = s_instance.m_head; 
		SDL_FreeWAV(dummy.wavBuffer); 

		initAudio();
	}

	void AudioManager::Shutdown()
	{  
		DeviceChain* curr;
		for (int i = 0; i < MAX_AVAILABLE_AUDIO_DEVICES; ++i)
		{
			SDL_CloseAudioDevice(s_instance.m_itr->deviceID);  
			curr = s_instance.m_itr->next;
			delete m_itr;
			m_itr = curr;
		}

		for (auto& data : m_sounds)
		{ 
			if (data.second)
			{
				SDL_FreeWAV(data.second->wavBuffer);
				delete data.second;
			}
		} 

		endAudio();
	}

	AudioID AudioManager::LoadSound(std::string _path)
	{ 
		AudioData* data = new AudioData();  

		if (SDL_LoadWAV(_path.c_str(), &data->wavSpec, &data->wavBuffer, &data->wavLength) == nullptr)
			ENGINE_CORE_ERROR("Incorrect audio file source"); 
		
		data->id = UUID::GenerateUUID();  
		m_sounds[data->id] = data;

		return data->id;
	} 

	void AudioManager::PlaySound(AudioID _id)
	{	
		if (m_sounds.find(_id) != m_sounds.end())
		{ 
			SDL_QueueAudio(m_itr->deviceID, m_sounds[_id]->wavBuffer, m_sounds[_id]->wavLength); 
			SDL_PauseAudioDevice(m_itr->deviceID, 0); 
			m_itr = m_itr->next;
		}
	}

	void AudioManager::DeleteSound(AudioID _id)
	{
		if (m_sounds.find(_id) != m_sounds.end())
		{
			SDL_FreeWAV(m_sounds[_id]->wavBuffer); 
			delete m_sounds[_id];  
			m_sounds.erase(_id);
		}
	}

	void AudioManager::PlayMusic(std::string _path, int volume)
	{ 
		playMusic(_path.c_str(), volume);
	}

	void AudioManager::PauseMusic()
	{  
		pauseAudio();
	} 

	void AudioManager::UnPauseMusic()
	{ 
		unpauseAudio();
	}
}

