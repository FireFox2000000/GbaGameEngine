#include "AudioChannelProperties.h"

bool AudioChannelProperties::IsValidAttributeValue(AudioChannelProperties::Attributes attribute, float value)
{
	switch (attribute)
	{
	case Attributes::Playrate:
		return value > 0.0f;

	case Attributes::Volume:
		return value > 0.0f;

	default: break;
	}
	return true;
}
