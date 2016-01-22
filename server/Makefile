##
## Makefile for  in /home/maillo_a/rendu/cpp_spider/server
## 
## Made by Flavien Maillot
## Login   <maillo_a@epitech.net>
## 
## Started on  Fri Oct 30 18:24:35 2015 Flavien Maillot
## Last update Fri Oct 30 18:26:51 2015 Flavien Maillot
##

SRC=		Server.cpp				\
		Client.cpp					\
		main.cpp 					\
		../Client/Packet.cpp 	 	\
		../Client/Rsa.cpp			\
		../Client/BBException.cpp	\
		ManageCommand.cpp 			\
		../Client/soundpacket.cpp 	\


OBJ=		$(SRC:.cpp=.o)

NAME=		server

CXXFLAGS=	-I./include -std=c++11

CXXFLAGS=	-I/usr/local/include -I/usr/local/include/opus/ -I/usr/local/ssl/include -I../Client/

LIB=		-lportaudio  -lopus -lssl -lcrypto -L/usr/local/lib -L/usr/lib


all:		$(NAME)

$(NAME):	$(OBJ)
		g++ $(OBJ) -o $(NAME) $(LIB)

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(NAME)

re:		fclean all

