#pragma once
#include "engine/math/geometry/GridMesh.h"

static const int RESPAWN_MESH_WIDTH = 5;
static const int RESPAWN_MESH_HEIGHT = 5;
using tRespawnMesh = GridMesh<RESPAWN_MESH_WIDTH, RESPAWN_MESH_HEIGHT>;

class RespawnMesh
{
	tRespawnMesh m_mesh;

public:
	int m_respawnX = 0;
	int m_respawnY = 0;

	RespawnMesh(const tRespawnMesh& mesh, int respawnX, int respawnY);

	const tRespawnMesh& GetMesh() const { return m_mesh; }
};
