#include "Tiles.h"   

#include <Engine/Renderer/Render.h> 
#include <json.hpp>  

#include <iostream>
#include <fstream>

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

using json = nlohmann::json;

namespace Engine
{  
	Bitmap BlankTile; 

	Index MakeIndex(byte row, byte col)
	{
		return (MUL_TILE_WIDTH(col) << TILEX_SHIFT) | MUL_TILE_HEIGHT(row);  
	} 

	Dim TileX(Index index) 
	{   
		return index >> TILEX_SHIFT;
	} 

	Dim TileY(Index index)
	{
		return index & TILEY_MASK;
	} 

	void PutTile(Bitmap& dest, Dim x, Dim y, Bitmap& tiles, Index tile)
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
		BlankTile.Generate(TILE_WIDTH, TILE_HEIGHT);

		for (int row = 0; row < m_Totalrows; row++) 
		{
			for (int col = 0; col < m_Totalcolumns; col++)
			{ 
				unsigned id = MapConfig["layers"][m_Id]["data"][row * m_Totalcolumns + col].get<unsigned>();
				if (id == 0)
				{ 
					SetTile(col, row, EMTY_TILE);
				} 
				else
				{
					Dim r = SetConfig["tiles"][std::to_string(id)]["y"].get<Dim>();
					Dim c = SetConfig["tiles"][std::to_string(id)]["x"].get<Dim>(); 
					SetTile(col, row, MakeIndex(r, c));
				}

				PutTile(
					m_DpyBuffer,
					MUL_TILE_WIDTH(col),
					MUL_TILE_HEIGHT(row),
					m_Tileset,
					GetTile(col, row)
				);
			}
		} 
	}

	TileLayer::TileLayer(uint32_t id) 
		: m_Id(id)
	{
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
			//auto startCol = DIV_TILE_WIDTH(m_ViewWindow.x);
			//auto startRow = DIV_TILE_HEIGHT(m_ViewWindow.y);
			//auto endCol = DIV_TILE_WIDTH(m_ViewWindow.x + m_ViewWindow.w - 1);
			//auto endRow = DIV_TILE_HEIGHT(m_ViewWindow.y + m_ViewWindow.h - 1);
			//m_DpyX = MOD_TILE_WIDTH(m_ViewWindow.x); 
			//m_DpyY = MOD_TILE_WIDTH(m_ViewWindow.y); 
			m_DpyX = m_ViewWindow.x; 
			m_DpyY = m_ViewWindow.y;
			m_ViewPosCached.x = m_ViewWindow.x;
			m_ViewPosCached.y = m_ViewWindow.y;

			//for (Dim row = startRow; row <= endRow; ++row)
			//{
			//	for (Dim col = startCol; col <= endCol; ++col)
			//	{
			//		PutTile(
			//			m_DpyBuffer,
			//			MUL_TILE_WIDTH(col - startCol),
			//			MUL_TILE_HEIGHT(row - startRow),
			//			m_Tileset,
			//			GetTile(col, row)
			//		);
			//	}
			//}

			//Rect dpySrc{ m_DpyX, m_DpyY, m_ViewWindow.w, m_ViewWindow.h };
			//Rect dpyDest{ displayArea.x, displayArea.y, displayArea.w, displayArea.h };
			//Bitmap::Blit (
			//	m_DpyBuffer, &dpySrc,
			//	dest, &dpyDest
			//);
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

	bool TileLayer::CanScrollHoriz(float dx) const
	{
		return (m_ViewWindow.x >= -dx) && (m_ViewWindow.x + m_ViewWindow.w + dx) <= (m_Totalcolumns * TILE_WIDTH);
	}

	bool TileLayer::CanScrollVert(float dy) const
	{
		return (m_ViewWindow.y >= -dy) && (m_ViewWindow.y + m_ViewWindow.h + dy) <= (m_Totalrows * TILE_HEIGHT);
	}

}

