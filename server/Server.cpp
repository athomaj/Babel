#include "Server.hh"


Server::Server(int const port)
{
  if ((pe = getprotobyname("TCP")) == NULL)
    throw ("init of protoent failed\n");
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(port);
  s_in.sin_addr.s_addr = INADDR_ANY;
  if ((fd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    throw ("server socket init failed\n");
  if (bind(fd, (const struct sockaddr *)(&s_in), sizeof(s_in)) == -1)
    {
      close(fd);
      throw ("bind failed\n");
    }
  if (listen(fd, 42) == -1)
    {
      close(fd);
      throw ("listen failed\n");
    }
  maxFd = fd;
  std::cout << "Server listening on port " << port << std::endl;
}

Server::~Server()
{
}

void    Server::setFds()
{
  FD_ZERO(&fdRead);
  FD_ZERO(&fdWrite);
  for (unsigned int i = 0; i < clients.size(); ++i)
    {
      FD_SET(clients[i]->getFd(), &fdRead);
      if (!clients[i]->isToSendEmpty())
      {
        FD_SET(clients[i]->getFd(), &fdWrite);
      }
      maxFd = (clients[i]->getFd() > maxFd ? clients[i]->getFd() : maxFd);
    }
  FD_SET(fd, &fdRead);
}

std::vector<Client *>&     Server::getClients()
{
  return (clients);
}

void    Server::addClient()
{
  struct sockaddr_in    s_in_c;
  socklen_t             s_size;
  int                   fd_c;
  int			err;

  if ((fd_c = accept(fd, (struct sockaddr *)(&s_in_c),
		     &(s_size))) == -1)
    {
      close(fd);
      throw ("Accept failed");
    }
  clients.push_back(new Client(fd_c, this));
}

void    Server::checkFds()
{
  std::vector<Client *>::iterator it = clients.begin();
  std::vector<std::vector<Client *>::iterator>    itList;

  while (it != clients.end())
    {
      if (FD_ISSET((*it)->getFd(), &fdRead) && (*it)->receiveData() == false)
    	{
    	  itList.push_back(it);
    	  std::cout << "Client left" << std::endl;
    	}
      else if (FD_ISSET((*it)->getFd(), &fdWrite))
      {
        (*it)->sendData();
      }
      ++it;
    }
  for (unsigned int i = 0; i < itList.size(); ++i)
  {
    for (unsigned int j=0; j < clients.size(); ++j)
      if (clients[j]->getUsername() != (*itList[i])->getUsername())
      {
        std::cout << "deleting" << std::endl;
        Identity resp((*itList[i])->getUsername(), DELCONTACT);
        Packet *t = new Packet(resp);
        clients[j]->addPacketToSend(t);
      }
    clients.erase(itList[i]);
  }
  if (FD_ISSET(fd, &fdRead))
    addClient();
}

void    Server::select_manage()
{
  while (42)
    {
      setFds();
      if (select(maxFd + 1, &fdRead, &fdWrite, NULL, NULL) == -1)
        throw ("select failed\n");
      checkFds();
        usleep(100000);
    }
}
