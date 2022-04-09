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

void Graphics::Update()
{
	DeleteCompletedGfxTasks();

	for (auto& task : m_currentGfxTasks)
	{
		task->Update();
	}
}

void Graphics::LateRender()
{
	for (auto& task : m_currentGfxTasks)
	{
		task->LateRender();
	}
}
