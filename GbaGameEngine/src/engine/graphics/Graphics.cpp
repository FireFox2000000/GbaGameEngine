#include "Graphics.h"
#include "engine/graphics/postprocessing/IPostProcessingGfxTask.h"

void Graphics::DeleteCompletedGfxTasks()
{
	for (int i = m_currentGfxTasks.Count() - 1; i >= 0; --i)
	{
		auto& task = m_currentGfxTasks[i];
		if (task->IsComplete())
		{
			m_currentGfxTasks.RemoveAt(i);
		}
	}
}

void Graphics::Update(Engine* engine)
{
	DeleteCompletedGfxTasks();

	for (auto& task : m_currentGfxTasks)
	{
		task->Update(engine);
	}
}

void Graphics::LateRender(Engine* engine)
{
	for (auto& task : m_currentGfxTasks)
	{
		task->LateRender(engine);
	}
}
