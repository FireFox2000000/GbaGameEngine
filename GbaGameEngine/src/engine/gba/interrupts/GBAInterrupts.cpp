#include "GBAInterrupts.h"
#include "engine/base/core/stl/Bitmask.h"
#include "engine/gba/registers/RegisterMap.h"

// Master interrupt control
#define vu16_REG_IME (*(vu16*)(REG_IME))
#define vu16_REG_IE (*(vu16*)(REG_IE))
#define vu16_REG_IF (*(vu16*)(REG_IF))
#define vu16_REG_IFBIOS (*(vu16*)(REG_IFBIOS))

#define Fnptr_REG_INTERRUPT *(GBA::Interrupts::InterruptHandler*)(REG_INTERRUPT)

void GBA::Interrupts::DisableInterrupts()
{
	vu16_REG_IME = 0x00;
}

void GBA::Interrupts::EnableInterrupts()
{
	vu16_REG_IME = 0x01;
}

void GBA::Interrupts::EnableInterrupt(InterruptType interrupt)
{
	((Bitmask<vu16>*)REG_IE)->SetBit(interrupt);
}

u16 GBA::Interrupts::GetActiveInterrupts()
{
	return vu16_REG_IE * vu16_REG_IF;
}

bool GBA::Interrupts::IsInterruptActive(InterruptType interrupt)
{
	return ((Bitmask<vu16>*)REG_IF)->TestBit(interrupt);
}

bool GBA::Interrupts::IsInterruptEnabled(InterruptType interrupt)
{
	return ((Bitmask<vu16>*)REG_IE)->TestBit(interrupt);;
}

void GBA::Interrupts::AcknowledgeInterrupt(InterruptType interrupt)
{
	// Yes this is weird, to acknowledge the interrupt we need to set the bit that's already been set.
	((Bitmask<vu16>*)REG_IF)->SetBit(interrupt);	
}

void GBA::Interrupts::AcknowledgeInterrupts(u16 interrupts)
{
	// Yes this is weird, to acknowledge the interrupt we need to set the bit that's already been set.
	vu16_REG_IF = interrupts;
	vu16_REG_IFBIOS |= interrupts;
}

void GBA::Interrupts::AcknowledgeInterruptBios(InterruptType interrupt)
{
	// Yes this is weird, to acknowledge the interrupt we need to set the bit that's already been set.
	((Bitmask<vu16>*)REG_IFBIOS)->SetBit(interrupt);
}

void GBA::Interrupts::RegisterInterruptHandler(InterruptHandler armHandler)
{
	Fnptr_REG_INTERRUPT = armHandler;
}
