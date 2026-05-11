#pragma once

#include "KOSAudio.h"

//WASAPI interfaces
#include <Audioclient.h>
#include <audiopolicy.h>

//ComponentObjectModel Files
#include <objbase.h>

//interface Identifiers
constexpr IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

//CLSID is used to identify GUIDs 
constexpr CLSID CLSID_MMDEviceEnumerator = 



KOSAudio::KOSAudio()
{

}

KOSAudio::~KOSAudio()
{

}

void KOSAudio::CreateAudioStream()
{
	
}