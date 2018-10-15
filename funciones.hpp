#ifndef FUNCIONES_HPP
#define FUNCIONES_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "usuario.hpp"

#define MSG_SIZE 250
#define MAX_CLIENTS 30

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds,std::vector <Usuario> &usuarios);
std::vector <std::string> dividirCadena(std::string cadena,std::string separador);
bool registro(std::string user, std::string password, int descriptor);
bool funcionUsuario(std::string usuario, int descriptor);
Usuario buscarUsuario(int sd,std::vector <Usuario> usuarios);

#endif
