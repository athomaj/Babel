#ifndef PORTAUDIO_HH_
# define PORTAUDIO_HH_

# include "IPortAudio.hh"

class PortAudio
{
public:
	PortAudio(IClient *client);
	~PortAudio();
	virtual void			initialize();
	virtual void			initInputDevice();
	virtual void			initOutputDevice();
	virtual void			openPlayStream();
	virtual void			openRecordStream();
	virtual void			recordSound();
	virtual void			playSound();
    virtual void            closePlaystream();
    virtual void            closeRecordstream();
	virtual SAMPLE 			*getRecordedSample() const;
	virtual void			setRecordedSample(SAMPLE*);
	virtual EncodeManager& 	getEncodeManager() const;
	virtual int& 		  	getRetenc();
	virtual void			setEncodedData(unsigned char *);
	virtual unsigned char*	getEncodedData() const;
	virtual IClient&			getClient() const;
private:
	PaError	paErr;
	SAMPLE 	*recordedSample;
	PaStreamParameters	inputParameters;
	PaStreamParameters	outputParameters;
	PaStream*           streamIn;
	PaStream*           streamOut;
	EncodeManager		*encMgr;
	IClient				*client;
	int 				retEnc;
	unsigned char		*encodedData;
};

# include "EncodeManager.hh"

#endif
