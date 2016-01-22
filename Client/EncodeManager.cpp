#include "EncodeManager.hh"


EncodeManager::EncodeManager()
{
	encode = opus_encoder_create(48000, 2, OPUS_APPLICATION_VOIP, &err);
	decode = opus_decoder_create(48000, 2, &err);
	opus_encoder_ctl(encode, OPUS_GET_BANDWIDTH(&length));
	length = 480;
}

EncodeManager::~EncodeManager()
{
	opus_encoder_destroy(encode);
	opus_decoder_destroy(decode);
}

unsigned char*	EncodeManager::encodeAudio(const SAMPLE *frame, int &retenc)
{
	unsigned char* compressed = static_cast<unsigned char*>(malloc(length));

	retenc = opus_encode_float(encode, frame, FRAMES_PER_BUFFER, compressed, length);
	return (compressed);
}

void			EncodeManager::decodeAudio(const unsigned char *data, SAMPLE* out, int const& retenc)
{
  int			rd;

  rd = opus_decode_float(decode, data, retenc, out, FRAMES_PER_BUFFER, 0);
//	for (unsigned int i=0; i < 480; )
  (void)rd;
}
