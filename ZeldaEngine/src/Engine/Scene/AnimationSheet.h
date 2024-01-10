#pragma once 

#include <Engine/Application/Core.h>
#include <Engine/Renderer/Bitmap.h>  
#include <string>

namespace Engine
{  
	class AnimationSheet
	{ 
	public:  
		AnimationSheet(std::string _id, std::string _path) 
			: m_id(_id) { Generate(_path); }

		Bitmap& GetSheet(void) 
				{ return m_cashedSheet; }  

		void Generate(std::string& _path) 
				{ m_cashedSheet.LoadBMP(_path); } 

		void Reset(void) { m_cashedSheet.Reset(); }

	private:  
		std::string& m_id;
		Bitmap m_cashedSheet;
	};
}
