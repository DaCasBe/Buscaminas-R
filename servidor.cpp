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
#include "funciones.hpp"


#define MSG_SIZE 250
#define MAX_CLIENTS 30


/*
 * El servidor ofrece el servicio de un chat
 */

int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];
    
    int on, ret;
    std::vector <Usuario> usuarios;

    
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}
    
    // Activaremos una propiedad del socket que permitir· que otros
    // sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    // Esto permitir· en protocolos como el TCP, poder ejecutar un
    // mismo programa varias veces seguidas y enlazarlo siempre al
    // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    // quedase disponible (TIME_WAIT en el caso de TCP)
    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2050);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}
	

   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
		from_len = sizeof (from);


		if(listen(sd,1) == -1){
			perror("Error en la operación de listen");
			exit(1);
		}
    
    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);
    
   	
    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
                
                
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        
                        if( i == sd){
                            
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                if((int)usuarios.size() < MAX_CLIENTS){//aqui aceptamos los clientes
				    Usuario usuario;
				    usuario.setDescriptor(new_sd);
				    usuarios.push_back(usuario);
				    
                                    FD_SET(new_sd,&readfds);
                                
                                    strcpy(buffer, "+Ok. Usuario conectado\n");
                                
                                    send(new_sd,buffer,strlen(buffer),0);
                                
            
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Demasiados clientes conectados\n");
                                    send(new_sd,buffer,strlen(buffer),0);
                                    close(new_sd);
                                }
                                
                            }
                            
                            
                        }
                        else if (i == 0){//Ya hemos entrado en el servidor
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer));// vaciamos el buffer
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0){
                             
                                for (j = 0; j < usuarios.size(); j++){
                                    send(usuarios[j].getDescriptor(), "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                                    close(usuarios[j].getDescriptor());
                                    FD_CLR(usuarios[j].getDescriptor(),&readfds);
                                }
                                
                                    usuarios.clear();
                                    close(sd);
                                    
                                    exit(-1);
                                
                                
                            }
                            //Mensajes que se quieran mandar a los clientes (implementar)
                            
                        } 
                        else{
                            bzero(buffer,sizeof(buffer));
                            
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            
                            if(recibidos > 0){
                                
                                if(strcmp(buffer,"SALIR\n") == 0){
                                    
                                    salirCliente(i,&readfds,usuarios);
                                    
                                }
                                else{


                                    std::vector <std::string> division=dividirCadena(std::string(buffer), " ");
                                    if(division[0]=="REGISTRO"){
                                        if(division.size() == 5 && division[1]=="-u" && division[3]=="-p"){
                                            if(registro(division[2],division[4],i)){
                                            	bzero(buffer,sizeof(buffer));
    						sprintf(buffer,"+Ok. Ha sido registrado correctamente\n");
    						send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                        else{
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer,"-Err. Formato incorrecto, usuario o contraseña nulos\n");
                                            send(i,buffer,sizeof(buffer),0);
                                        }
                                    }

                                    else if(division[0]=="USUARIO"){
                                    	if(buscarUsuario(i,usuarios).getEstado()!=CONECTADO){
                                    		bzero(buffer,sizeof(buffer));
                                            	sprintf(buffer,"-Err. No esta conectado\n");
                                            	send(i,buffer,sizeof(buffer),0);
                                    	}
                                    	
                                    	else{
                                    		if(division.size()==2){ // si ha introducido el usuario buscamos que esta registrado
                                    			if(funcionUsuario(division[1],i)){
								bzero(buffer,sizeof(buffer));
								sprintf(buffer,"+Ok. Usuario correcto\n");
								send(i,buffer,sizeof(buffer),0);
                                    			}
                                    		}
                                    		else{ // el usuario no ha introducido un usuario
							bzero(buffer,sizeof(buffer));
                                            		sprintf(buffer,"-Err. Formato incorrecto, no ha introducido un usuario\n");
                                            		send(i,buffer,sizeof(buffer),0);
                                    		}
                                    	}
                                    	
                                    	
                                    }
                                    
                                    else{
                                    	bzero(buffer,sizeof(buffer));
                                        sprintf(buffer,"-Err. No ha usado ninguna opcion disponible\n");
                                        send(i,buffer,sizeof(buffer),0);
                                    }
                                    
                                    sprintf(identificador,"%d: %s",i,buffer);
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,identificador);
                                                                                                                                                                                                                                                        
                                    for(j=0; j<usuarios.size(); j++)
                                        if(usuarios[j].getDescriptor() != i)
                                            send(usuarios[j].getDescriptor(),buffer,strlen(buffer),0);

                                    
                                }
                                                                
                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,usuarios);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}
