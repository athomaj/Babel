
#ifdef _WIN32

#else
#include "Connection_Lin.h"

Connection_Lin::Connection_Lin(std::string const& username) :
  recording(false), write_server(false), isHost(false), _username(username), client_fd(-1), streamStatus(paContinue)
{
  FD_ZERO(&fd_write);
  FD_ZERO(&fd_read);
}

Connection_Lin::~Connection_Lin()
{
}

void Connection_Lin::setGui(IGui *gui)
{
	_gui = gui;
}


PortAudio*    Connection_Lin::getAudio()
{
    return (audio);
}

void    Connection_Lin::setAudio(PortAudio *aud)
{
    audio = aud;
}

void	Connection_Lin::write_to_server(Packet *package, Instruct inst)
{
  ManagePacket newPacket(package, actions.getAction(inst));
  queuSendServer.push_back(newPacket);
}

void	Connection_Lin::write_fd()
{

}

void	Connection_Lin::check_write_fd()
{
  if (client_fd != -1 && FD_ISSET(client_fd, &fd_write))
    {
      if (addr_size != 0 && isHost == true)
    	{
//          std::cout << "writing sound packet as host" << std::endl;
          t_soundPacket packet;

    	  if (isRecVectEmpty() == false)
            {
              packet = getRecordPacket();
    	      sendto(client_fd, &packet, 488, 0,  reinterpret_cast<struct sockaddr *>
    		     (&serverStorage), addr_size);
    	    }
    	}
      else if (isHost == false)
    	{
    	  t_soundPacket packet;
    	  if (isRecVectEmpty() == false)
    	    {
    	      packet = getRecordPacket();
    	      write(client_fd, &packet, 488);
    	      //sendto(client_fd, &packet, 488, 0,  reinterpret_cast<struct sockaddr *>
    	      //	     (&s_in), sizeof(s_in));

    	    }
    	}
    }
  if (FD_ISSET(this->fd, &fd_write))
    {
        if (queuSendServer.empty() == false)
	      {
//            std::cout << "try writing" << std::endl;
            std::vector<ManagePacket>::iterator it = queuSendServer.begin();
          if (it->getSent() == true)
            return ;
//          std::cout << "writing" << std::endl;
          it->setSent(true);
          std::vector<unsigned char>* stream = it->getPacket()->build();
          unsigned int vectSize = stream->size();
          unsigned char data[vectSize];
          Packet::streamToString(stream, data);
      		write(this->fd, data, vectSize);
//          queuSendServer.erase(it);
      		write_server = true;
	      }
    }
}

std::list<Packet*>	Connection_Lin::getPacket() const
{
  return (queuPacket);
}


void	Connection_Lin::setPacket(std::list<Packet*> qP)
{
  this->queuPacket = qP;
}

void    Connection_Lin::read_fd()
{
  int	ret;
  t_soundPacket packet;
  char	buff[4096];

  memset(buff, 0, 4095);
  if (client_fd != -1 && FD_ISSET(client_fd, &fd_read))
    {
      if (recording == false)
      {
          streamStatus = paContinue;
          audio->openRecordStream();
          audio->openPlayStream();
          audio->recordSound();
          audio->playSound();
      }
      actFptr fptr = actions.getAction(CALLESTABLISHED);
      (_gui->*fptr)(NULL);

      addr_size = sizeof(serverStorage);
      ret = recvfrom(client_fd, &packet, 488, 0, reinterpret_cast<struct sockaddr *>
	       (&serverStorage), &addr_size);
      addSoundPacket(packet.retenc, packet.data, getPlayPackets());
      recording = true;
    }
  if (FD_ISSET(this->fd, &fd_read))
    {
      if ((ret = read(this->fd, buff, 4095)) < 0)
      {
          std::cout << "fail reading" << std::endl;
          return ;
      }
      if (ret == 0)
      {
          close(fd);
          exit(0);
      }
      std::vector<unsigned char> vec = Packet::stringToStream(buff, ret);
      Packet* pckt = Packet::fromStream(vec);

      Identity *id = pckt->unpack<Identity>();
      std::cout << "instruct " << id->getInstruct() << std::endl;
      if (id && id->getInstruct() == ADDCONTACT)
      {
          std::cout << "Reading for contacts" << std::endl;
        ManagePacket newPacket(pckt, actions.getAction(ADDCONTACT));
        newPacket.execTAction(_gui);
      }
      else if (id && id->getInstruct() == DELCONTACT)
      {
          ManagePacket newPacket(pckt, actions.getAction(DELCONTACT));
          newPacket.execTAction(_gui);
      }
      else if (id && id->getInstruct() == ASKCALL)
      {
          ManagePacket newPacket(pckt, actions.getAction(ASKCALL));
          newPacket.execTAction(_gui);
      }
      else if (id && id->getInstruct() == ENDCALL)
      {
          ManagePacket newPacket(pckt, actions.getAction(ENDCALL));
          newPacket.execTAction(_gui);
      }
      else
          addResponse(pckt);
      // queuRcvServer.push_back(pckt);
    }
  // if (write_server == true) /* traite les packets 1 par 1 (systeme de ping pong) */
  //   {
  //     std::list<Packet*>::iterator it = queuRcvServer.begin();
  //     if (it != queuRcvServer.end())
  //   	{
  //   	  queuPacket.push_back(*it);
  //   	  queuRcvServer.erase(it);
  //   	  write_server = false;
  //   	}
  //   }
}

void    Connection_Lin::addResponse(Packet *pck)
{
  std::vector<ManagePacket>::iterator it = queuSendServer.begin();
  it->addResponse(pck);
}

