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
		env.dAttackTime = 0.01;
		env.dDecayTime = 1.0;
		env.dSustainAmplitude = 0.0;
		env.dReleaseTime = 1.0;

		dVolume = 1.0;
	}

	 virtual FTYPE sound(const FTYPE dTime, note n, bool& bNoteFinished) 
	{
		FTYPE dAmplitude = envw(dTime, env, n.on, n.off);
		if (dAmplitude <= 0.0) bNoteFinished = true;

		FTYPE dSound =
			+ 2.00 * osc(n.on - dTime, scale(n.id + 12),OSC_SINE, 5.0, 0.001)
			+ 2.20 * osc(n.on - dTime, scale(n.id + 24)) + 
			+ 1.25 * osc(n.on - dTime, scale(n.id + 36));

		return dAmplitude * dSound * dVolume;
	}

};
