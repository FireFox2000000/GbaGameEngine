#include "GBAGraphics.h"

namespace GBA
{
	void Graphics::EndFrame()
	{
		m_oamManager.DoMasterRenderIntoMemory();
	}
}
