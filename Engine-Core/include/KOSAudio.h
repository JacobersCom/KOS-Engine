#pragma once


class KOSAudio
{
public:

	KOSAudio();
	~KOSAudio();

	KOSAudio(const KOSAudio&) = delete;
	KOSAudio& operator=(const KOSAudio) = delete;


private: 

	void CreateAudioStream();
};