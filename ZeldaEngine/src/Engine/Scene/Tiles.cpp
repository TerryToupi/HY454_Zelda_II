#include "Tiles.h"   

#include <Engine/Renderer/Render.h> 
#include <json.hpp>  

#include <iostream>
#include <fstream>
#include <iomanip>

#define		TILE_WIDTH			16
#define		TILE_HEIGHT			16 
#define		TILESET_WIDTH		16 
#define		TILESET_HEIGHT		16
#define		ROW_MASK			0x0F
#define		COL_MASK			0xF0
#define		COL_SHIFT			4
#define		MUL_TILE_WIDTH(i)	((i)<<4) 
#define		MUL_TILE_HEIGHT(i)	((i)<<4) 
#define		DIV_TILE_WIDTH(i)	((i)>>4) 
#define		DIV_TILE_HEIGHT(i)	((i)>>4) 
#define		MOD_TILE_WIDTH(i)	((i)&15) 
#define		MOD_TILE_HEIGHT(i)	((i)&15)
#define		TILEX_MASK			0xFF00
#define		TILEX_SHIFT			8
#define		TILEY_MASK			0x00FF
#define		EMTY_TILE			0xFFFF
#define		TRANSPARENT			0x00000000 

#define		GRID_ELEMENT_WIDTH		4
#define		GRID_ELEMENT_HEIGHT		4
#define		GRID_BLOCK_COLUMNS		(TILE_WIDTH / GRID_ELEMENT_WIDTH)
#define		GRID_BLOCK_ROWS			(TILE_HEIGHT / GRID_ELEMENT_HEIGHT)
#define		GRID_ELEMENTS_PER_TILE	(GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS) 

//#define GRID_MAX_HEIGHT (MAX_HEIGHT * GRID_BLOCK_ROWS)
//#define GRID_MAX_WIDTH (MAX_WIDTH * GRID_BLOCK_COLUMNS)

#define		GRID_THIN_AIR_MASK		0x0000 // element is ignored
#define		GRID_LEFT_SOLID_MASK	0x0001 // bit 0
#define		GRID_RIGHT_SOLID_MASK	0x0002 // bit 1
#define		GRID_TOP_SOLID_MASK		0x0004 // bit 2
#define		GRID_BOTTOM_SOLID_MASK	0x0008 // bit 3
#define		GRID_GROUND_MASK		0x0010 // bit 4, keep objects top / bottom (gravity)
#define		GRID_FLOATING_MASK		0x0020 // bit 5, keep objects anywhere inside (gravity)

#define		GRID_EMPTY_TILE GRID_THIN_AIR_MASK
#define		GRID_SOLID_TILE \
(GRID_LEFT_SOLID_MASK | GRID_RIGHT_SOLID_MASK | GRID_TOP_SOLID_MASK | GRID_BOTTOM_SOLID_MASK)

//#define		MAX_PIXEL_WIDTH				MUL_TILE_WIDTH(MAX_WIDTH)
//#define		MAX_PIXEL_HEIGHT			MUL_TILE_HEIGHT(MAX_HEIGHT)
#define		DIV_GRID_ELEMENT_WIDTH(i)	((i)>>2)
#define		DIV_GRID_ELEMENT_HEIGHT(i)	((i)>>2)
#define		MUL_GRID_ELEMENT_WIDTH(i)	((i)<<2)
#define		MUL_GRID_ELEMENT_HEIGHT(i)	((i)<<2)

using json = nlohmann::json;

namespace Engine
{  
	Bitmap BlankTile;  

	Index TileLayer::MakeIndex(byte row, byte col)
	{
		return (MUL_TILE_WIDTH(col) << TILEX_SHIFT) | MUL_TILE_HEIGHT(row);  
	} 

	Dim TileLayer::TileX(Index index)
	{   
		return index >> TILEX_SHIFT;
	} 

	Dim TileLayer::TileY(Index index)
	{
		return index & TILEY_MASK;
	} 

