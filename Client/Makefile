
SRC=	main.cpp			\
		portAudio.cpp		\
		EncodeManager.cpp	\
		Connection_Lin.cpp	\
		Packet.cpp		\
		Rsa.cpp			\
		BBException.cpp		\
		ManagePacket.cpp 	\
		Action.cpp 			\
		HandleConnection.cpp	\

OBJ=	$(SRC:.cpp=.o)

NAME=	testAudio

CXXFLAGS=	-W -Wall -Wextra -Werror -I/usr/local/include -I/usr/local/include/opus/ -I/usr/local/ssl/include

LIB=		-lportaudio  -lopus -lssl -lcrypto -L/usr/local/lib -L/usr/lib


all:	$(NAME)

$(NAME):	$(OBJ)
			g++ -o $(NAME) $(OBJ) $(LIB)

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME)

re:			fclean all
