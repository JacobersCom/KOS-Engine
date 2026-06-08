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

void KOSAudio::PrintHResult(HRESULT hr)
{

	DWORD result = HRESULT_CODE(hr);

	LPSTR messageBuffer = NULL;
	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER //Allocated buffer large enought to hold message
		| FORMAT_MESSAGE_FROM_SYSTEM, //Functions looks through the message table for the requested message
		NULL,
		result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		messageBuffer,
		0, NULL
	))
	{
		if (result == FACILITY_WIN32)
		{
			wsprintf("Error: %s", messageBuffer);
			LocalFree(messageBuffer);
		}
		else
		{
			wsprintf("Result: %s", messageBuffer);
			LocalFree(messageBuffer);
		}
	}
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

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator,
		NULL,
		CLSCTX_ALL, //activate an object using any server type
		IID_IMMDeviceEnumerator,
		(void**)&pEnumerator
	);

	PrintHResult(hr);

	hr = pEnumerator->GetDefaultAudioEndpoint(
		eRender,//The data flow to render audio to the end point
		eConsole,//the role of this audio end point
		&pDevice //End point
	);
	
	PrintHResult(hr);


}