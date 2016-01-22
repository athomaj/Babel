#include "portAudio.hh"

PortAudio::PortAudio(IClient *client) :
    streamIn(NULL), streamOut(NULL)
{
	encMgr = new EncodeManager();
	recordedSample = static_cast<SAMPLE *>(malloc(FRAMES_PER_BUFFER * 2 * sizeof(SAMPLE)));
    if (recordedSample == NULL)
    	std::cerr << "sample alloc failed" << std::endl;
    memset(recordedSample, 0, FRAMES_PER_BUFFER * 2 * sizeof(SAMPLE));
    this->client = client;
}

PortAudio::~PortAudio()
{

}

static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData)
{
    PortAudio *data = static_cast<PortAudio *>(userData);
    SAMPLE* data2 = (SAMPLE *)(inputBuffer);
    unsigned char *tmp;

    if (data->getClient().getRecording() == true)
    {
        tmp = data->getEncodeManager().encodeAudio(data2, data->getRetenc());
        data->getClient().addSoundPacket(data->getRetenc(), tmp, data->getClient().getRecordPackets());
    }

    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;
    (void) framesPerBuffer;
    return data->getClient().getStreamStatus();
}

static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    PortAudio *data = static_cast<PortAudio *>(userData);
    SAMPLE *data2 = (SAMPLE *)outputBuffer;

//    std::cout << "try" << std::endl;
    if (data->getClient().isPlayVectEmpty() == false)
    {
        t_soundPacket tmpPacket = data->getClient().getPlayPacket();
        data->getEncodeManager().decodeAudio(tmpPacket.data, data2, tmpPacket.retenc);
    }
    else
    {
//        std::cout << "empty" << std::endl;
        memset(data2, 0.0f, 480*2);
    }

    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;
    (void) inputBuffer;
    (void) outputBuffer;
    (void) framesPerBuffer;
    (void) data;
    return data->getClient().getStreamStatus();
}

IClient&         PortAudio::getClient() const
{
    return (*client);
}

unsigned char*	PortAudio::getEncodedData() const
{
	return (encodedData);
}

EncodeManager& PortAudio::getEncodeManager() const
{
    return (*encMgr);
}

int&            PortAudio::getRetenc()
{
    return (retEnc);
}

SAMPLE* PortAudio::getRecordedSample() const
{
    return (recordedSample);
}

void			PortAudio::setEncodedData(unsigned char *encData)
{
	encodedData = encData;
}

void			PortAudio::setRecordedSample(SAMPLE* sample)
{
	recordedSample = sample;
}

void	PortAudio::initialize()
{
	paErr = Pa_Initialize();
    if (paErr != paNoError)
    	std::cerr << "PortAudio initalization faied" << std::endl;
}

void	PortAudio::initInputDevice()
{
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        std::cerr << "Error: No default input device." << std::endl;
    }
    inputParameters.channelCount = 2;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
}

void	PortAudio::initOutputDevice()
{
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        std::cerr << "Error: No default output device." << std::endl;
    }
    outputParameters.channelCount = 2;                     /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
}

void	PortAudio::openRecordStream()
{
    paErr = Pa_OpenStream(&streamIn, &inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, recordCallback, this);
    if (paErr != paNoError)
    	std::cerr << "Failed opening stream" << std::endl;
}

void	PortAudio::openPlayStream()
{
    paErr = Pa_OpenStream(&streamOut, NULL, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, playCallback, this);
    if (paErr != paNoError)
    	std::cerr << "Failed opening stream" << std::endl;
}

void	PortAudio::recordSound()
{
	paErr = Pa_StartStream(streamIn);
    if (paErr != paNoError)
    	std::cerr << "Failed to start record stream" << std::endl;
}

void	PortAudio::playSound()
{
    paErr = Pa_StartStream( streamOut);
    if (paErr != paNoError)
    	std::cerr << "Failed to start play stream" << std::endl;
}

void            PortAudio::closePlaystream()
{
    if (!streamOut)
        return ;
    paErr = Pa_CloseStream(streamOut);
    streamOut = NULL;
    if (paErr != paNoError)
        std::cerr << "Failed to close play stream" << std::endl;
}

void            PortAudio::closeRecordstream()
{
    if (!streamIn)
        return ;
    paErr = Pa_CloseStream(streamIn);
    streamIn = NULL;
    if (paErr != paNoError)
        std::cerr << "Failed to close record stream" << std::endl;
}
