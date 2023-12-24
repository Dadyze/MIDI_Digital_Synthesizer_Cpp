#pragma once
// Converts frequency (Hz) to angular velocity
FTYPE w(const FTYPE dHertz)
{
	return dHertz * 2.0 * PI;
}



//////////////////////////////////////////////////////////////////////////////
// Multi-Function Oscillator
const int OSC_SINE = 0;
const int OSC_SQUARE = 1;
const int OSC_TRIANGLE = 2;
const int OSC_SAW_ANA = 3;
const int OSC_SAW_DIG = 4;
const int OSC_NOISE = 5;

FTYPE osc(const FTYPE dTime, const FTYPE dHertz, const int nType = OSC_SINE,
	const FTYPE dLFOHertz = 0.0, const FTYPE dLFOAmplitude = 0.0, FTYPE dCustom = 50.0)
{

	FTYPE dFreq = w(dHertz) * dTime + dLFOAmplitude * dHertz * (sin(w(dLFOHertz) * dTime)); // osc(dTime, dLFOHertz, OSC_SINE);

	switch (nType)
	{
	case OSC_SINE: // Sine wave bewteen -1 and +1
		return sin(dFreq);

	case OSC_SQUARE: // Square wave between -1 and +1
		return sin(dFreq) > 0 ? 1.0 : -1.0;

	case OSC_TRIANGLE: // Triangle wave between -1 and +1
		return asin(sin(dFreq)) * (2.0 / PI);

	case OSC_SAW_ANA: // Saw wave (analogue / warm / slow)
	{
		FTYPE dOutput = 0.0;
		for (FTYPE n = 1.0; n < dCustom; n++)
			dOutput += (sin(n * dFreq)) / n;
		return dOutput * (2.0 / PI);
	}

	case OSC_SAW_DIG:
		return (2.0 / PI) * (dHertz * PI * fmod(dTime, 1.0 / dHertz) - (PI / 2.0));

	case OSC_NOISE:
		return 2.0 * ((FTYPE)rand() / (FTYPE)RAND_MAX) - 1.0;

	default:
		return 0.0;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Scale to Frequency conversion

const int SCALE_DEFAULT = 0;

FTYPE scale(const int nNoteID, const int nScaleID = SCALE_DEFAULT)
{
	switch (nScaleID)
	{
	case SCALE_DEFAULT: default:
		return 256 * pow(1.0594630943592952645618252949463, nNoteID);
	}
}




FTYPE envw(const FTYPE dTime, envelope& env, const FTYPE dTimeOn, const FTYPE dTimeOff)
{
	return env.amplitude(dTime, dTimeOn, dTimeOff);
}
