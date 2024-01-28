#pragma once 

#include<Engine/Application/Core.h> 
#include<Engine/Renderer/Bitmap.h>   
#include<Engine/Scene/AnimationFilm.h> 
#include<Engine/Renderer/MotionQuantizer.h> 
#include<Engine/Renderer/Clipper.h> 
#include<Engine/Scene/LatelyDestroyable.h> 
#include<Engine/Scene/BoundingArea.h> 
#include<Engine/Scene/Gravity.h>

#include<string>

namespace Engine
{ 
	class Clipper;
	class SpriteClass;  

	typedef SpriteClass* Sprite;

	class SpriteClass : public LatelyDestroyable
	{ 
	public: 
		using Mover = std::function<void(Rect& r, int* dx, int* dy)>; 

	public:
		SpriteClass(std::string _name, int _x, int _y, AnimationFilm* _film, const std::string& _typeid);

		void		 SetMover(const Mover& move); 
		Rect		 GetBox(void); 
		SpriteClass& Move(int dx, int dy);
		void		 SetPos(int _x, int _y); 
		void		 SetZorder(int z); 
		unsigned	 GetZorder(void); 
		void		 SetFrame(byte i);  
		int			 GetPosX() const;
		int			 GetPosY() const;
		byte		 GetFrame(void) const;   
		void		 SetFilm(AnimationFilm* film); 
		auto		 GetFilm() const->AnimationFilm* { return m_currFilm; }
		void		 SetBoundingArea(BoundingArea* area); 
		void		 SetColiderBox(unsigned _w, unsigned _h);
		auto		 GetBoundingArea() const -> BoundingArea* { return m_boundingArea; }  
		void		 SetMotionQuantizerUse(bool v);
		void	     SetQuanntizerVertHorz(int v, int h);
		
		const std::string&	GetTypeId();
		void				SetVisibility(bool v);
		bool				IsVisible(void) const;  
		bool				CollisionCheck(const Sprite s);  
		GravityHandler&		GetGravityHandler(void); 
		SpriteClass&		SetHasDirectMotion(bool v);
		bool				GetHasDirectMotion(void) const; 

		std::string&		GetHashName();


		void Display(Bitmap& dest, const Rect& dpyArea, const Clipper& clipper);

	private:  
		std::string m_hashName; 
		byte m_frameNo = 0; 
		Rect m_frameBox { 0, 0, 0, 0 }; // inside the film 
		bool m_emptyFrameBox = true;
		Rect m_hitBox;
		int m_x = 0, m_y = 0;
		bool m_isVisible = false;
		AnimationFilm* m_currFilm = nullptr;
		BoundingArea* m_boundingArea = nullptr;
		int m_zorder = 0;
		std::string m_typeId, m_stateId;
		Mover m_mover;
		MotionQuantizer m_quantizer;  
		GravityHandler m_gravity;
		bool m_directMotion = false;
	};
}
