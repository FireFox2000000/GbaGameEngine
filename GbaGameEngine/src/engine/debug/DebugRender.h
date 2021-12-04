#pragma once
#include "engine/asset/FixedAssetManager.h"

class Engine;
class GameObject;

class DebugRender
{
	enum SpriteAtlusID {
		DebugPrimitives,

		SpriteCount
	};

	enum DebugPrimitiveSprites {
		HollowSquare,
		HollowCircle,

		DebugPrimitiveSpritesCount
	};

	enum SpriteAnimationID {
		AnimationCount
	};

	enum TilemapSetID {
		TilemapSetCount
	};

	FixedAssetManager<
		SpriteAtlusID,
		SpriteAnimationID,
		TilemapSetID>
		m_assetManager;

public:
	DebugRender();
	void RenderColliders(Engine* engine, GameObject* camera);
};
