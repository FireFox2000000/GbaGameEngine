#pragma once
#include "Array.h"

template<class T>
class DoubleBuffer
{
	Array<T, 2> m_doubleBuffer;
	int m_currentBufferIndex = 0;

public:
	void Flip() 
	{
		++m_currentBufferIndex;
		if (m_currentBufferIndex > 1)
			m_currentBufferIndex = 0;
	}

	T& GetPrimary() 
	{
		return m_doubleBuffer[m_currentBufferIndex];
	}

	T& GetSecondary() 
	{
		int previousBufferIndex = m_currentBufferIndex + 1;
		if (previousBufferIndex > 1)
			previousBufferIndex = 0;

		return m_doubleBuffer[previousBufferIndex];
	}
};
