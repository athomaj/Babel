#include "Client.hh"

Client::Client(int const fd, Server* serv) :
  fd(fd), _serv(serv)
{
  
}

Client::~Client()
{

}


Server* Client::getServer() const
{
  return (_serv);
}

std::string &Client::getUsername()
{
  return (username);
}

void Client::setUsername(char const *name)
{
  this->username = std::string(name);
}

void Client::setIp(char const *ip)
{
  this->ip = std::string(ip);
}

void Client::setPort(int const port)
{
  this->port = port;
}

void  Client::addPacketToSend(Packet *packet)
{
  toSend.push_back(packet);
}

Packet *Client::getPacketToSend()
{
  Packet *pck = toSend.front();
  if (!pck)
    return (NULL);
  toSend.erase(toSend.begin());
  return (pck);
}

bool  Client::isToSendEmpty() const
{
  return (toSend.empty());
}

int     Client::getFd() const
{
  return (fd);
}

std::string &Client::getIp()
{
  return (ip);
}

int Client::getPort() const
{
  return (port);
}

bool    Client::receiveData()
{
  int                   ret;
  std::vector<unsigned char> vec;
  Packet *pckt;
  char          buf[4096];

  memset(buf, 0, 4096);
  if ((ret = read(fd, buf, 4096)) <= 0)
    return (false);
std::cout << "rcv: " << ret << std::endl;
  vec = Packet::stringToStream(buf, ret);
  pckt = Packet::fromStream(vec);
  Identity *id = pckt->unpack<Identity>();
  comManager.whichInstruct(*id, *this);

  return (true);
}

void  Client::sendData()
{
  Packet *pck = getPacketToSend();

  if (!pck)
    return ;
  std::cout << "WRITE to" << fd << std::endl;
  std::vector<unsigned char>* stream = pck->build();
  unsigned int vectSize = stream->size();
  unsigned char data[vectSize];
  Packet::streamToString(stream, data);
  std::cout << "write value " << write(fd, data, vectSize) << std::endl;
}