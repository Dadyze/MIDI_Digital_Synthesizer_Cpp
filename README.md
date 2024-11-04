# MIDI Digital Synthesizer in C++

This project is a C++ implementation of a digital synthesizer capable of generating audio output based on MIDI input. It allows users to control various sound parameters like pitch, volume, and modulation to simulate different musical instruments.

## Features

- **MIDI Input Processing**: Reads and interprets MIDI signals to produce corresponding audio tones.
- **Waveform Generation**: Supports multiple waveform types (sine, square, triangle, and sawtooth) for sound generation.
- **Polyphony**: Allows multiple notes to be played simultaneously.
- **Envelope Control**: Adjustable attack, decay, sustain, and release (ADSR) for precise sound shaping.
- **Real-time Synthesis**: Generates audio output in real-time, responding instantly to MIDI input changes.
- **Cross-Platform Support**: Designed to be compiled and run on various platforms including Linux, macOS, and Windows.

## Getting Started

### Prerequisites

To build and run this project, you will need:

- A C++ compiler supporting C++11 or later (e.g., GCC, Clang, MSVC).
- A MIDI keyboard or controller (optional but recommended).
- Audio output hardware (e.g., speakers or headphones).

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/Dadyze/MIDI_Digital_Synthesizer_Cpp.git
   cd MIDI_Digital_Synthesizer_Cpp
   g++ -std=c++11 -m32 -o synthesizer main.cpp
