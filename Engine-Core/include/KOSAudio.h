#pragma once

#include <winerror.h>


class KOSAudio
{
public:

	KOSAudio();
	~KOSAudio();

	KOSAudio(const KOSAudio&) = delete;
	KOSAudio& operator=(const KOSAudio) = delete;


private: 

	void CreateAudioStream();

	void PrintHResult(HRESULT hr);
};