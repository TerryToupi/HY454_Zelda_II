#include "Audio.h" 
#include <algorithm> 

#define MAX_AVAILABLE_AUDIO_DEVICES 8

namespace Engine
{   
	AudioManager AudioManager::s_instance; 

	void callbackfun(void* data, Uint8* buffer, int buffer_len)
	{
		MusicData* music = (MusicData*)data; 

		if (music->wavLength == 0 && !music->looping)
		{ 
			return;
		} 
		else if (music->wavLength == 0 && music->looping) 
		{
			music->wavBuffer = music->wavBufferConst; 
			music->wavLength = music->wavLengthConst;
		}

		Uint32 len = (Uint32)buffer_len; 
		len = (len > music->wavLength ? music->wavLength : len);

		SDL_memcpy(buffer, music->wavBuffer, len); 

		music->wavBuffer += len;
		music->wavLength -= len;
	}

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

		for (auto& data : m_tracs)
		{ 
			DeleteMusicAndDevice(data.first);
		}
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

	DeviceID AudioManager::InitMusicDevice(std::string _path, bool looping)
	{ 
		MusicData* data = new MusicData();  

		if (SDL_LoadWAV(_path.c_str(), &data->wavSpec, &data->wavBuffer, &data->wavLength) == nullptr)
			ENGINE_CORE_ERROR("Incorrect audio file source");  
 
		data->looping = looping;
		data->wavSpec.userdata = data;  
		data->wavLengthConst = data->wavLength;
		data->wavBufferConst = data->wavBuffer;
		data->wavSpec.callback = callbackfun;
		data->id = SDL_OpenAudioDevice(NULL, 0, &data->wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE); 
		SDL_PauseAudioDevice(data->id, 1); 

		m_tracs[data->id] = data;

		return data->id;
	}

	void AudioManager::PauseMusicDevice(DeviceID _device, bool _v)
	{ 
		SDL_PauseAudioDevice(_device, _v);
	}

	void AudioManager::DeleteMusicAndDevice(DeviceID _device)
	{ 
		if (m_tracs.find(_device) != m_tracs.end())
		{
			SDL_PauseAudioDevice(_device, 1); 
			SDL_CloseAudioDevice(_device);
			SDL_FreeWAV(m_tracs[_device]->wavBuffer);
			delete m_tracs[_device]; 
			m_tracs.erase(_device);
		}
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
}

