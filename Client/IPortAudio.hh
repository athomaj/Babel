#ifndef IPORTAUDIO_HH_
# define IPORTAUDIO_HH_

# include <portaudio.h>
# include <cstdlib>
# include <iostream>

class Connection_Lin;

# define SAMPLE_RATE 24000
# define NUM_CHANNELS 2
# define FRAMES_PER_BUFFER 480
# define SAMPLE_SILENCE  (0.0f)
# define PA_SAMPLE_TYPE paFloat32

typedef float SAMPLE;

class EncodeManager;

class IPortAudio
{
public:
	virtual void			initialize() = 0;
	virtual void			initInputDevice() = 0;
	virtual void			initOutputDevice() = 0;
	virtual void			openPlayStream() = 0;
	virtual void			openRecordStream() = 0;
	virtual void			recordSound() = 0;
	virtual void			playSound() = 0;
    virtual void            closePlaystream() = 0;
    virtual void            closeRecordstream() = 0;
    virtual SAMPLE 			*getRecordedSample() const = 0;
	virtual void			setRecordedSample(SAMPLE*) = 0;
	virtual EncodeManager&	getEncodeManager() const = 0;
	virtual int&			getRetenc() const = 0;
	virtual void			setEncodedData(unsigned char *) = 0;
	virtual unsigned char*	getEncodedData() const = 0;
	virtual Connection_Lin&			getClient() const = 0;
};

# include "EncodeManager.hh"
#ifdef _WIN32
# include "Connection_Win.h"
#else
# include "Connection_Lin.h"
#endif

#endif
