#include "AnimationFilm.h" 

#include <json.hpp>
#include <iostream>
#include <fstream> 

using json = nlohmann::json;

namespace Engine
{
	void AnimationFilm::Generate(std::string configPath)
	{
		std::ifstream file(configPath);
		json animConfig = json::parse(file); 

		m_name = animConfig["animation"].get<std::string>(); 

		for (int frame = 0; frame < animConfig["total_frames"].get<int>(); frame++)
		{ 
			Append({
				animConfig["frames"][frame]["x"].get<int>(),
				animConfig["frames"][frame]["y"].get<int>(),
				animConfig["frames"][frame]["w"].get<int>(),
				animConfig["frames"][frame]["h"].get<int>()
			});
		}
	}
}
