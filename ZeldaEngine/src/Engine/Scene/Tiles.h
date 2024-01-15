#pragma once 

#include <Engine/Application/Core.h> 
#include <Engine/Renderer/Bitmap.h>
#include <Engine/Scene/Grid.h>

namespace Engine
{ 
	typedef uint16_t Index; 
	typedef uint16_t Dim; 

	class TileLayer
	{  
	public: 
		using GridDpyFunk = std::function<void(Bitmap&, int, int, int, int)>; 

	public:
		void SetTile(Dim col, Dim row, Index index);
		Index GetTile(Dim col, Dim row) const; 
		const Point Pick(Dim x, Dim y) const; 

		const Rect& GetViewWindow(void);
		void SetViewWindow(const Rect& r); 
		void Display(Bitmap& dest, const Rect& displayArea);
		Bitmap GetBitmap(void) const;  
		GridLayer* GetGrid();

		int GetPixelWidth(void) const;
		int GetPixelHeight(void) const; 

		unsigned GetTileWidth(void) const;
		unsigned GetTileHeight(void) const; 

		void	PutTile(Bitmap& dest, Dim x, Dim y, Bitmap& tiles, Index tile);
		Dim		TileY(Index index);
		Dim		TileX(Index index);
		Index	MakeIndex(byte row, byte col);

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
		void DisplayGrid(Bitmap& dest, const GridDpyFunk& display_f);

		TileLayer(uint32_t id);
		~TileLayer();

	private:
		void Allocate(); 

	private:
		friend class GridLayer;
			
	private: 
		uint32_t	m_Id = 0;
		Index*		m_Map = nullptr;  
		GridLayer*	m_grid = nullptr;
		Dim			m_Totalrows = 0, m_Totalcolumns = 0; 
		Bitmap		m_Tileset;
		Rect		m_ViewWindow;
		Bitmap		m_DpyBuffer;
		bool		m_DpyChanged = 0; 
		Dim			m_DpyX = 0, m_DpyY = 0;  
		Point		m_ViewPosCached {-1, -1};  
		Bitmap		m_blankTile;
	};
}
