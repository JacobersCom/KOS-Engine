#pragma once

#include "KOSAudio.h"

//WASAPI interfaces
#include <Audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>

//ComponentObjectModel Files
#include <objbase.h>

//CLSID is used to identify GUIDs 
constexpr CLSID CLSID_MMDEviceEnumerator = __uuidof(MMDeviceEnumerator);

//interface Identifiers
constexpr IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
constexpr IID IID_IAudioClient = __uuidof(IAudioClient);
constexpr IID IID_IMMDeviceEnumrator = __uuidof(IMMDeviceEnumerator);

KOSAudio::KOSAudio()
{

}

KOSAudio::~KOSAudio()
{

}

void KOSAudio::CreateAudioStream()
{
	
}