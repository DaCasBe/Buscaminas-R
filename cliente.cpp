#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "funcionesCliente.hpp"
#include <iostream>

int main(int argc,char ** argv){
	if(argc!=2){ //La cantidad de argumentos no es correcta
		std::cout << "Error de sintaxis: ./cliente.exe <direccion IP>" << std::endl;
		
		exit(-1);
	}
	
	int sd;
	struct sockaddr_in sockname;
	char buffer[250];
	socklen_t len_sockname;
	fd_set readfds,auxfds;
	int salida;
	int fin=0;
	
	//Se abre el socket
	sd=socket(AF_INET, SOCK_STREAM,0);
	
	if(sd==-1){ //No se ha abierto el socket
		perror("No se puede abrir el socket cliente\n");
		
    		exit (1);	
	}
	
	//Almacenamos la informacion relacionada con el socket
	sockname.sin_family=AF_INET;
	sockname.sin_port=htons(2050);
	sockname.sin_addr.s_addr=inet_addr(argv[1]);

	len_sockname=sizeof(sockname);
	
	if(connect(sd,(struct sockaddr *)&sockname,len_sockname)==-1){ //Se solicita la conexion con el servidor
		perror("Error de conexión");
		
		exit(1);
	}
    
	//Inicializamos las estructuras
	FD_ZERO(&auxfds);
	FD_ZERO(&readfds);
	FD_SET(0,&readfds);
	FD_SET(sd,&readfds);
	
	std::cout << "Bienvenido a Buscaminas" << std::endl;
	std::cout << "Estos son los comandos que puedes usar: " << std::endl;
	std::cout << "    REGISTRO -u <nombre> -p <password>: registrarse en el juego" << std::endl;
	std::cout << "    USUARIO <nombre>: introducir un nombre de usuario" << std::endl;
	std::cout << "    PASSWORD <password>: introducir una password para loguearse" << std::endl;
	std::cout << "    INICIAR-PARTIDA: entrar en cola para esperar a poder jugar" << std::endl;
	std::cout << "    DESCUBRIR <letra>,<numero>: descubrir una casilla" << std::endl;
	std::cout << "    PONER-BANDERA <letra>,<numero>: indicar que hay una mina en una casilla" << std::endl;
	std::cout << "    SALIR: cerrar el programa" << std::endl << std::endl << std::endl << std::endl;

	do{ //Bucle del cliente
		auxfds=readfds;
		
		salida=select(sd+1,&auxfds,NULL,NULL,NULL); //Esperamos a recibir mensajes del servidor
        
		if(FD_ISSET(sd,&auxfds)){ //Hay un mensaje del servidor
			//Se recibe el mensaje
			bzero(buffer,sizeof(buffer));
			recv(sd,buffer,sizeof(buffer),0);
			
			if(strstr(buffer,"+Ok")!=NULL or strstr(buffer,"-Err")!=NULL){ //Se ha recibido ningun mensaje convencional
				printf("\n%s\n",buffer);
				
				if(strcmp(buffer,"-Err. Demasiados clientes conectados\n")==0 or strcmp(buffer,"-Err. Desconexion servidor\n")==0){ //Se ha recibido alguno de los mensajes especificados
					fin=1;
				}
			}
			
			else{ //Se ha recibido un tablero
				recibirTablero(buffer); //Se procesa el tablero recibido
			}
		}
		
		else{ //No hay ningun mensaje del servidor
			if(FD_ISSET(0,&auxfds)){ //Se ha introducido informacion por el teclado
				//Se recoge la informacion recibida por el teclado
				bzero(buffer,sizeof(buffer));
				fgets(buffer,sizeof(buffer),stdin);
                
				if(strcmp(buffer,"SALIR\n")==0){ //Se ha introducido SALIR
					fin=1;
				}
                
				send(sd,buffer,sizeof(buffer),0); //Se envia el mensaje al servidor
			}
		}		
	}while(fin==0);
		
	close(sd); //Se cierra el socket

	return 0;
}
