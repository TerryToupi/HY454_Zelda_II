#pragma once 

#include <Engine/Application/Core.h> 
#include <Engine/Renderer/Bitmap.h>

namespace Engine
{ 
	typedef uint16_t Index; 
	typedef uint16_t Dim;

	class TileLayer
	{  
	public: 
		void SetTile(Dim col, Dim row, Index index);
		Index GetTile(Dim col, Dim row) const; 
		const Point Pick(Dim x, Dim y) const; 

		const Rect& GetViewWindow(void);
		void SetViewWindow(const Rect& r); 
		void Display(Bitmap& dest, const Rect& displayArea);
		Bitmap GetBitmap(void) const; 

		int GetPixelWidth(void) const;
		int GetPixelHeight(void) const; 

		unsigned GetTileWidth(void) const;
		unsigned GetTileHeight(void) const; 

		void Scroll(float dx, float dy); 
		void FilterScrollDistance(uint32_t viewStartCoord, uint32_t viewSize, uint32_t* d, uint32_t maxMapSize);
		void FilterScroll(uint32_t* dx, uint32_t* dy); 
		void ScrollWithBoundsCheck(uint32_t _dx, uint32_t _dy);
		bool CanScrollHoriz(float dx) const;
		bool CanScrollVert(float dy) const;

		auto ToString(void) const -> const std::string; // unparse
		bool FromString(const std::string&); // parse
		void Save(const std::string& path) const;
		bool Load(const std::string& path);
		FILE* WriteText(FILE* fp) const;
		bool ReadText(FILE* fp);  

		void LoadTiles(std::string path);

		TileLayer(uint32_t id);
		~TileLayer();

	private:
		void Allocate();
			
	private: 
		uint32_t	m_Id = 0;
		Index*		m_Map = nullptr; 
		// GRID
		Dim			m_Totalrows = 0, m_Totalcolumns = 0; 
		Bitmap		m_Tileset;
		Rect		m_ViewWindow;
		Bitmap		m_DpyBuffer;
		bool		m_DpyChanged = 0; 
		Dim			m_DpyX = 0, m_DpyY = 0;  
		Point		m_ViewPosCached {-1, -1}; 
	};
}
