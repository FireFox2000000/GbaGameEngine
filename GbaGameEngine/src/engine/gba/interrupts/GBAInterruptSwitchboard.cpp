#include "GBAInterruptSwitchboard.h"
#include "GBASDK/GBA.h"

static constexpr int MAX_HANDLERS_PER_INTERRUPT = 1;
static Array<FixedList<GBA::InterruptSwitchboard::InterruptHandler, MAX_HANDLERS_PER_INTERRUPT>, GBA::Interrupts::Count> m_interruptHandlers;

void GBA::InterruptSwitchboard::RegisterHandler(GBA::Interrupts::InterruptType interruptType, GBA::InterruptSwitchboard::InterruptHandler handler)
{
	m_interruptHandlers[interruptType].Add(handler);
}

GBA_ARM_IWRAM_CODE void HandleInterrupts()
{
	u16 interrupts = GBA::Interrupts::GetActiveInterrupts();

	if (GBA::Interrupts::IsInterruptActive(GBA::Interrupts::VBlank))
	{
		for (auto& handler : m_interruptHandlers[GBA::Interrupts::VBlank])
		{
			handler();
		}
	}

	GBA::Interrupts::AcknowledgeInterrupts(interrupts);
}

void GBA::InterruptSwitchboard::Init()
{
	GBA::Interrupts::DisableInterrupts();
	GBA::Interrupts::RegisterInterruptHandler(HandleInterrupts);
}
