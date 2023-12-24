#pragma once
struct note
{
	int id;		// Position in scale
	FTYPE on;	// Time note was activated
	FTYPE off;	// Time note was deactivated
	bool active;
	int channel;

	note()
	{
		id = 0;
		on = 0.0;
		off = 0.0;
		active = false;
		channel = 0;
	}
};