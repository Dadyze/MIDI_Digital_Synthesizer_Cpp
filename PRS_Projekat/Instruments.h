#pragma once
#include "Envelope.h"
#include "note.h"
#include "Utilities.h"
struct instrument_base
{
	FTYPE dVolume;
	envelope_adsr env;
	virtual FTYPE sound(const FTYPE dTime, note n, bool& bNoteFinished) = 0;
};

struct instrument_test : public instrument_base
{
	instrument_test()
	{
		env.dAttackTime = 0.05;
		env.dDecayTime = 1.0;
		env.dSustainAmplitude = 0.95;
		env.dReleaseTime = 0.1;

		dVolume = 1.0;
	}

	 virtual FTYPE sound(const FTYPE dTime, note n, bool& bNoteFinished) 
	{
		FTYPE dAmplitude = envw(dTime, env, n.on, n.off);
		if (dAmplitude <= 0.0) bNoteFinished = true;

		FTYPE dSound =
			+ 1.00 * osc(n.on - dTime, scale(n.id), OSC_SQUARE, 5.0, 0.001)
			+ 0.50 * osc(n.on - dTime ,scale(n.id + 12), OSC_SQUARE)
			+ 0.05 * osc(n.on - dTime, scale(n.id + 24), OSC_NOISE);

		return dAmplitude * dSound * dVolume;
	}

};
