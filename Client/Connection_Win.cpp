#include <iostream>
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Connection_Win.h"

Connection_Win::Connection_Win(std::string const& username) :
recording(false), write_server(false), isHost(false), _username(username), client_fd(-1), streamStatus(paContinue)
{
	FD_ZERO(&fd_write);
	FD_ZERO(&fd_read);
}

Connection_Win::~Connection_Win()
{
}

void Connection_Win::setGui(IGui *gui)
{
	_gui = gui;
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
			t_soundPacket packet;

			if (isRecVectEmpty() == false)
			{
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
				send(client_fd, reinterpret_cast<char *>(&packet), 488, 0);
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
			unsigned char* data = static_cast<unsigned char*>(malloc(vectSize));
			Packet::streamToString(stream, data);
			send(this->fd, reinterpret_cast<char *>(data), vectSize, 0);
//			write(this->fd, data, vectSize);
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
		std::cout << "ret" << ret << std::endl;
		addSoundPacket(packet.retenc, packet.data, getPlayPackets());
		recording = true;
	}
	if (FD_ISSET(this->fd, &fd_read))
	{
		std::cout << "reading" << std::endl;
		if ((ret = recv(this->fd, buff, 4095, 0)) < 0)
			return;
		if (ret == 0)
        {
            closesocket(fd);
            exit(0);
        }
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
	}

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
	closesocket(client_fd);
	client_fd = -1;
	isHost = false;
	streamStatus = paComplete;
	//    usleep(1000);
	//    audio->closePlaystream();
	//    audio->closeRecordstream();
	playSoundPackets.clear();
	recordSoundPackets.clear();
}

int Connection_Win::getStreamStatus() const
{
	return (streamStatus);
}

bool Connection_Win::connectionHost(int port, const std::string & ipt)
{
	struct protoent       *pe;
	struct sockaddr_in    s_in;
	WSADATA wsa;

	std::cout << "trying to connect" << std::endl;
	streamStatus = paContinue;
	audio->openRecordStream();
	audio->openPlayStream();
	audio->recordSound();
	audio->playSound();

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	if ((pe = getprotobyname("UDP")) == NULL)
	{
		throw(std::runtime_error("getprotobyname failed"));
	}
	recording = true;
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(port);
	s_in.sin_addr.s_addr = inet_addr(ipt.c_str());
	if ((client_fd = socket(AF_INET, SOCK_DGRAM, pe->p_proto)) == -1)
	{
		throw(std::runtime_error("socket failed"));
	}
	if (connect(client_fd, (const struct sockaddr *)&s_in, sizeof(s_in)) == -1)
	{
		throw(std::runtime_error("connect failed"));
	}
	playSoundPackets.clear();
	std::cout << "connection worked" << std::endl;
	return (true);
}

int  Connection_Win::checkResponses()
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
			closesocket(fd);
			queuSendServer.erase(it);
			return (-1);
		}
	}
	return (0);
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
		closesocket(fd);
		return (-1);
	}
	else
	{
		read_fd();
		FD_ZERO(&fd_read);
		check_write_fd();
		FD_ZERO(&fd_write);
		if (checkResponses() == -1)
		{
			closesocket(fd);
			return (-1);
		}
	}
	return (0);
}

int	Connection_Win::create_server(int port)
{
	struct sockaddr_in    sin;
	struct protoent	*pe;
	WSADATA wsa;

	std::cout << "creating udp server" << std::endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
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
		closesocket(client_fd);
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


bool Connection_Win::connection(int port, const std::string & ip)
{
	WSADATA wsaData;
	fd = INVALID_SOCKET;
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
    iResult = getaddrinfo((PCSTR)ip.c_str(), "4242", &hints, &result);
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
	std::cout << "Connection Done" << std::endl;
	Identity id(_username, getIpAddr(), 4545, CONNECTION);
	Packet *t = new Packet(id);
	write_to_server(t, CONNECTION);
}

// void		 Connection_Win::add_user(int fd)
// {
// 	clients.push_back(fd);
// }

/*
int						Connection_Win::create_server(int port)
{
//	int					s;
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;


	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, "4244", &hints, &result);
	if (iResult != 0) 
	{
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) 
	{
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	std::cout << "22" << std::endl;
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);
	std::cout << "h00" << std::endl;
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) 
	{
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	std::cout << "hello" << std::endl;
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) 
	{
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	std::cout << "hello" << std::endl;
	//	add_user(ClientSocket);
	return (0);
}
*/
/*
void	Connection_Win::read_fd()
{
	for (std::list<int>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		if (FD_ISSET(*i, &fd_read))
			std::cout << "pol" << std::endl;
	}
}
*/
/*
bool	Connection_Win::connectionHost(int, const std::string &)
{
  return (true);
}
*/
/*
void	Connection_Win::write_fd()
{
	std::string t("");

	for (std::list<int>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		if (FD_ISSET(*i, &fd_write))
			std::cout << "pp" << std::endl;
	}
}


void	Connection_Win::set_fd()
{
	for (std::list<int>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		//std::cout << "mm" << std::endl;
		if ((*i) > max)
			max = *i;
		//std::cout << "fd = " << *i << std::endl;
		FD_SET(*i, &fd_read);
	}
}

void	Connection_Win::handle_connection()
{
	//fd_set fd_read;
	//fd_set fd_write;

	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);
	//this->fd = 0;
	while (1)
	{
		max = this->fd;
		//FD_SET(this->fd, &fd_read);
		set_fd();
		//std::cout << "azea" << this->fd <<  " " << this->max << std::endl;
		//FD_SET(0, &fd_write);
		if (select(this->max + 1, &(fd_read), NULL, NULL, NULL) == -1)
		{
			std::cout << "zaezae"  << std::endl;
			return ;
		}
		//std::cout << "zae" << std::endl;
		if (FD_ISSET(this->fd, &fd_read) != 0)
		  ;
		  //			receive_message();
		read_fd();
		write_fd();
		//if (FD_ISSET(0, &fd_read) != 0)
			//send_message(std::string("aze"));
		return ;
	}
}

// bool Connection_Win::send_message(std::string & message)
// {
// 	std::cout << "zaea" << std::endl;
// 	//_write(1, message.c_str(), strlen(message.c_str()));
// 	return (true);
// }

// std::string Connection_Win::receive_message()
// {
// 	std::string message("");
// 	std::cout << "dg" << std::endl;
// 	//_write(1, message.c_str(), strlen(message.c_str()));
// 	return (message);
// }

bool Connection_Win::Close(int port)
{
	return (true);
}
*/
#endif
