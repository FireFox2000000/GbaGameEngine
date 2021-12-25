#pragma once
#include "engine/base/core/stl/Array.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"
#include "engine/math/Vector2.h"

namespace GBA
{
	namespace Gfx
	{
		class Tilemap;
		class TilemapSet;

		class TilemapManager
		{
			Array<u8, GBA::BackgroundControl::Backgrounds::Count> m_tilesetRefCounter;

			void Load(Tilemap& out_tilemap, u32 tilesToAlloc, GBA::Gfx::Background::ControlRegister::Size size, bool isAffine, bool copyMapDirectlyToMemory);

			// Loads tileset into memory and allocates space for the background. Actual map will load in chunks. 
			void LoadStaticMap(Tilemap& out_tilemap);

			// Loads a variable sized tilemap, reserving a space of 32x32 tiles to display on the screen. Does not load tiles in directly, gives responsibility of the chunk loading to the renderer.
			void LoadDynamicMap(Tilemap& out_tilemap);

		public:
			static const Vector2<u8> VARIABLE_TILEMAP_SIZE;

			TilemapManager();
			~TilemapManager();

			void LoadTilemap(Tilemap& out_tilemap);

			// Loads a fixed size tilemap.
			//void Load(Tilemap& out_tilemap, GBA::Gfx::Background::ControlRegister::Size size, bool isAffine = false);

			void Unload(Tilemap* tilemap);
			void Clear();

			static void LoadPalette(TilemapSet* tilemapSet);
			static void LoadTileset(TilemapSet* tilemapSet);
		};
	}
}
