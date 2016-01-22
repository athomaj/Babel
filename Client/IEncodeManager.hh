#ifndef IENCODE_MANAGER_HH_
# define IENCODE_MANAGER_HH_

# include <opus.h>
# include "portAudio.hh"

class IEncodeManager
{
public:
	virtual unsigned char*	encodeAudio(const SAMPLE *frame, int &retenc) = 0;
	virtual void			decodeAudio(const unsigned char *, SAMPLE *, int const& retenc) = 0;
};

#endif
