#include <sound.hpp>
using namespace sglf;

#include "window.hpp"

LPDIRECTSOUND Sound::dsound;
LPDIRECTSOUNDBUFFER Sound::primaryBuffer;
WAVEFORMATEX Sound::waveFormat;
DSBUFFERDESC Sound::primaryBufferDesc;

void Sound::initialize()
{
	DirectSoundCreate(nullptr, &Sound::dsound, nullptr);
	Sound::dsound->SetCooperativeLevel(Window::hwnd, DSSCL_PRIORITY);

	ZeroMemory(&Sound::waveFormat, sizeof(WAVEFORMATEX));
	Sound::waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	Sound::waveFormat.nChannels = 2;
	Sound::waveFormat.nSamplesPerSec = 44100;
	Sound::waveFormat.wBitsPerSample = 16;
	Sound::waveFormat.nBlockAlign = (Sound::waveFormat.nChannels * Sound::waveFormat.wBitsPerSample) / 8;
	Sound::waveFormat.nAvgBytesPerSec = Sound::waveFormat.nSamplesPerSec * Sound::waveFormat.nBlockAlign;

	ZeroMemory(&Sound::primaryBufferDesc, sizeof(DSBUFFERDESC));
	Sound::primaryBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	Sound::primaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	Sound::dsound->CreateSoundBuffer(&Sound::primaryBufferDesc, &Sound::primaryBuffer, nullptr);
	Sound::primaryBuffer->SetFormat(&Sound::waveFormat);
}

void Sound::finalize()
{
	Sound::primaryBuffer->Release();
	Sound::dsound->Release();
}

Sound::Sound(const char *fileName, bool loop)
: loop(loop)
{
	OggVorbis_File vorbisFile;
	ov_fopen(fileName, &vorbisFile);
	bufferSize = ov_pcm_total(&vorbisFile, -1) * Sound::waveFormat.nBlockAlign;

	DSBUFFERDESC secondaryBufferDes;
	ZeroMemory(&secondaryBufferDes, sizeof(DSBUFFERDESC));
	secondaryBufferDes.dwSize = sizeof(DSBUFFERDESC);
	secondaryBufferDes.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	secondaryBufferDes.dwBufferBytes = bufferSize;
	secondaryBufferDes.lpwfxFormat = &Sound::waveFormat;

	Sound::dsound->CreateSoundBuffer(&secondaryBufferDes, &buffer, nullptr);
	buffer->SetFormat(&Sound::waveFormat);
	buffer->Lock(0, bufferSize, (LPVOID*)&bufferData, &bufferSize, nullptr, 0, 0);

	unsigned long bytesRead;
	unsigned long totalRead = 0;

	while (totalRead < bufferSize) {

		bytesRead = ov_read(&vorbisFile, bufferData + totalRead, bufferSize - totalRead, 0, 2, 1, nullptr);
		
		if (bytesRead <= 0)
			break;

		totalRead += bytesRead;
	}
	buffer->Unlock(bufferData, totalRead, nullptr, 0);
	ov_clear(&vorbisFile);

	// Preload sound
	buffer->Play(0, 0, 0);
	buffer->Stop();
	buffer->SetCurrentPosition(0);
}

Sound::~Sound()
{
	buffer->Release();
}

void Sound::play()
{
	buffer->SetCurrentPosition(0);
	buffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
}

void Sound::setVolume(long volume)
{
	buffer->SetVolume(volume);
}

void Sound::setPitch(unsigned int freq)
{
	buffer->SetFrequency(freq);
}