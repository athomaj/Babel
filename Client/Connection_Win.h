#ifndef CONNECTION_WIN_H
#define CONNECTION_WIN_H
#include "IClient.h"
#include <list>
#include <string>
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#pragma comment(lib, "Ws2_32.lib")

#include "IClient.h"
#include <sys/types.h>
#include <exception>
#include <stdexcept>
#include <string.h>
#include <string>
#include <iostream>
#include <list>
#include <string>
#include <map>
#include <vector>
#include <list>

# include "Identity.hpp"
# include "ManagePacket.hh"
#include "Action.hh"

typedef struct s_soundPacket t_soundPacket;
class TAction;
class ManagePacket;
class PortAudio;
class IClient;

class Connection_Win : public IClient
{
public:
	Connection_Win(std::string const&);
	~Connection_Win();
	virtual bool connection(int, const std::string &);
	virtual bool Close(int);
	virtual int handle_connection();
	virtual int	create_server(int);
	virtual void write_fd();
	virtual void read_fd();
	virtual void set_fd();
	virtual void	write_to_server(Packet*, Instruct);
	void	check_write_fd();

	virtual void	addSoundPacket(const int, unsigned char*, std::vector<t_soundPacket>&);
	virtual t_soundPacket	getPlayPacket();
	virtual t_soundPacket	getRecordPacket();
	virtual std::vector<t_soundPacket>& getRecordPackets();
	virtual std::vector<t_soundPacket>& getPlayPackets();
	virtual bool	isPlayVectEmpty();
	virtual bool 	isRecVectEmpty();
	virtual bool	getRecording() const;
	virtual bool		connectionHost(int, const std::string &);
	virtual std::list<Packet*>	getPacket() const;
	virtual void	setPacket(std::list<Packet*>);
	virtual void 	addResponse(Packet *);
    virtual int  checkResponses();
	virtual void resetClientFd();
	virtual PortAudio*    getAudio();
	virtual void setAudio(PortAudio*);
	virtual int getStreamStatus() const;
	virtual int getFd() const;
	virtual void setGui(IGui *);

private:
	std::list<int>		clients;
	fd_set                  fd_read;
	fd_set                  fd_write;
	SOCKET			client_fd;
	struct sockaddr_storage serverStorage;
	socklen_t		addr_size;
	std::vector<t_soundPacket>	recordSoundPackets;
	std::vector<t_soundPacket>	playSoundPackets;
	bool		recording;
	SOCKET			fd;
	bool		write_server;
	bool		isHost;
	struct sockaddr_in    s_in;
	std::list<Packet*> queuRcvServer;
	std::vector<ManagePacket> queuSendServer;
	std::list<Packet*> queuPacket;
	std::string _username;
	TAction actions;
	PortAudio* audio;
	int streamStatus;
	IGui* _gui;
};
#endif
