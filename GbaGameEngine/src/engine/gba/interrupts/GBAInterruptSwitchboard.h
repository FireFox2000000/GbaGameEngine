#pragma once
#include <functional>

namespace GBA
{
	//struct Interrupts;

	namespace InterruptSwitchboard
	{
		using InterruptHandler = std::function<void()>;

		void Init();
		// void RegisterHandler(const Interrupts& interrupts, InterruptHandler handler);
	};
}
