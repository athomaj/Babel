#ifndef ICLIENT_H
#define ICLIENT_H
#include "Packet.h"
#ifdef _WIN32
# include <WinSock2.h>
#else
# include <sys/socket.h>
# include <sys/times.h>
# include <netdb.h>
# include <netinet/in.h>
# include <unistd.h>
# include <arpa/inet.h>
#endif
#include <sys/types.h>
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#include <list>
#include <string>
#include <map>
# include <vector>
#include "IGui.h"

class PortAudio;

std::string getIpAddr();

typedef struct 	s_soundPacket
{
	int 			retenc;
	int 			dataSize;
	unsigned char		data[480];
}				t_soundPacket;

class IClient
{
public:
    virtual bool connection(int, std::string const&) = 0;
	//virtual bool send_message(std::string &) = 0;
	//virtual std::string receive_message() = 0;
	virtual bool Close(int) = 0;
    virtual int handle_connection() = 0;
	virtual int create_server(int) = 0;
	virtual void write_fd() = 0;
        virtual void read_fd() = 0;
	//virtual void add_user(int) = 0;
	virtual void set_fd() = 0;
	virtual void	addSoundPacket(const int, unsigned char*, std::vector<t_soundPacket>&) = 0;
	virtual t_soundPacket	getPlayPacket() = 0;
	virtual t_soundPacket	getRecordPacket() = 0;
	virtual std::vector<t_soundPacket>& getRecordPackets() = 0;
	virtual std::vector<t_soundPacket>& getPlayPackets() = 0;
	virtual bool	isPlayVectEmpty() = 0;
	virtual bool 	isRecVectEmpty() = 0;
	virtual bool	getRecording() const = 0;
	virtual void write_to_server(Packet*, Instruct) = 0;
	virtual bool	connectionHost(int, const std::string &) = 0;
	virtual std::list<Packet *> getPacket() const = 0;
	virtual void    setPacket(std::list<Packet*>) = 0;
	virtual void 	addResponse(Packet *) = 0;
    virtual int  checkResponses() = 0;
    virtual void resetClientFd() = 0;
    virtual PortAudio*    getAudio() = 0;
    virtual void          setAudio(PortAudio*) = 0;
    virtual int getStreamStatus() const = 0;
    virtual int getFd() const = 0;
	virtual void setGui(IGui *) = 0;
};

#include "portAudio.hh"

#endif
