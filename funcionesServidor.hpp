#ifndef FUNCIONESSERVIDOR_HPP
#define FUNCIONESSERVIDOR_HPP

#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include "usuario.hpp"
#include "partida.hpp"

#define MSG_SIZE 250

void manejador(int signum);
void salirCliente(int socket,fd_set * readfds,std::vector <Usuario> &usuarios);
std::vector <std::string> dividirCadena(std::string cadena,std::string separador);
bool registro(std::string user,std::string password,int descriptor);
bool funcionUsuario(std::string usuario,int descriptor);
bool funcionPassword(std::string usuario,std::string password,int descriptor);
int indiceUsuario(int sd,std::vector <Usuario> usuarios);
int indicePartida(int descriptor, std::vector <Partida> partidas);
bool existeUsuario(std::string nombre,std::vector <Usuario> usuarios);

#endif
