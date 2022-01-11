#pragma once

class Engine;

namespace System
{
	namespace Physics
	{
		void UpdateTransforms(Engine* engine);
		void ResolveCollisions(Engine* engine);
	}
}
