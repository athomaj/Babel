#ifndef CONNECTION_LIN_H
#define CONNECTION_LIN_H
#include "IClient.h"
#include <sys/types.h>
#ifdef _WIN32
# include <WinSock2.h>
#else
# include <sys/socket.h>
# include <sys/times.h>
# include <netdb.h>
# include <netinet/in.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <net/if.h>
#endif
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

class Connection_Lin : public IClient
{
public:
    Connection_Lin(std::string const&);
	~Connection_Lin();
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
	int			client_fd;
	struct sockaddr_storage serverStorage;
	socklen_t		addr_size;
	std::vector<t_soundPacket>	recordSoundPackets;
	std::vector<t_soundPacket>	playSoundPackets;
	bool		recording;
	int			fd;
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
	//	std::mutex		mtx;
};

#endif
