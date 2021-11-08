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
		m_currentBufferIndex ^= 1;
	}

	inline T& GetPrimary() 
	{
		return m_doubleBuffer[m_currentBufferIndex];
	}

	inline T& GetSecondary() 
	{
		return m_doubleBuffer[m_currentBufferIndex ^ 1];
	}
};
