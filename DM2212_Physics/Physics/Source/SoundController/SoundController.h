/**
 CSoundController
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include <SingletonTemplate.h>

// Include GLEW
#include "irrKlang.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

// Include string
#include <string>
// Include map storage
#include <map>
using namespace std;

// Include SoundInfo class; it stores the sound and other information
#include "SoundInfo.h"

// Include SoundType enum
#include "SoundType.h"

typedef struct Fader Fader;

class CSoundController : public CSingletonTemplate<CSoundController>
{
	friend CSingletonTemplate<CSoundController>;
public:
	// Initialise this class instance
	bool Init(void);

	// Load a sound
	bool LoadSound(	string filename, 
					const int ID,
					const bool bPreload = true,
					const bool bIsLooped = false,
					CSoundInfo::SOUNDTYPE eSoundType = CSoundInfo::SOUNDTYPE::_2D,
					vec3df vec3dfSoundPos = vec3df(0.0f, 0.0f, 0.0f));

	void FadeUpdater(double dt);
	double ElapsedTime();

	// Play a sound by its ID
	void PlaySoundByID(const int ID);
	void PlaySoundByID(const int ID, float fadeTime, float fadeLeadTime);
	bool StopPlayingSoundByID(const int ID);
	void StopPlayingSoundByID(const int ID, float fadeTime, float fadeLeadTime);

	void SetMasterVolume(float v);
	// Increase Master volume
	bool MasterVolumeIncrease(void);
	// Decrease Master volume
	bool MasterVolumeDecrease(void);

	float& GetMasterVolume();

	// Increase volume of a ISoundSource
	bool VolumeIncrease(const int ID);
	bool VolumeIncrease(const int ID, float mag);
	// Decrease volume of a ISoundSource
	bool VolumeDecrease(const int ID);
	bool VolumeDecrease(const int ID, float mag);

	// For 3D sounds only
	// Set Listener position
	void SetListenerPosition(const float x, const float y, const float z);
	// Set Listener direction
	void SetListenerDirection(const float x, const float y, const float z);

protected:
	// Constructor
	CSoundController(void);

	// Destructor
	virtual ~CSoundController(void);

	// Get an sound from this map
	CSoundInfo* GetSound(const int ID);
	// Remove an sound from this map
	bool RemoveSound(const int ID);
	// Get the number of sounds in this map
	int GetNumOfSounds(void) const;

	// The handler to the irrklang Sound Engine
	ISoundEngine* cSoundEngine;

	// The map of all the entity created
	std::map<int, CSoundInfo*> soundMap;

	// For 3D sound only: Listener position
	vec3df vec3dfListenerPos;
	// For 3D sound only: Listender view direction
	vec3df vec3dfListenerDir;

	std::map<int, Fader*> fadeIn;
	std::map<int, Fader*> fadeOut;
	std::map<int, ISound*> activeSound;
	

private:
	double elapsed;
};

struct Fader
{
	float elapsed_fadeStartAt;
	float elapsed_fadeEndAt;
	float magnitudePerSecond;
	Fader(float fadeTime, float leadTime) {
		elapsed_fadeEndAt = CSoundController::GetInstance()->ElapsedTime() + leadTime + fadeTime;
		elapsed_fadeStartAt = elapsed_fadeEndAt - fadeTime;
		magnitudePerSecond = 1.0 / fadeTime;
	}
};


