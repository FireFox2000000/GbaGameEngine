#pragma once
#include "engine/base/core/stl/Array.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

class Tilemap;

class TilemapManager
{
	Array<u8, GBA::BackgroundControl::Backgrounds::Count> m_tilesetRefCounter;

public:
	TilemapManager();
	~TilemapManager();

	void Load(Tilemap& out_tilemap);
	void Unload(Tilemap* tilemap);
	void Clear();
};
