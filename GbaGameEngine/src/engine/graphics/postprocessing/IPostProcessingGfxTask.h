#pragma once

class Engine;

class IPostProcessingGfxTask
{
public:
	virtual ~IPostProcessingGfxTask() = default;

	// Main update loop. Pre-prepare any calcs here.
	virtual void Update() = 0;

	// Called after main render
	virtual void LateRender() = 0;

	/// <summary>
	/// When this return true, the task will be removed and deleted.
	/// </summary>
	/// <returns></returns>
	virtual bool IsComplete() = 0;

	/// <summary>
	/// Stop/force the task to complete.
	/// </summary>
	virtual void Abort() = 0;
};
