#pragma once
#include <pch.hpp>

namespace sglf
{
	struct Sound
	{
		static void initialize();
		static void finalize();

		static LPDIRECTSOUND dsound;
		static LPDIRECTSOUNDBUFFER primaryBuffer;
		static WAVEFORMATEX waveFormat;
		static DSBUFFERDESC primaryBufferDesc;

		Sound(const char *fileName, bool loop = false);
		~Sound();
		void play();
		void setVolume(long volume);
		void setPitch(unsigned int freq);
		
		LPDIRECTSOUNDBUFFER buffer;
		unsigned long bufferSize;
		char *bufferData;
		bool loop;
	};
}