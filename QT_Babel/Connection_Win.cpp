#ifdef _WIN32
# include "Connection_Win.h"

Connection_Win::Connection_Win(std::string const& username) :
	recording(false), write_server(false), isHost(false), _username(username), client_fd(-1), streamStatus(paContinue)
{
	FD_ZERO(&fd_write);
	FD_ZERO(&fd_read);
}

Connection_Win::~Connection_Win()
{
}

PortAudio*    Connection_Win::getAudio()
{
	return (audio);
}

void    Connection_Win::setAudio(PortAudio *aud)
{
	audio = aud;
}

void	Connection_Win::write_to_server(Packet *package, Instruct inst)
{
	ManagePacket newPacket(package, actions.getAction(inst));
	queuSendServer.push_back(newPacket);
}

void	Connection_Win::write_fd()
{

}

void	Connection_Win::check_write_fd()
{
	if (client_fd != -1 && FD_ISSET(client_fd, &fd_write))
	{
		if (addr_size != 0 && isHost == true)
		{
			//          std::cout << "writing sound packet as host" << std::endl;
			t_soundPacket packet;

			if (isRecVectEmpty() == false)
			{
				std::cout << "sending" << std::endl;
				packet = getRecordPacket();
				sendto(client_fd, reinterpret_cast<char *>(&packet), 488, 0, reinterpret_cast<struct sockaddr *>
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
			std::vector<ManagePacket>::iterator it = queuSendServer.begin();
			if (it->getSent() == true)
				return;
			std::cout << "writing" << std::endl;
			it->setSent(true);
			std::vector<unsigned char>* stream = it->getPacket()->build();
			unsigned int vectSize = stream->size();
			unsigned char *data = static_cast<unsigned char *>(malloc(vectSize));
			Packet::streamToString(stream, data);
			write(this->fd, data, vectSize);
			//          queuSendServer.erase(it);
			write_server = true;
		}
	}
}

std::list<Packet*>	Connection_Win::getPacket() const
{
	return (queuPacket);
}


void	Connection_Win::setPacket(std::list<Packet*> qP)
{
	this->queuPacket = qP;
}

void    Connection_Win::read_fd()
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
		ret = recvfrom(client_fd, reinterpret_cast<char *>(&packet), 488, 0, reinterpret_cast<struct sockaddr *>
			(&serverStorage), &addr_size);
		addSoundPacket(packet.retenc, packet.data, getPlayPackets());
		recording = true;
	}
	if (FD_ISSET(this->fd, &fd_read))
	{
		std::cout << "reading" << std::endl;
		if ((ret = read(this->fd, buff, 4095)) < 0)
			return;
		if (ret == 0)
			close(fd);
		std::cout << "red: " << ret << std::endl;
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

void    Connection_Win::addResponse(Packet *pck)
{
	std::vector<ManagePacket>::iterator it = queuSendServer.begin();
	it->addResponse(pck);
}

void    Connection_Win::set_fd()
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

void Connection_Win::resetClientFd()
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

bool Connection_Win::connection(int port, const std::string & ip)
{
	WSADATA wsaData;
	SOCKET fd = INVALID_SOCKET;
	struct addrinfo *result = NULL, hints;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed with error:" << iResult << std::endl;
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo("127.0.0.1", "4242", &hints, &result);
	if (iResult != 0)
	{
		std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}
	fd = socket(result->ai_family, result->ai_socktype,
		result->ai_protocol);
	if (fd == INVALID_SOCKET) {
		std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	iResult = connect(fd, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(fd);
		fd = INVALID_SOCKET;
	}
	freeaddrinfo(result);
	if (fd == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
/*
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
	return (true);*/
}

int Connection_Win::getStreamStatus() const
{
	return (streamStatus);
}

bool Connection_Win::connectionHost(int port, const std::string & ipt)
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

void  Connection_Win::checkResponses()
{
	std::vector<ManagePacket>::iterator it = queuSendServer.begin();

	if (it != queuSendServer.end() && it->getResponse() && it->isResponseOk())
	{
		it->execTAction(_gui);
		queuSendServer.erase(it);
	}
}

int Connection_Win::getFd() const
{
	return (fd);
}

int Connection_Win::handle_connection()
{
	set_fd();
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;
	if (select(500, &fd_read, &fd_write, NULL, &timeout) == -1)
	{
		perror("select");
		std::cout << "fail select" << std::endl;
		FD_ZERO(&fd_write);
		FD_ZERO(&fd_read);
		return (-1);
	}
	else
	{
		read_fd();
		FD_ZERO(&fd_read);
		check_write_fd();
		FD_ZERO(&fd_write);
		checkResponses();
	}
	Sleep(100);
	return (0);
}

int	Connection_Win::create_server(int port)
{
	struct sockaddr_in    sin;
	struct protoent	*pe;

	std::cout << "creating udp server" << std::endl;
	if ((pe = getprotobyname("UDP")) == NULL)
	{
		//      perror("getprotobyname");
		throw(std::runtime_error("getprotobyname failed"));
	}
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

bool Connection_Win::Close(int port)
{
	(void)port;
	return (true);
}

void  Connection_Win::addSoundPacket(const int retenc, unsigned char* data, std::vector<t_soundPacket>& soundPackets)
{
	t_soundPacket soundPacket;

	soundPacket.retenc = retenc;
	soundPacket.dataSize = 480;
	memcpy(soundPacket.data, data, soundPacket.dataSize);
	soundPackets.push_back(soundPacket);
}

bool      Connection_Win::isPlayVectEmpty()
{
	if (playSoundPackets.size() > 0)
		return (false);
	return (true);
}

bool      Connection_Win::isRecVectEmpty()
{
	if (recordSoundPackets.size() > 0)
		return (false);
	return (true);
}

t_soundPacket Connection_Win::getPlayPacket()
{
	t_soundPacket packet = playSoundPackets.front();
	playSoundPackets.erase(playSoundPackets.begin());

	return (packet);
}

t_soundPacket Connection_Win::getRecordPacket()
{
	t_soundPacket packet = recordSoundPackets.front();
	recordSoundPackets.erase(recordSoundPackets.begin());

	return (packet);
}

std::vector<t_soundPacket>& Connection_Win::getRecordPackets()
{
	return (recordSoundPackets);
}

std::vector<t_soundPacket>& Connection_Win::getPlayPackets()
{
	return (playSoundPackets);
}

bool                     Connection_Win::getRecording() const
{
	return (recording);
}

#endif
