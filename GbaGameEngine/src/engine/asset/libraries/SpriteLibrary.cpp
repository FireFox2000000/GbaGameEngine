#include "SpriteLibrary.h"
#include "engine/math/Math.h"
#include "engine/filestream/CppFileReader.h"

#include "game/data/Shantae_Idle_bin.h"

SpriteLibrary::SpriteLibrary()
{
	DEBUG_LOG("Setting up Sprite Library...")

	u32 totalBytes = 0;

	totalBytes += AddSpriteSheetFromFile(Shantae_Idle_bin::data);

	DEBUG_LOGFORMAT("Sprite Library total sprite memory = %.2fkb", BYTES_TO_KB(totalBytes));
}

int SpriteLibrary::AddSpriteSheetFromFile(const u32 * file)
{
	CppFileReader reader = CppFileReader(file);

	const u32 spriteCount = reader.Read<u32>();
	const u8 paletteLength = reader.Read<u8>();
	const u32 dataLength = reader.Read<u32>();
	const u32 compressionFlags = reader.Read<u32>();
	const u16 * palette = reader.ReadAddress<u16>(paletteLength);
	const u8 * widthMap = reader.ReadAddress<u8>(spriteCount);
	const u8 * heightMap = reader.ReadAddress<u8>(spriteCount);
	const u32 * offsets = reader.ReadAddress<u32>(spriteCount);
	const u32 * data = reader.ReadAddress<u32>(dataLength);

	AddSpriteSheet(spriteCount, paletteLength, palette, widthMap, heightMap, dataLength, compressionFlags, data, offsets);

	return dataLength;
}

void SpriteLibrary::AddSpriteSheet(
	const u32 spriteCount, 
	const u8 paletteLength, 
	const u16 * palette, 
	const u8 * widthMap, 
	const u8 * heightMap, 
	const u32 dataLength, 
	const u32 compressionFlags,
	const u32 * data,
	const u32 * offsets)
{
	m_spriteAtlusCollection.AddNew(spriteCount, paletteLength, palette, widthMap, heightMap, dataLength, compressionFlags, data, offsets);
}

SpriteLibrary::~SpriteLibrary()
{
}

Sprite * SpriteLibrary::GetSprite(SpriteAtlusID::Enum atlusId, u32 spriteIndex)
{
	SpriteAtlus& atlus = m_spriteAtlusCollection[atlusId];
	Sprite* sprite = atlus.GetSprite(spriteIndex);

	DEBUG_ASSERTMSGFORMAT(sprite, "Unable to get sprite for atlus %d at index %d", atlusId, spriteIndex);

	return sprite;
}