void    Connection_Lin::set_fd()
{
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
    if (client_fd != -1)
    {
        FD_SET(client_fd, &fd_write);
        FD_SET(client_fd, &fd_read);
    }
  FD_SET(fd, &fd_read);
  if (!queuSendServer.empty())
    FD_SET(fd, &fd_write);
}

void Connection_Lin::resetClientFd()
{
    recording = false;
    close(client_fd);
    client_fd = -1;
    isHost = false;
    streamStatus = paComplete;
//    usleep(1000);
//    audio->closePlaystream();
//    audio->closeRecordstream();
    playSoundPackets.clear();
    recordSoundPackets.clear();
}

bool Connection_Lin::connection(int port, const std::string & ip)
{
  struct sockaddr_in sin;
  in_addr_t ip_convert;

  this->fd = socket(AF_INET, SOCK_STREAM, 0);
  if (INADDR_NONE == (ip_convert = inet_addr(ip.c_str())))
    return (false);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = ip_convert;
  if (connect(this->fd,
	      (const struct sockaddr *)&sin,
	      sizeof(sin)) == -1)
    return (false);
  std::cout << getIpAddr() << std::endl;
  Identity id(_username, getIpAddr(), 4545, CONNECTION);
  Packet *t = new Packet(id);
  write_to_server(t, CONNECTION);
  return (true);
}

int Connection_Lin::getStreamStatus() const
{
    return (streamStatus);
}

bool Connection_Lin::connectionHost(int port, const std::string & ipt)
{
  struct protoent       *pe;
  struct sockaddr_in    s_in;

  std::cout << "trying to connect" << std::endl;
  streamStatus = paContinue;
  audio->openRecordStream();
  audio->openPlayStream();
  audio->recordSound();
  audio->playSound();
  if ((pe = getprotobyname("UDP")) == NULL)
    {
      throw(std::runtime_error("getprotobyname failed"));
    }
  recording = true;
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(port);
  s_in.sin_addr.s_addr = inet_addr(ipt.c_str());
  std::cout << ipt.c_str() << std::endl;
  if ((client_fd = socket(AF_INET, SOCK_DGRAM, pe->p_proto)) == -1)
    {
      //perror("socket");
      throw(std::runtime_error("socket failed"));
    }
  if (connect(client_fd, (const struct sockaddr *)&s_in, sizeof(s_in)) == -1)
    {
      //perror("connect");
      throw(std::runtime_error("connect failed"));
    }
  playSoundPackets.clear();
  std::cout << "connection worked" << std::endl;
  return (true);
}

int  Connection_Lin::checkResponses()
{
  std::vector<ManagePacket>::iterator it = queuSendServer.begin();
  if (it != queuSendServer.end() && it->getResponse())
  {
      if (it->isResponseOk())
      {
          it->execTAction(_gui);
          queuSendServer.erase(it);
      }
      else
      {
          std::cout << "die" << std::endl;
          close(fd);
          queuSendServer.erase(it);
          return (-1);
      }
  }
  return (0);
}

int Connection_Lin::getFd() const
{
    return (fd);
}

int Connection_Lin::handle_connection()
{
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
  set_fd();
  struct timeval timeout;


  timeout.tv_sec = 0;
  timeout.tv_usec = 1000;
  if (select(500, &fd_read, &fd_write, NULL, &timeout) == -1)
    {
      perror("select");
      std::cout << "fail select"  << std::endl;
      FD_ZERO(&fd_write);
      FD_ZERO(&fd_read);
      ::close(fd);
      return (-1);
    }
  else
  {
      read_fd();
      check_write_fd();
      if (checkResponses() == -1)
      {
          ::close(fd);
          return (-1);
      }
  }
  usleep(10000);
  return (0);
}

int	Connection_Lin::create_server(int port)
{
  struct sockaddr_in    sin;
  struct protoent	*pe;

  std::cout << "creating udp server" << std::endl;
  if ((pe = getprotobyname("UDP")) == NULL)
      throw(std::runtime_error("getprotobyname failed"));
  isHost = true;
  if ((client_fd = socket(PF_INET, SOCK_DGRAM, pe->p_proto)) == -1)
    return (-1);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  if (bind(client_fd, (struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
      std::cout << "error bind" << std::endl;
      close(client_fd);
      return (-1);
    }
  std::cout << "udp server created" << std::endl;
  return (1);
}

bool Connection_Lin::Close(int port)
{
  (void)port;
	return (true);
}

void  Connection_Lin::addSoundPacket(const int retenc, unsigned char* data, std::vector<t_soundPacket>& soundPackets)
{
  t_soundPacket soundPacket;

  soundPacket.retenc = retenc;
  soundPacket.dataSize = 480;
  memcpy(soundPacket.data, data, soundPacket.dataSize);
  soundPackets.push_back(soundPacket);
}

bool      Connection_Lin::isPlayVectEmpty()
{
  if (playSoundPackets.size() > 0)
    return (false);
  return (true);
}

bool      Connection_Lin::isRecVectEmpty()
{
  if (recordSoundPackets.size() > 0)
    return (false);
  return (true);
}

t_soundPacket Connection_Lin::getPlayPacket()
{
  t_soundPacket packet = playSoundPackets.front();
  playSoundPackets.erase(playSoundPackets.begin());

  return (packet);
}

t_soundPacket Connection_Lin::getRecordPacket()
{
  t_soundPacket packet = recordSoundPackets.front();
  recordSoundPackets.erase(recordSoundPackets.begin());

  return (packet);
}

std::vector<t_soundPacket>& Connection_Lin::getRecordPackets()
{
  return (recordSoundPackets);
}

std::vector<t_soundPacket>& Connection_Lin::getPlayPackets()
{
  return (playSoundPackets);
}

bool                     Connection_Lin::getRecording() const
{
  return (recording);
}

#endif