	void TileLayer::PutTile(Bitmap& dest, Dim x, Dim y, Bitmap& tiles, Index tile)
	{  
		if (tile != EMTY_TILE)
		{
			Rect TileSetPos{ TileX(tile), TileY(tile), TILE_WIDTH, TILE_HEIGHT };
			Rect DpyPos{ x, y, TILE_WIDTH, TILE_HEIGHT };
			Bitmap::Blit (
				tiles,	&TileSetPos,
				dest,	&DpyPos
			);
		}
		else
		{
			Rect TileSetPos{ 0, 0, TILE_WIDTH, TILE_HEIGHT };
			Rect DpyPos{ x, y, TILE_WIDTH, TILE_HEIGHT };
			Bitmap::Blit (
				BlankTile,	&TileSetPos,
				dest,	&DpyPos
			);
		}

		return;
	}

	void printGrid(int* grid) {
		int m_TotalCol = 4;

		for (Dim row = 0; row < 4; row++) {

			for (Dim col = 0; col < 4; col++) {
				std::cout << " | " << std::setw(2) << grid[row * m_TotalCol + col] << " | ";
			}
			std::cout << std::endl;

			for (int i = 0; i < 31; i++)
				std::cout << "-";
			std::cout << std::endl;
		}
	}

	void TileLayer::LoadTiles(std::string path)
	{
		std::ifstream file1("Assets/Config/TileMap/TileMap.json"); 
		std::ifstream file2("Assets/Config/TileMap/TileSet.json");
		json MapConfig = json::parse(file1);  
		json SetConfig = json::parse(file2);  

		Dim maxHeight	= MapConfig["layers"][m_Id]["height"].get<Dim>();
		Dim maxWidth	= MapConfig["layers"][m_Id]["width"].get<Dim>();
		m_Totalrows		= maxHeight;
		m_Totalcolumns	= maxWidth; 
		m_Tileset.LoadBMP(path);
	
		Allocate();    
		m_grid = new GridLayer((m_Totalrows * GRID_BLOCK_ROWS), (m_Totalcolumns * GRID_BLOCK_COLUMNS));  

		byte assumtedEmpty;

		for (int row = 0; row < m_Totalrows; row++) 
		{
			for (int col = 0; col < m_Totalcolumns; col++)
			{ 
				unsigned id = MapConfig["layers"][m_Id]["data"][row * m_Totalcolumns + col].get<unsigned>();
				if (id == 0)
				{ 
					SetTile(col, row, EMTY_TILE); 
					assumtedEmpty = 1;
				} 
				else
				{
					Dim r = SetConfig["tiles"][std::to_string(id)]["y"].get<Dim>();
					Dim c = SetConfig["tiles"][std::to_string(id)]["x"].get<Dim>(); 
					assumtedEmpty = SetConfig["tiles"][std::to_string(id)]["empty"].get<byte>();
					SetTile(col, row, MakeIndex(r, c)); 
				} 

				PutTile(
					m_DpyBuffer,
					MUL_TILE_WIDTH(col),
					MUL_TILE_HEIGHT(row),
					m_Tileset,
					GetTile(col, row)
				); 

				m_grid->ComputeTileGridBlock(this, row, col, m_Totalcolumns, 8, assumtedEmpty);
			}
		} 

		for (int row = 0; row < m_Totalrows; row++)
		{
			for (int col = 0; col < m_Totalcolumns; col++)
			{
				printGrid((int*)m_grid->GetGridTileBlock(col, row, m_Totalcolumns)); 
				std::cout << std::endl;
			}
		}

	}

	TileLayer::TileLayer(uint32_t id) 
		: m_Id(id)
	{
		BlankTile.Generate(TILE_WIDTH, TILE_HEIGHT);

		m_ViewWindow.x = 0	* TILE_WIDTH;
		m_ViewWindow.y = 0	* TILE_HEIGHT;
		m_ViewWindow.w = 24 * TILE_WIDTH;
		m_ViewWindow.h = 17 * TILE_HEIGHT; 
	}

	TileLayer::~TileLayer()
	{
		free(m_Map);
	}

	void TileLayer::Allocate()
	{ 
		ENGINE_CORE_ASSERT(m_Totalrows && m_Totalcolumns);
		m_Map = (Index*)malloc((m_Totalcolumns * m_Totalrows) * sizeof(Index));  

		m_DpyBuffer.Generate(
				(m_Totalcolumns * TILE_WIDTH) + 2 * TILE_WIDTH,
				(m_Totalrows * TILE_HEIGHT) + 2 * TILE_HEIGHT
			);
	} 

