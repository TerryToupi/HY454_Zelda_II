#include "Audio.h" 
#include <algorithm>

namespace Engine
{ 
	AudioManager AudioManager::s_instance;

	Uint32 AudioManager::LoadSound(std::string _path)
	{ 
		AudioData* data = new AudioData(); 

		if (SDL_LoadWAV(_path.c_str(), &data->wavSpec, &data->wavBuffer, &data->wavLength) == nullptr)
			ENGINE_CORE_ERROR("Not valid .wav file or path error"); 

		SDL_AudioDeviceID id = SDL_OpenAudioDevice(NULL, 0, &data->wavSpec, NULL, 0); 
		data->deviceID = id;
		int success = SDL_QueueAudio(data->deviceID, data->wavBuffer, data->wavLength); 
		ENGINE_CORE_ASSERT(success);  

		SDL_PauseAudioDevice(id, 1);
		
		m_data.push_back(data); 
		return id;
	} 

	void AudioManager::PlaySound(int _deviceID)
	{ 
		SDL_PauseAudioDevice(_deviceID, 0);
	} 

	void AudioManager::PauseSound(int _deviceID)
	{ 
		SDL_PauseAudioDevice(_deviceID, 1);
	}

	void AudioManager::DeleteSound(int _deviceID)
	{  
		auto itr = find(_deviceID); 
		AudioData* data = (AudioData*)(*itr);
		SDL_CloseAudioDevice((*itr)->deviceID);
		SDL_FreeWAV((*itr)->wavBuffer); 
		m_data.erase(itr); 
		delete data;
	}

	auto AudioManager::find(int _deviceID) -> std::list<AudioData*>::iterator
	{
		return std::find_if( 
			m_data.begin(), 
			m_data.end(), 
			[_deviceID](const AudioData* _d) {
				return _d->deviceID == _deviceID;
			}
		);
	}
}

