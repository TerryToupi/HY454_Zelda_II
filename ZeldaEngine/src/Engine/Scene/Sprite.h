#pragma once 

#include<Engine/Application/Core.h> 
#include<Engine/Renderer/Bitmap.h>
#include<string>

namespace Engine
{ 
	class Clipper;
	class Scene; 

	class Sprite
	{ 
	public: 
		using Mover = std::function<void(const Rect&, int* dx, int* dy)>;

		Sprite(std::string name, Scene* scene);  

		void		SetMover(const Mover& move); 
		const Rect	GetBox(void) const; 
		void		Move(int dx, int dy);
		void		SetPos(int _x, int _y); 
		void		SetZorder(unsigned z); 
		unsigned	GetZorder(void); 
		void		SetFrame(byte i); 
		byte		GetFrame(void) const;  

		// Bounding Area
		// void		SetBoundingArea()  
		// auto		GetBoundingArea() const -> const BoundingArea* { return boundingArea; }
		
		const std::string& GetTypeId();
		void		SetVisibility(bool v);
		bool		IsVisible(void) const;  
		bool		CollisionCheck(const Sprite& s) const; 
		void		Display(Bitmap& dest, const Rect& dpyArea, const Clipper& clipper) const;

	private:  
		Scene* m_Scene; 
		std::string m_HashName;
	};
}
