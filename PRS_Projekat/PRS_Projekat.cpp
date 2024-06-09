#include <list>
#include <iostream>
#include <algorithm>

#define FTYPE double

#include "olcNoiseMaker.h"
#include "Instruments.h"
#include "Envelope.h"
#include "note.h"
#include "Utilities.h"


using namespace std;


vector<note> vecNotes;
mutex muxNotes;
instrument_test instrument;



typedef bool(*lambda)(note const& item);
template<class T>

void safe_remove(T& v, lambda f)
{
	auto n = v.begin();
	while (n != v.end())
		if (!f(*n))
			n = v.erase(n);
		else
			++n;
}

// Function used by olcNoiseMaker to generate sound waves
// Returns amplitude (-1.0 to +1.0) as a function of time
FTYPE MakeNoise(int nChannel, FTYPE dTime)
{
	unique_lock<mutex> lm(muxNotes);
	FTYPE dMixedOutput = 0.0;

	for (auto& n : vecNotes)
	{
		bool bNoteFinished = false;
		FTYPE dSound = 0;
		if (n.channel == 1)
			dSound = instrument.sound(dTime, n, bNoteFinished) * 0.5;
		dMixedOutput += dSound;

		if (bNoteFinished && n.off > n.on)
			n.active = false;
	}

	safe_remove<vector<note>>(vecNotes, [](note const& item) { return item.active; });


	return dMixedOutput * 0.2;
}

int main()
{

	// Get all sound hardware
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	// Display findings
	for (auto d : devices) wcout << "Found Output Device: " << d << endl;
	wcout << "Using Device: " << devices[0] << endl;

	// Display a keyboard
	wcout << endl <<
		"|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << endl <<
		"|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |" << endl <<
		"|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << endl <<
		"|     |     |     |     |     |     |     |     |     |     |" << endl <<
		"|  Y  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |" << endl <<
		"|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << endl << endl;

	wcout << endl <<
		"|   |    |   |   |    | |    |  |   |    | |    | |    |   |   |" << endl <<
		"|   | A# |   |   | C# | | D# |  |   | F# | | G# | | A# |   |   |" << endl <<
		"|   |____|   |   |____| |____|  |   |____| |____| |____|   |   |__" << endl <<
		"|     |      |     |     |      |     |      |      |      |     |" << endl <<
		"|  A  |  B   |  C  |  D  |  E   |  F  |  G   |  A   |  B   |  C  |" << endl <<
		"|_____|______|_____|_____|______|_____|______|______|______|_____|" << endl << endl;


	// Create sound machine!!
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	// Link noise function with sound machine
	sound.SetUserFunction(MakeNoise);

	char keyboard[129];
	memset(keyboard, ' ', 127);
	keyboard[128] = '\0';

	auto clock_old_time = chrono::high_resolution_clock::now();
	auto clock_real_time = chrono::high_resolution_clock::now();
	double dElapsedTime = 0.0;

	while (1)
	{
		for (int k = 0; k < 16; k++)
		{
			short nKeyState = GetAsyncKeyState((unsigned char)("YSXCFVGBNJMK\xbcL\xbe\xbf"[k]));

			double dTimeNow = sound.GetTime();

			// Check if note already exists in currently playing notes
			muxNotes.lock();
			auto noteFound = find_if(vecNotes.begin(), vecNotes.end(), [&k](note const& item) { return item.id == k; });
			if (noteFound == vecNotes.end())
			{
				// Note not found in vector

				if (nKeyState & 0x8000)
				{
					// Key has been pressed so create a new note
					note n;
					n.id = k;
					n.on = dTimeNow;
					n.channel = 1;
					n.active = true;

					// Add note to vector
					vecNotes.emplace_back(n);
				}
				else
				{
					// Note not in vector, but key has been released...
					// ...nothing to do
				}
			}
			else
			{
				// Note exists in vector
				if (nKeyState & 0x8000)
				{
					// Key is still held, so do nothing
					if (noteFound->off > noteFound->on)
					{
						// Key has been pressed again during release phase
						noteFound->on = dTimeNow;
						noteFound->active = true;
					}
				}
				else
				{
					// Key has been released, so switch off
					if (noteFound->off < noteFound->on)
					{
						noteFound->off = dTimeNow;
					}
				}
			}
			muxNotes.unlock();
		}
		wcout << "\rNotes: " << vecNotes.size() << "    ";

		this_thread::sleep_for(5ms);
	}

}
