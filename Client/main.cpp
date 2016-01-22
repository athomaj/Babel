#include <iostream>
#include <stdlib.h>
#include "portAudio.hh"
#include "HandleConnection.h"

#ifdef _WIN32
# include "Connection_Win.h"
#else
#include <iostream>
#include "Connection_Lin.h"
#endif

#define PORT 4242
#define IP "10.20.86.133"
#define IPS "127.0.0.1"


int		main(int ac, char **av)
{
	(void)ac;
	(void)av;
	IClient	*co = NULL;
	HandleConnection hc;


#ifdef _WIN32
	co = new Connection_Win();
	std::cout << "* Running in Windows env *" << std::endl;
#else
	co = new Connection_Lin();
	std::cout << "* Running in UNIX env *" << std::endl;
#endif
	if (co->connection(atoi(av[1]), IPS) == true)
		std::cout << "Worked" << std::endl;
	else
		std::cout << "blop" << std::endl;

	// PortAudio 	audio(co);
	// audio.initialize();
	// audio.initInputDevice();
	// audio.openRecordStream();
	// audio.initOutputDevice();
	// audio.openPlayStream();
	// audio.recordSound();
	// audio.playSound();
	hc.handle(co);
	//co->connectionHost(atoi(av[1]), IP);
	//co->handle_connection();
	//std::cout << "not good" << std::endl;
	//while (1);
	// if (ac == 2)
	//   {
	//     if (co->connection(atoi(av[1]), IP) == true)
	//       std::cout << "Worked" << std::endl;
	//     else
	//       {
	// 	std::cout << "blop" << std::endl;
	// 	return (1);
	//       }
	//     b = co;
	//     // co->create_server(atoi(av[1]));
	//     //std::cout << "not good" << std::endl;
	//     std::cout << "aeza" << std::endl;
	//     std::thread th1 (init_server);
	//     std::thread th2 (init_server);
	//     std::cout << "azeaze" << std::endl;
	//     th1.join();
	//     th2.join();
	//     //std::thread th2 (init_server);
	//     // while (1)
	//     //   {
	//     // 	read(0, buff, 500);
	//     // 	std::string b(buff);
	//     // 	co->write_to_server(b);
	//     //   }
	//   }
}
