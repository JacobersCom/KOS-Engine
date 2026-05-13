#pragma once

#include "KOSAudio.h"

//WASAPI interfaces
#include <Audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>

//ComponentObjectModel Files
#include <objbase.h>

//CLSID is used to identify GUIDs 
constexpr CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);

//interface Identifiers
constexpr IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
constexpr IID IID_IAudioClient = __uuidof(IAudioClient);
//Interface to multimedia devices
constexpr IID IID_IMMDeviceEnumrator = __uuidof(IMMDeviceEnumerator);

KOSAudio::KOSAudio()
{

}

KOSAudio::~KOSAudio()
{

}

void KOSAudio::CreateAudioStream()
{
	HRESULT hr;
	//Uint64_t type used for clock time
	REFERENCE_TIME RequestedDuration = 10000000;
	REFERENCE_TIME ActualDuration;

	IMMDeviceEnumerator* pEnumerator = nullptr;

	//Used to interface with the audio end points
	IMMDevice* pDevice = nullptr;

	//Used to inerface to create and initialize an audio stream
	//Between the audio app and engine
	IAudioClient* pAudioClient = nullptr;

	//Used to enable client write output data to a rendering
	//end point
	IAudioRenderClient* pAudioRenderer = nullptr;

	//Defines the format of waveform-audio data
	WAVEFORMATEX* pwfx = nullptr;

	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE* pData;
	DWORD flags = 0;
}