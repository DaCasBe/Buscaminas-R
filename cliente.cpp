#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "funcionesCliente.hpp"

int main(){
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
	sockname.sin_addr.s_addr=inet_addr("127.0.0.1");

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

	do{ //Bucle del cliente
		auxfds=readfds;
		
		salida=select(sd+1,&auxfds,NULL,NULL,NULL); //Esperamos a recibir mensajes del servidor
        
		if(FD_ISSET(sd,&auxfds)){ //Hay un mensaje del servidor
			//Se recibe el mensaje
			bzero(buffer,sizeof(buffer));
			recv(sd,buffer,sizeof(buffer),0);
			
			if(strstr(buffer,"+Ok")!=NULL or strstr(buffer,"-Err")!=NULL){ //Se ha recibido ningun mensaje convencional
				if(strcmp(buffer,"-Err. Demasiados clientes conectados\n")==0 or strcmp(buffer,"-Err. Desconexion servidor\n")==0){ //Se ha recibido alguno de los mensajes especificados
					fin=1;
				}
				
				else{
					//Se recibe el mensaje del servidor
					printf("\n%s\n",buffer);
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
