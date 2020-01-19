#include "KokiriRespawn.h"
#include "RespawnMesh.h"
#include "engine/engine/engine.h"
#include "game/scripts/componentsystems/PlayerComponent.h"
#include "engine/gameobject/transformation/Transform.h"

void System::KokiriRespawn::Update(Engine* engine)
{
	auto* entityManager = engine->GetEntityRegistry();
	auto view = entityManager->View<Component::Player, Component::Transform>();
	for (auto entity : view)
	{
		Component::Transform& playerTransform = *entityManager->EditComponent<Component::Transform>(entity);
		const auto playerPos = playerTransform.GetPosition();

		entityManager->InvokeEach<RespawnMesh, Component::Transform>(
			[&playerPos, &playerTransform]
		(const RespawnMesh& respawnMesh, const Component::Transform& respawnTransform)
			{
				auto respawnPos = respawnTransform.GetPosition();
				auto playerRelativePos = playerPos - respawnPos;

				if (respawnMesh.GetMesh().GetShape(playerRelativePos.x.ToInt(), playerRelativePos.y.ToInt()) != tRespawnMesh::None)
				{
					// Do respawn
					int scaleSize = respawnMesh.GetMesh().GetGridBoxSize();
					int respawnGridX = respawnMesh.m_respawnX * scaleSize;
					int respawnGridY = respawnMesh.m_respawnY * scaleSize;

					auto playerRespawnGridPos = respawnPos + Vector2<int>(respawnGridX, respawnGridY);	// Bottom left of the actual grid

					auto playerOffsetX = playerRelativePos.x;
					{
						int offsetFactor = scaleSize;
						if (playerOffsetX >= tFixedPoint8(scaleSize))
						{
							offsetFactor *= -1;
						}

						while (playerOffsetX < tFixedPoint8(0) || playerOffsetX >= tFixedPoint8(scaleSize))
						{
							playerOffsetX += offsetFactor;
						}
					}

					auto playerOffsetY = playerRelativePos.y;
					{
						int offsetFactor = scaleSize;
						if (playerOffsetY >= tFixedPoint8(scaleSize))
						{
							offsetFactor *= -1;
						}

						while (playerOffsetY < tFixedPoint8(0) || playerOffsetY >= tFixedPoint8(scaleSize))
						{
							playerOffsetY += offsetFactor;
						}
					}

					playerRespawnGridPos += Vector2<tFixedPoint8>(playerOffsetX, playerOffsetY);

					playerTransform.SetPosition(playerRespawnGridPos);
					DEBUG_LOGFORMAT("Player respawned to position (%f, %f)", playerRespawnGridPos.x.ToFloat(), playerRespawnGridPos.y.ToFloat());
				}
			});
	}
}
