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

struct instrument_bell : public instrument_base
{
	instrument_bell()
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
			+ 0.20 * osc(n.on - dTime, scale(n.id + 24))
			+ 1.25 * osc(n.on - dTime, scale(n.id + 36));

		return dAmplitude * dSound * dVolume;
	}

};

struct instrument_bell8 : public instrument_base
{
	instrument_bell8()
	{
		env.dAttackTime = 0.01;
		env.dDecayTime = 0.5;
		env.dSustainAmplitude = 0.8;
		env.dReleaseTime = 1.0;

		dVolume = 1.0;
	}

	virtual FTYPE sound(const FTYPE dTime, note n, bool& bNoteFinished) 
	{
		FTYPE dAmplitude = envw(dTime, env, n.on, n.off);
		if (dAmplitude <= 0.0) bNoteFinished = true;

		FTYPE dSound =
			+ 1.00 * osc(n.on - dTime, scale(n.id), OSC_SQUARE, 5.0, 0.001)
			+ 0.50 * osc(n.on - dTime, scale(n.id + 12))
			+ 0.25 * osc(n.on - dTime, scale(n.id + 24));

		return dAmplitude * dSound * dVolume;
	}

};

struct instrument_harmonica : public instrument_base
{
	instrument_harmonica()
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
			//+ 1.0  * synth::osc(n.on - dTime, synth::scale(n.id-12), synth::OSC_SAW_ANA, 5.0, 0.001, 100)
			+ 1.00 * osc(n.on - dTime, scale(n.id), OSC_SINE, 5.0, 0.001)
			+ 0.50 * osc(n.on - dTime, scale(n.id + 12), OSC_SINE)
			+ 0.05 * osc(n.on - dTime, scale(n.id + 24), OSC_SINE);

		return dAmplitude * dSound * dVolume;
	}

};