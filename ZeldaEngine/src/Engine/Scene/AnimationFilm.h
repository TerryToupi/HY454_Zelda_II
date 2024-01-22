#pragma once  

#include <Engine/Application/Core.h>  
#include <Engine/Renderer/Bitmap.h> 
#include <Engine/Scene/AnimationSheet.h> 
#include <Engine/Scene/LatelyDestroyable.h>
#include <Engine/Math/Math.h> 
#include <string>

namespace Engine
{
	class AnimationFilm : public LatelyDestroyable
	{ 
	public: 
		using FrameBoxes = std::vector<Rect>; 

	public:  
		AnimationFilm(AnimationSheet* sheet, std::string _configPath) 
			: m_animationSheet(sheet) { Generate(_configPath); }

		uint32_t	GetTotalFrames(void) const { return m_boxes.size() - 1; } 
		Bitmap&		GetBitmap(void) { return m_animationSheet->GetSheet(); } 
		auto		GetId(void) -> const std::string& { return m_name; } 
		auto		GetFrameBox(uint32_t boxNo) -> const Rect& 
						{ ENGINE_CORE_ASSERT(!m_boxes.empty()); return m_boxes[boxNo]; } 

		void		DisplayFrame(Bitmap& dest, Rect from, Rect to, uint32_t boxNo) 
						{ Bitmap::Blit(m_animationSheet->GetSheet(), &from, dest, &to); }

	private:
		void		Generate(std::string configPath);
		void		Append(const Rect& r) { m_boxes.push_back(r); }

	private: 
		FrameBoxes		m_boxes;
		AnimationSheet* m_animationSheet; 
		std::string		m_name;
	};
}
