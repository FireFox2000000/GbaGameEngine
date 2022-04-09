#pragma once
#include "engine/gba/graphics/GBAGraphics.h"
#include "engine/base/core/stl/List.h"
#include <memory>

class Engine;
class IPostProcessingGfxTask;

class Graphics : public GBA::Graphics 
{
	static constexpr int MAX_CONCURRENT_GFX_TASKS = 1;
	FixedList<std::shared_ptr<IPostProcessingGfxTask>, MAX_CONCURRENT_GFX_TASKS> m_currentGfxTasks;

	void DeleteCompletedGfxTasks();

public:
	bool KickPostProcessingGfxTask(std::shared_ptr<IPostProcessingGfxTask> task)
	{
		if (m_currentGfxTasks.Count() < m_currentGfxTasks.Capacity())
		{
			m_currentGfxTasks.Add(task);

			return true;
		}

		DEBUG_ASSERTMSG(false, "Unable to kick post-processing gfx task as it is out of capacity.");

		return false;
	}

	void Update(Engine* engine);
	void LateRender(Engine* engine);
};
