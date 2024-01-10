#pragma once 

#include<Engine/Application/Core.h> 
#include<Engine/Renderer/Bitmap.h>   
#include<Engine/Scene/AnimationFilm.h> 
#include<Engine/Renderer/MotionQuantizer.h> 
#include<Engine/Renderer/Clipper.h> 
#include<Engine/Scene/LatelyDestroyable.h>

#include<string>

namespace Engine
{ 
	class Clipper;
	class SpriteClass;  
	class BoundingArea;

	typedef SpriteClass* Sprite;

	class SpriteClass : public LatelyDestroyable
	{ 
	public: 
		using Mover = std::function<void(const Rect&, int* dx, int* dy)>; 

	public:
		SpriteClass(std::string _name, int _x, int _y, AnimationFilm* _film, const std::string& _typeid);

		void		 SetMover(const Mover& move); 
		const Rect	 GetBox(void) const; 
		void		 Move(int dx, int dy);
		void		 SetPos(int _x, int _y); 
		void		 SetZorder(unsigned z); 
		unsigned	 GetZorder(void); 
		void		 SetFrame(byte i); 
		byte		 GetFrame(void) const;   
		void		 SetFilm(AnimationFilm* film);
		void		SetBoundingArea(BoundingArea* area); 
		auto		GetBoundingArea() const -> BoundingArea* { return m_boundingArea; }
		
		const std::string&	GetTypeId();
		void				SetVisibility(bool v);
		bool				IsVisible(void) const;  
		bool				CollisionCheck(const SpriteClass* s) const;  
		//GravityHandler&		GetGravityHandler(void); 
		SpriteClass&		SetHasDirectMotion(bool v); 
		bool				GetHasDirectMotion(void) const; 

		std::string&		GetHashName();


		void Display(Bitmap& dest, const Rect& dpyArea, const Clipper& clipper) const;

	private:  
		std::string m_hashName; 
		byte m_frameNo = 0;
		Rect m_frameBox; // inside the film
		int m_x = 0, m_y = 0;
		bool m_isVisible = false;
		AnimationFilm* m_currFilm = nullptr;
		BoundingArea* m_boundingArea = nullptr;
		unsigned m_zorder = 0;
		std::string m_typeId, m_stateId;
		Mover m_mover;
		MotionQuantizer m_quantizer;
	};
}