	void TileLayer::SetTile(Dim col, Dim row, Index index)
	{ 
		m_Map[row * m_Totalcolumns + col] = index;
	} 

	Index TileLayer::GetTile(Dim col, Dim row) const
	{ 
		ENGINE_CORE_ASSERT((col * row) < (m_Totalcolumns * m_Totalrows));
		return m_Map[row * m_Totalcolumns + col];
	}

	const Point TileLayer::Pick(Dim x, Dim y) const
	{ 
		Point p
		{
			DIV_TILE_WIDTH(x + m_ViewWindow.x),
			DIV_TILE_HEIGHT(y + m_ViewWindow.y)
		}; 

		return p;
	} 

	const Rect& TileLayer::GetViewWindow(void)
	{ 
		return m_ViewWindow;
	} 

	void TileLayer::SetViewWindow(const Rect& r)
	{ 
		m_ViewWindow = r; 
		m_DpyChanged = true;
	} 

	void TileLayer::Display(Bitmap& dest, const Rect& displayArea)
	{ 
		if (m_ViewPosCached.x != m_ViewWindow.x || m_ViewPosCached.y != m_ViewWindow.y)
		{
			m_DpyX = m_ViewWindow.x; 
			m_DpyY = m_ViewWindow.y;
			m_ViewPosCached.x = m_ViewWindow.x;
			m_ViewPosCached.y = m_ViewWindow.y;
		} 

		Rect dpySrc{ m_DpyX, m_DpyY, m_ViewWindow.w, m_ViewWindow.h };
		Rect dpyDest{ displayArea.x, displayArea.y, displayArea.w, displayArea.h };
		Bitmap::Blit (
			m_DpyBuffer, &dpySrc,
			dest, &dpyDest
		);

		return;
	} 

	Bitmap TileLayer::GetBitmap(void) const
	{
		return m_DpyBuffer;
	}

	int TileLayer::GetPixelWidth(void) const
	{
		return m_ViewWindow.w;
	}

	int TileLayer::GetPixelHeight(void) const
	{
		return m_ViewWindow.h;
	}

	unsigned TileLayer::GetTileWidth(void) const
	{
		return DIV_TILE_WIDTH(m_ViewWindow.w);
	}

	unsigned TileLayer::GetTileHeight(void) const
	{
		return DIV_TILE_HEIGHT(m_ViewWindow.h);
	}

	void TileLayer::Scroll(float dx, float dy)
	{ 
		m_ViewWindow.x += dx;
		m_ViewWindow.y += dy;
	}

	void TileLayer::FilterScrollDistance(uint32_t viewStartCoord, uint32_t viewSize, uint32_t* d, uint32_t maxMapSize)
	{ 
		auto val = *d + viewStartCoord; 
		if (val < 0)
			*d = viewStartCoord; 

		else if (viewSize >= maxMapSize)
			*d = 0;  

		else if ((val + viewSize) >= maxMapSize)
			*d = maxMapSize - (viewStartCoord + viewSize);
	}

	void TileLayer::FilterScroll(uint32_t* dx, uint32_t* dy)
	{ 
		FilterScrollDistance(m_ViewWindow.x, m_ViewWindow.w, dx, GetPixelWidth());
		FilterScrollDistance(m_ViewWindow.y, m_ViewWindow.h, dy, GetPixelHeight());
	}

	void TileLayer::ScrollWithBoundsCheck(uint32_t _dx, uint32_t _dy)
	{ 
		FilterScroll(&_dx, &_dy); 
		Scroll(_dx, _dy);
	}

	bool TileLayer::CanScrollHoriz(float dx) const
	{
		return (m_ViewWindow.x >= -dx) && (m_ViewWindow.x + m_ViewWindow.w + dx) <= (m_Totalcolumns * TILE_WIDTH);
	}

	bool TileLayer::CanScrollVert(float dy) const
	{
		return (m_ViewWindow.y >= -dy) && (m_ViewWindow.y + m_ViewWindow.h + dy) <= (m_Totalrows * TILE_HEIGHT);
	}

}

