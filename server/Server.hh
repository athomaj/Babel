/* 
 * File:   server.hh
 * Author: maillo_a
 *
 * Created on October 30, 2015, 6:40 PM
 */

#ifndef	SERVER_HH
# define SERVER_HH

#include <condition_variable>
#include <sstream>
# include <stdexcept>
# include <sys/select.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/param.h>
# include <sys/stat.h>
# include <dirent.h>
# include <vector>
# include <sstream>
# include <iostream>
# include <fstream>
# include <errno.h>
# include "../Client/Packet.h"

class Client;

class Server
{
public:
  Server(int const port=4242);
  ~Server();
  void  setFds();
  void  checkFds();
  void  addClient();
  void  select_manage();
  std::vector<Client *>&   getClients();
private:
  struct protoent       *pe;
  int                   fd;
  struct sockaddr_in    s_in;
  std::vector<Client *>   clients;
  int                   maxFd;
  fd_set                fdRead;
  fd_set                fdWrite;
  char			buf[4096];
  
};

#include "Client.hh"

#endif	/* SERVER_HH */

