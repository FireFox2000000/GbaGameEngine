#pragma once

namespace AudioChannelProperties
{
	enum Flags
	{
		Active,		// Read only please
		DisposeOnCompletion,
		Loop,
	};

	enum Attributes
	{
		Playrate,	// Also known as tempo
		Volume,

		AttributesCount
	};

	bool IsValidAttributeValue(AudioChannelProperties::Attributes attribute, float value);
}
