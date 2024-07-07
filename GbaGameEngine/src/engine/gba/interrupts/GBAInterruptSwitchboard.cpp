#include "GBAInterruptSwitchboard.h"
#include "gbatek/GBA.h"
#include "gbatek/Interrupts.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"

//static constexpr int MAX_HANDLERS_PER_INTERRUPT = 1;
//static Array<FixedList<GBA::InterruptSwitchboard::InterruptHandler, MAX_HANDLERS_PER_INTERRUPT>, GBA::Interrupts::Count> m_interruptHandlers;

//void GBA::InterruptSwitchboard::RegisterHandler(const Interrupts& interrupts, GBA::InterruptSwitchboard::InterruptHandler handler)
//{
//	m_interruptHandlers[interruptType].Add(handler);
//}

GBATEK_ARM_IWRAM_CODE void HandleInterrupts()
{
	GBATEK::InterruptFlags activeInterrupts = *GBATEK::ioRegisterInterruptEnable & GBATEK::ioRegisterInterruptRequestFlags->interrupts;

	if ((activeInterrupts & GBATEK::InterruptFlags::VBlank) != GBATEK::InterruptFlags::None)
	{
		//for (auto& handler : m_interruptHandlers[GBA::Interrupts::VBlank])
		//{
		//	handler();
		//}
	}

	GBATEK::ioRegisterInterruptRequestFlags->AcknowledgeInterrupts(activeInterrupts);

	// Acknowledge bios as we're using VBlankInterruptWait
	GBATEK::ioRegisterBiosInterruptFlags->AcknowledgeInterrupts(activeInterrupts);
}

void GBA::InterruptSwitchboard::Init()
{
	*GBATEK::ioRegisterInterruptHandler = HandleInterrupts;
}
