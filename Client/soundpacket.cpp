#include "soundpacket.hh"

SoundPacket::SoundPacket(int const retenc, int const datasize, unsigned char* data) :
    _retenc(retenc), _datasize(datasize)
{
    for (unsigned int i = 0; i < 480; ++i)
        _data[i] = data[i];
}

unsigned char* SoundPacket::getData()
{
    return (_data);
}

int SoundPacket::getDatasize() const
{
    return (_datasize);
}

int SoundPacket::getRetenc() const
{
    return (_retenc);
}
