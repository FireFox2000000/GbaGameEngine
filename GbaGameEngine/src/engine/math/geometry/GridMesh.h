#pragma once
#include "engine/base/core/stl/Array.h"
#include "engine/math/Math.h"

template<int SIZE_X, int SIZE_Y>
class GridMesh
{
public:
	enum FillShape
	{
		None,
		Box,

		Count
	};

	using tMeshRow = Array<FillShape, SIZE_X>;
	using tMesh = Array<tMeshRow, SIZE_Y>;

private:
	tMesh m_mesh;
	bool m_enableWrapping = false;
	int m_gridSize = 1;

public:
	GridMesh(const tMesh& mesh, bool enableWrapping, int gridSize)
		: m_mesh(mesh)
		, m_enableWrapping(enableWrapping)
		, m_gridSize(gridSize)
	{}

	int GetGridBoxSize() const { return m_gridSize; }

	static void WrapPosition(int& out_x, int& out_y)
	{
		while (out_x < 0)
		{
			out_x += SIZE_X;
		}

		while (out_y < 0)
		{
			out_y += SIZE_Y;
		}

		out_x %= SIZE_X;
		out_y %= SIZE_Y;
	}

	FillShape GetShape(int x, int y) const 
	{ 
		x /= m_gridSize;
		y /= m_gridSize;

		if (m_enableWrapping)
		{
			WrapPosition(x, y);
		}
		else
		{
			y = Math::Clamp(y, 0, SIZE_Y);
			x = Math::Clamp(x, 0, SIZE_X);
		}

		return m_mesh[y][x];
	}
};
