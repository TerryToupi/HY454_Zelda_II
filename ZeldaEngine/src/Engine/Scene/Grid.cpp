#include "Grid.h"

#include <Engine/Scene/Tiles.h>

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

#define		GRID_BLOCK_SIZEOF							(GRID_ELEMENTS_PER_TILE * sizeof(GridIndex)) 
#define		SetGridTileBlockEmpty(col, row, cols)		SetGridTileBlock(col, row, cols, GRID_EMPTY_TILE) 
#define		SetGridTileBlockSolid(col, row, cols)		SetGridTileBlock(col, row, cols, GRID_SOLID_TILE)

namespace Engine
{
	GridLayer::GridLayer(unsigned rows, unsigned cols)
		: m_totalRows(rows), m_totalColumns(cols)
	{
		Allocate();
	}

	GridLayer::~GridLayer()
	{ 
		free(m_grid);
	}

	void GridLayer::FilterGridMotion(Rect& r, int* dx, int* dy)
	{ 
		// try horizontal move
		if (*dx < 0)
			FilterGridMotionLeft(r, dx);
		else if (*dx > 0)
			FilterGridMotionRight(r, dx);
		// try vertical move
		if (*dy < 0)
			FilterGridMotionUp(r, dy);
		else if (*dy > 0)
			FilterGridMotionDown(r, dy);
	}

	void GridLayer::Allocate(void)
	{
		m_grid = (GridIndex *)malloc((m_total = m_totalRows * m_totalColumns) * sizeof(GridIndex));
		memset(m_grid, GRID_EMPTY_TILE, m_total);
	} 

	void GridLayer::FilterGridMotionDown(Rect& r, int* dy)
	{
	}

	void GridLayer::FilterGridMotionLeft(Rect& r, int* dx)
	{
		auto x1_next = r.x + *dx;
		if (x1_next < 0)
			*dx = -r.x;
		else {
			auto newCol = DIV_GRID_ELEMENT_WIDTH(x1_next);
			auto currCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			if (newCol != currCol) {
				assert(newCol + 1 == currCol); // we really move left
				auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
				auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
				for (auto row = startRow; row <= endRow; ++row)
					if (!CanPassGridTile(newCol, row, GRID_RIGHT_SOLID_MASK)) {
						*dx = MUL_GRID_ELEMENT_WIDTH(currCol) - r.x;
						break;
					}
			}
		}
	}

	void GridLayer::FilterGridMotionRight(Rect& r, int* dx)
	{ 
		auto x2 = r.x + r.w - 1;
		auto x2_next = x2 + *dx;
		if (x2_next >= m_totalColumns * GRID_BLOCK_COLUMNS)
			*dx = ((m_totalColumns * GRID_BLOCK_COLUMNS) - 1) - x2;
		else {
			auto newCol = DIV_GRID_ELEMENT_WIDTH(x2_next);
			auto currCol = DIV_GRID_ELEMENT_WIDTH(x2);
			if (newCol != currCol) {
				assert(newCol - 1 == currCol); // we really move right
				auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
				auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
				for (auto row = startRow; row <= endRow; ++row)
					if (!CanPassGridTile(newCol, row, GRID_LEFT_SOLID_MASK)) {
						*dx = (MUL_GRID_ELEMENT_WIDTH(newCol) - 1) - x2;
						break;
					}
			}
		}
	}

	void GridLayer::FilterGridMotionUp(Rect& r, int* dx)
	{
	}

	GridIndex* GridLayer::GetGridTileBlock(Dim colTile, Dim rowTile, Dim tileCols)
	{
		return m_grid + (rowTile * tileCols + colTile) * GRID_BLOCK_SIZEOF;
	}

	void GridLayer::SetGridTileBlock(Dim colTile, Dim rowTile, Dim tileCols, GridIndex flags)
	{
		memset(
			GetGridTileBlock(colTile, rowTile, tileCols), 
			flags, 
			GRID_BLOCK_SIZEOF
		);
	}

	void GridLayer::SetGridTile(Dim col, Dim row, GridIndex index)
	{ 
		m_grid[row * m_totalColumns + col] = index;
	} 

	GridIndex GridLayer::GetGridTile(Dim col, Dim row)
	{
		return m_grid[row * m_totalColumns + col];
	} 

	void GridLayer::SetSolidGridTile(Dim col, Dim row)
	{
		SetGridTile(col, row, GRID_SOLID_TILE);
	}

	void GridLayer::SetEmptyGridTile(Dim col, Dim row)
	{
		SetGridTile(col, row, GRID_EMPTY_TILE);
	}

	void GridLayer::SetGridTileFlags(Dim col, Dim row, GridIndex flags)
	{
		SetGridTile(col, row, flags);
	}

	void GridLayer::SetGridTileTopSolidOnly(Dim col, Dim row)
	{
		SetGridTileFlags(row, col, GRID_TOP_SOLID_MASK);
	}

	bool GridLayer::CanPassGridTile(Dim col, Dim row, GridIndex flags)
	{
		return (GetGridTile(row, col) & flags) != 0;
	}

	void GridLayer::ComputeTileGridBlock(TileLayer* tlayer, Dim row, Dim col, Dim tileCols, byte solidThreshold ,bool assumtedEmpty)
	{
		Bitmap tileElement;
		tileElement.Generate(TILE_WIDTH, TILE_HEIGHT); 
		Bitmap gridElement; 
		gridElement.Generate(GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT); 
		
		auto index = tlayer->GetTile(col, row);
		tlayer->PutTile(tileElement, 0, 0, tlayer->m_Tileset, index);
		if (assumtedEmpty)
			SetGridTileBlockEmpty(col, row, tileCols, m_grid);
		else
			ComputeGridBlock(
				GetGridTileBlock(col, row, tileCols),
				tileElement, 
				gridElement,
				tlayer->m_Tileset,
				solidThreshold 
			); 
	}

	void GridLayer::ComputeGridBlock(GridIndex* block, Bitmap& tileElem, Bitmap& gridElem, Bitmap& tileSet, byte solidThreshold)
	{
		for (auto i = 0; i < GRID_ELEMENTS_PER_TILE; ++i)
		{
			auto x = i % GRID_BLOCK_ROWS; 
			auto y = i / GRID_BLOCK_COLUMNS;

			Rect tl = { x * GRID_ELEMENT_WIDTH, y * GRID_ELEMENT_HEIGHT, GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT }; 
			Rect gl = { 0, 0, GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT };

			Bitmap::Blit(
				tileElem, 
				&tl, 
				gridElem, 
				&gl
			); 

			auto isEmpty = ComputeIsGridIndexEmpty(gridElem, solidThreshold); 
			*block++ = isEmpty ? GRID_EMPTY_TILE : GRID_SOLID_TILE;
		}
	} 

	bool GridLayer::ComputeIsGridIndexEmpty(Bitmap& gridElem, byte solidThreshold)
	{
		auto n = 0; 
		Bitmap::BitmapAccessPixels(
			gridElem,
			[&n](Bitmap& bmp, Bitmap::PixelMemory mem)
			{
				auto c = Bitmap::GetPixel32(bmp, mem);
				if (c != TRANSPARENT)
					++n;
			}
		); 

		return n <= solidThreshold; 
	}
}