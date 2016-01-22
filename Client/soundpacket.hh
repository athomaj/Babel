#ifndef SOUNDPACKET_H
#define SOUNDPACKET_H

class SoundPacket
{
public:
    SoundPacket(int const, int const, unsigned char*);
    unsigned char* getData();
    int getDatasize() const;
    int getRetenc() const;
private:
    int             _retenc;
    int             _datasize;
    unsigned char   _data[480];
};

#endif // SOUNDPACKET_H
