#ifndef ACLIENT_H
#define ACLIENT_H
#include "Packet.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <netdb.h>
#include <netinet/in.h>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <list>
#include <string>
#include <map>
# include <vector>

typedef struct 	s_soundPacket
{
	int 			retenc;
	int 			dataSize;
	unsigned char		data[480];
}				t_soundPacket;

class AClient
{
public:
	virtual bool connection(int, const std::string &) = 0;
	//virtual bool send_message(std::string &) = 0;
	//virtual std::string receive_message() = 0;
	virtual bool Close(int) = 0;
	virtual void handle_connection() = 0;
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
	virtual void write_to_server(Packet*) = 0;
	virtual bool	connectionHost(int, const std::string &) = 0;
	virtual std::list<Packet *> getPacket() const = 0;
	virtual void    setPacket(std::list<Packet*>) = 0;
	int fd;
};
#endif
