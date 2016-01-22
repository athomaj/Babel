#ifndef CLIENT_HH_
# define CLIENT_HH_

# include "../Client/Identity.hpp"
# include "Server.hh"
# include "ManageCommand.hh"

class   Client
{
public:
  Client(int const fd, Server *serv);
  ~Client();
  int   getFd() const;
  bool  receiveData();
  void	sendData();
  void	addPacketToSend(Packet *);
  Packet *getPacketToSend();
	bool  isToSendEmpty() const;
  void setIp(char const *ip);
  void setUsername(char const *ip);
  void setPort(int const port);
  Server* getServer() const;
  std::string &getUsername();
  std::string &getIp();
  int getPort() const;
private:
  int           fd;
  Server                *_serv;
  std::string   ip;
  int           port;
  std::string username;
  ManageCommand 		comManager;
  std::vector<Packet *>	toSend;
};

#endif


