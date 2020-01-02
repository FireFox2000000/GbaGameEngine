#include "GBABgVramAllocator.h"

namespace GBA
{
	BgVramAllocator::BgVramAllocator()
		: m_screenEntryTracker(SEAllocationState::Free)
	{
	}

	void BgVramAllocator::Alloc(u32 totalBytes, Background::tCharacterBaseBlock & out_cbbIndex, Background::tScreenBaseBlockIndex & out_sbbIndex)
	{
		// TODO- Implement me
	}

	void BgVramAllocator::Free(Background::tScreenBaseBlockIndex sbbIndex)
	{
		while (sbbIndex < m_screenEntryTracker.Count() && m_screenEntryTracker[sbbIndex] != SEAllocationState::Free)
		{
			m_screenEntryTracker[sbbIndex++] = SEAllocationState::Free;
		}
	}

	void BgVramAllocator::Clear()
	{
		for (u32 i = 0; i < m_screenEntryTracker.Count(); ++i)
		{
			m_screenEntryTracker[i] = SEAllocationState::Free;
		}
	}
}
