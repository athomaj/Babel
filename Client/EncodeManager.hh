#ifndef ENCODE_MANAGER_HH_
# define ENCODE_MANAGER_HH_

# include "IEncodeManager.hh"

class EncodeManager : IEncodeManager
{
public:
	EncodeManager();
	~EncodeManager();
	virtual unsigned char*	encodeAudio(const SAMPLE *frame, int &retenc);
	virtual void			decodeAudio(const unsigned char *, SAMPLE *,int const& retenc);
private:
	OpusEncoder	*encode;
	OpusDecoder	*decode;
	opus_int32	length;
	int 		err;
};

#endif
