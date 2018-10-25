#include <stdlib.h>
#include "funciones.hpp"
#include <queue>
#include "partida.hpp"
#include <iostream>

#define MSG_SIZE 250
#define MAX_CLIENTS 30
#define MAX_MATCHES 10 

int main(){
	int sd,new_sd;
	struct sockaddr_in sockname,from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
	fd_set readfds,auxfds;
	int salida;
	int i,j,k;
	int recibidos;
	char identificador[MSG_SIZE];
	int on,ret;
	std::vector <Usuario> usuarios;
	std::queue <Usuario> espera;
	std::vector <Partida> partidas;
    
	//Se abre el socket
  	sd=socket(AF_INET,SOCK_STREAM,0);
  	
	if(sd==-1){ //No se ha abierto el socket
		perror("No se puede abrir el socket cliente\n");
		
    		exit(1);	
	}
    
	/*Activaremos una propiedad del socket que permitir que otros
	sockets puedan reutilizar cualquier puerto al que nos enlacemos.
	Esto permitir en protocolos como el TCP, poder ejecutar un
	mismo programa varias veces seguidas y enlazarlo siempre al
	mismo puerto. De lo contrario habrÌa que esperar a que el puerto
	quedase disponible (TIME_WAIT en el caso de TCP)*/
	on=1;
	ret=setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	//Almacenamos la informacion relacionada con el socket
	sockname.sin_family=AF_INET;
	sockname.sin_port=htons(2050);
	sockname.sin_addr.s_addr=INADDR_ANY;

	if(bind(sd,(struct sockaddr *)&sockname,sizeof(sockname))==-1){ //Asociamos el socket al puerto
		perror("Error en la operación bind");
		
		exit(1);
	}
	
	from_len=sizeof(from);

	if(listen(sd,1)==-1){ //Habilitamos el socket para que pueda recibir conexiones
		perror("Error en la operación de listen");
		
		exit(1);
	}
    
	//Inicializamos las estructuras
	FD_ZERO(&readfds);
	FD_ZERO(&auxfds);
	FD_SET(sd,&readfds);
	FD_SET(0,&readfds);
   	
	signal(SIGINT,manejador); //Capturamos la señal SIGINT (Ctrl+c)
    
	while(1){ //Bucle del servidor
		auxfds=readfds;
            
		salida=select(FD_SETSIZE,&auxfds,NULL,NULL,NULL); //Esperamos a recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
		if(salida>0){ //Se ha establecido comunicacion con un cliente
			for(i=0;i<FD_SETSIZE;i++){ //Buscamos el socket por el que se ha establecido la comunicación
				if(FD_ISSET(i,&auxfds)){ //Miramos si el descriptor se encuentra en el conjunto auxfds
					if(i==sd){ //Se ha encontrado al cliente
						if((new_sd=accept(sd,(struct sockaddr *)&from,&from_len))==-1){ //No se ha podido aceptar la peticion del cliente
							perror("Error aceptando peticiones");
						}
						
						else{ //Se ha podido aceptar la peticion del cliente
							if((int)usuarios.size()<MAX_CLIENTS){ //No se ha alcanzado el maximo de clientes
								//Introducimos al nuevo usuario en la lista de usuarios conectados
								Usuario usuario;
								usuario.setDescriptor(new_sd);
								usuarios.push_back(usuario);
				    
								FD_SET(new_sd,&readfds);
                                
								strcpy(buffer,"+Ok. Usuario conectado\n");
								send(new_sd,buffer,strlen(buffer),0);
							}
							
							else{ //Se ha alcanzado el maximo de clientes
								bzero(buffer,sizeof(buffer));
								strcpy(buffer,"Demasiados clientes conectados\n");
								send(new_sd,buffer,strlen(buffer),0);
								
								close(new_sd); //Se cierra el socket del cliente
							}
                                
						}
                            
                            
					}
					
					else if(i==0){ //El cliente ya ha entrado en el servidor
						//Se ha introducido información de teclado
						bzero(buffer,sizeof(buffer));
						fgets(buffer,sizeof(buffer),stdin);
                            
						if(strcmp(buffer,"SALIR\n")==0){ //Se ha introducido SALIR
							//Se desconecta a todos los clientes
							for(j=0;j<usuarios.size();j++){
								send(usuarios[j].getDescriptor(),"Desconexion servidor\n",strlen("Desconexion servidor\n"),0);
								
								//Se desconecta al usuario
								close(usuarios[j].getDescriptor());
								FD_CLR(usuarios[j].getDescriptor(),&readfds);
							}
                                
							usuarios.clear(); //Se limpia la lista de usuarios
							
							close(sd); //Se cierra el socket
                                    
							exit(-1);
						}
					}
					
					else{
						//Se recibe un mensaje
						bzero(buffer,sizeof(buffer));
						recibidos=recv(i,buffer,sizeof(buffer),0);
                            
						if(recibidos>0){ //Se ha recibido un mensaje
							if(strcmp(buffer,"SALIR\n")==0){ //Se ha recibido SALIR
								salirCliente(i,&readfds,usuarios); //Se desconecta al cliente
							}
							
							else{ //No se ha recibido SALIR
								std::vector <std::string> division=dividirCadena(std::string(buffer)," "); //Se divide la cadena recibida
								
								if(division[0]=="REGISTRO"){ //Se ha recibido REGISTRO
									if(division.size()==5 and division[1]=="-u" and division[3]=="-p"){ //Se cumple el formato de REGISTRO
										if(registro(division[2],division[4],i)){ //Se ha registrado al usuario
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"+Ok. Ha sido registrado correctamente\n");
											send(i,buffer,sizeof(buffer),0);
										}
									}
										
									else{ //No se cumple el formato de REGISTRO
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Formato incorrecto, usuario o contraseña nulos\n");
										send(i,buffer,sizeof(buffer),0);
									}
								}

								else if(division[0]=="USUARIO"){ //Se ha recibido USUARIO
 									if(usuarios[indiceUsuario(i,usuarios)].getEstado()!=CONECTADO){ //El usuario no esta en estado CONECTADO
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. No esta conectado\n");
										send(i,buffer,sizeof(buffer),0);
									}
                                    	
									else{ //El usuario esta en estado CONECTADO
										if(division.size()==2){ //Se cumple el formato de USUARIO
											if(funcionUsuario(division[1],i)){ //Se ha validado el nombre de usuario
												usuarios[indiceUsuario(i,usuarios)].setUsuario(division[1]); //Se almacena el nombre de usuario
												usuarios[indiceUsuario(i,usuarios)].setEstado(NOMBRE); //Se cambia el estado del usuario a NOMBRE
												
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"+Ok. Usuario correcto\n");
												send(i,buffer,sizeof(buffer),0);
											}
										}
											
										else{ //No se cumple el formato de USUARIO
											bzero(buffer,sizeof(buffer));
                                            						sprintf(buffer,"-Err. Formato incorrecto, no ha introducido un usuario\n");
                                            						send(i,buffer,sizeof(buffer),0);
										}
									}
								}
								
								else if(division[0]=="PASSWORD"){ //Se ha recibido PASSWORD
									if(usuarios[indiceUsuario(i,usuarios)].getEstado()!=NOMBRE){ //El usuario no ha introducido su nombre anteriormente
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Aun no ha introducido ningun nombre de usuario\n");
										send(i,buffer,sizeof(buffer),0);
									}
									
									else{ //El usuario ha introducido su nombre anteriormente
										if(division.size()==2){ //Se cumple el formato de PASSWORD
											if(funcionPassword(usuarios[indiceUsuario(i,usuarios)].getUsuario(),division[1],i)){ //Se ha validado al usuario
												usuarios[indiceUsuario(i,usuarios)].setPassword(division[1]); //Se almacena la password del usuario
												usuarios[indiceUsuario(i,usuarios)].setEstado(LOGUEADO); //Se cambia el estado del usuario a LOGUEADO
												
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"+Ok. Usuario validado\n");
												send(i,buffer,sizeof(buffer),0);
											}
										}
										
										else{ //No se ha validado al usuario
											bzero(buffer,sizeof(buffer));
                                            						sprintf(buffer,"-Err. Formato incorrecto, no ha introducido una password\n");
                                            						send(i,buffer,sizeof(buffer),0);
										}
									}
								}
								
								else if(division[0]=="INICIAR-PARTIDA"){ //Se ha recibido INICIAR-PARTIDA
									if(usuarios[indiceUsuario(i,usuarios)].getEstado()!=LOGUEADO){ //El usuario no esta logueado
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Aun no esta logueado\n");
										send(i,buffer,sizeof(buffer),0);
									}
									
									else{ //El usuario esta logueado
										if(division.size()!=1){ //No se cumple el formato de INICIAR-PARTIDA
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-Err. Formato incorrecto\n");
											send(i,buffer,sizeof(buffer),0);
										}
										
										else{ //Se cumple el formato de INICIAR-PARTIDA
											if(espera.size()<MAX_CLIENTS){ //No se ha alcanzado el maximo de clientes en cola
												espera.push(usuarios[indiceUsuario(i,usuarios)]); //Se introduce al usuario en la cola de espera
												
												espera.back().setEstado(ESPERA); //Se cambia el estado del usuario a ESPERA
												
												if(espera.size()>=2 && partidas.size() < MAX_MATCHES){ //Hay jugadores suficientes en la cola de espera como para crear una partida
													//Se extraen a los dos usuarios que van a jugar
													Usuario usuario1=espera.front();
													espera.pop();
													Usuario usuario2=espera.front();
													espera.pop();
													
													//Se cambia el estado de los usuarios a PARTIDA
													usuario1.setEstado(PARTIDA);
													usuario2.setEstado(PARTIDA);
													
													//Se crea una nueva partida
													Partida partida(usuario1,usuario2);
													partidas.push_back(partida);
												}
											}
											
											else{ //Se ha alcanzado el maximo de clientes en cola
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"-Err. Cola de espera llena\n");
												send(i,buffer,sizeof(buffer),0);
											}
										}
									}
								}
								else if(division[0]=="DESCUBRIR"){ // comando descubrir del juego
									if(usuarios[indiceUsuario(i,usuarios)].getEstado()!=PARTIDA){ //El usuario no esta logueado
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Aun no esta en partida\n");
										send(i,buffer,sizeof(buffer),0);

									}else{ // El usuario esta logueado

										division=dividirCadena(division[1],",");

										if(division.size()!=2){ //No se cumple el formato de DESCUBRIR la casilla
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-Err. Formato incorrecto\n");
											send(i,buffer,sizeof(buffer),0);
										}else{ // El comando esta bien escrito

											if(division[0]=="A" || division[0]=="B" || division[0]=="C" || division[0]=="D" || division[0]=="E" || division[0]=="F" || division[0]=="G" || division[0]=="H" || division[0]=="I" || division[0]=="J"){ // La letra que ha introducido es correcta
												if(division[1]=="0" || division[1]=="1" || division[1]=="2" || division[1]=="3" || division[1]=="4" || division[1]=="5" || division[1]=="6" || division[1]=="7" || division[1]=="8" || division[1]=="9"){ // La cifra que ha introducido es correcta
													//continuar desde aqui
													partidas[indicePartida(i,partidas)].destaparCasillas(i, atoi(division[0].c_str())-17 , atoi(division[1].c_str()));

												}else{// la cifra que ha introducido es incorrecta
													bzero(buffer,sizeof(buffer));
													sprintf(buffer,"-Err. Formato incorrecto cifra del 0 al 9\n");
													send(i,buffer,sizeof(buffer),0);
												}

											}else{ // la letra que ha introducido es incorrecta
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"-Err. Formato incorrecto letra de la A a la J\n");
												send(i,buffer,sizeof(buffer),0);
											}

										}
									}
								}
                                    
								else{ //No se ha recibido ningun comando valido
									bzero(buffer,sizeof(buffer));
									sprintf(buffer,"-Err. No ha usado ningun comando valido\n");
									send(i,buffer,sizeof(buffer),0);
								}

								//Se envia la informacion al resto de usuarios
								sprintf(identificador,"%d: %s",i,buffer);
								bzero(buffer,sizeof(buffer));
								strcpy(buffer,identificador);
                                                                                                                                                                                                                                                        
								for(j=0; j<usuarios.size(); j++){
									if(usuarios[j].getDescriptor() != i){
										send(usuarios[j].getDescriptor(),buffer,strlen(buffer),0); //Se envia la informacion al usuario
									}
								}
							}
						}
						
						if(recibidos==0){ //El cliente ha introducido ctrl+c
							printf("El socket %d, ha introducido ctrl+c\n", i);
							
							salirCliente(i,&readfds,usuarios); //Se desconecta al cliente
						}
					}
				}
			}
		}
	}

	close(sd); //Se cierra el socket
	
	return 0;
}
