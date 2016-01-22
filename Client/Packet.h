//
// Created by rustam_t on 10/13/15.
//

#ifndef CPP_BABEL_PACKET_H
#define CPP_BABEL_PACKET_H

#include <string>
#include <vector>
#include <typeinfo>
#include "Rsa2.h"
#include "Identity.hpp"

#define _MAGIC_ 0x0101010

class Packet
{
public:

    virtual ~Packet() {};

    enum Type {

        String = 0xb16b00b5,
        IntVector = 0x42424242,
        Id = 0x12345678,
        Inst = 0x51515151,
        SSLPublicKey = 0x4ab2321a
    };

    //object specific constructors
    Packet(Instruct inst);
    Packet(Identity& id);
    Packet(std::string &str);
    Packet(std::vector<int> &vec);
    Packet(Rsa &);

    static std::vector<unsigned char>& stringToStream(char *buf, int const size)
    {
        std::vector<unsigned char>* stream = new std::vector<unsigned char>();

        for (int i=0; i < size; ++i)
        {
            stream->push_back(buf[i]);
        }
        return (*stream);
    }

    static void streamToString(std::vector<unsigned char>* stream, unsigned char* data)
    {
        for (unsigned int i=0; i < stream->size(); ++i)
        {
            data[i] = (*stream)[i];
        }
    }

    template <typename T>
    static Packet *pack(T &obj)
    {
        return (new Packet(obj));
    };

    //build a bytestream from the packet
    std::vector<unsigned char>              *build(Rsa *rsa = NULL);

    //reconstruct the packet from vector stream which will be consumed if succeeded
    static Packet                           *fromStream(std::vector<unsigned char> &data, Rsa *rsa = NULL);

    //unpack the packet to get your object back
    template <typename T>
    T *unpack(Rsa *rsa = NULL)
    {
        if (rsa != NULL && this->_encrypted) {
            this->_data = rsa->decrypt(this->_data);
            this->_encrypted = false;
        }
        if (typeid(T) == typeid(std::string))
            return (reinterpret_cast<T *>(this->getString()));
        else if (typeid(T) == typeid(std::vector<int>))
            return (reinterpret_cast<T *>(this->getIntVector()));
        else if (typeid(T) == typeid(Rsa))
            return (reinterpret_cast<T *>(this->getRsa()));
        else if (typeid(T) == typeid(Identity))
            return (reinterpret_cast<T *>(this->getIdentity()));
        return (NULL);
    };

    //just 3 * (sizeof(unsigned int))
    static unsigned int                     getHeaderSize();

    Type                                    getType() const;

    bool                                    isEncrypted() const;

    std::vector<unsigned char>              &getData();

    Identity*                               getIdentity();
    std::string *getString();

    Instruct*                               getInstruction();

protected:

    //properties
    Type _type;
    bool _encrypted;
    std::vector<unsigned char> _data;

    //constructor
    Packet();

    //object getters
    std::vector<int> *getIntVector();
    Rsa *getRsa();
};

#endif //CPP_BABEL_PACKET_H
