#include <stdlib.h>
#include "funcionesServidor.hpp"
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
	std::vector <Usuario> espera;
	std::vector <Partida> partidas;
    
	//Se abre el socket
  	sd=socket(AF_INET,SOCK_STREAM,0);
  	
	if(sd==-1){ //No se ha abierto el socket
		perror("No se puede abrir el socket cliente\n");
		
    		exit(1);	
	}
    
	/*Activaremos una propiedad del socket que permitir que otros sockets puedan reutilizar cualquier puerto al que nos enlacemos.
	Esto permitir en protocolos como el TCP, poder ejecutar un mismo programa varias veces seguidas y enlazarlo siempre al
	mismo puerto. De lo contrario habrÌa que esperar a que el puerto quedase disponible (TIME_WAIT en el caso de TCP)*/
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
								strcpy(buffer,"-Err. Demasiados clientes conectados\n");
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
								send(usuarios[j].getDescriptor(),"-Err. Desconexion servidor\n",strlen("-Err. Desconexion servidor\n"),0);
								
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

								if(usuarios[indiceUsuario(i,usuarios)].getEstado()==PARTIDA){
									if(partidas[indicePartida(i,partidas)].buscarUsuario(i)==1){ //Quiere salir el jugador 1
										usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario2()->getDescriptor(),usuarios)].setEstado(LOGUEADO); //Se saca al jugador 2 de la partida
									}
								
									if(partidas[indicePartida(i,partidas)].buscarUsuario(i)==2){ //Quiere salir el jugador 2
										usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario1()->getDescriptor(),usuarios)].setEstado(LOGUEADO); //Se saca al jugador 1 de la partida
									}
								
									partidas[indicePartida(i,partidas)].setFin(true); //Termina la partida
								
									partidas.erase(partidas.begin()+indicePartida(i,partidas)); //Se borra la partida
								}
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
									
									else if(existeUsuario(division[1],usuarios)){ //Ya hay alguien con su mismo nombre
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Ya hay un usuario conectado con su mismo nombre\n");
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
												usuarios[indiceUsuario(i,usuarios)].setEstado(ESPERA); //Se cambia el estado del usuario a ESPERA
												espera.push_back(usuarios[indiceUsuario(i,usuarios)]); //Se introduce al usuario en la cola de espera
												
												if(espera.size()>=2 && partidas.size()<MAX_MATCHES){ //Hay jugadores suficientes en la cola de espera como para crear una partida y no se ha llegado al maximo de partidas simultaneas
													//Se crea una nueva partida
													Partida partida;
													
													usuarios[indiceUsuario(espera[0].getDescriptor(),usuarios)].setEstado(PARTIDA);
													partida.setUsuario1(espera[0]);
													espera.erase(espera.begin());
													
													usuarios[indiceUsuario(espera[0].getDescriptor(),usuarios)].setEstado(PARTIDA);
													partida.setUsuario2(espera[0]);
													espera.erase(espera.begin());
													
													partidas.push_back(partida);
													
													bzero(buffer,sizeof(buffer));
													sprintf(buffer,"+Ok. Conectado a la partida eres el jugador A, es tu turno\n");
													send(partida.getUsuario1()->getDescriptor(),buffer,sizeof(buffer),0);

													bzero(buffer,sizeof(buffer));
													sprintf(buffer,"+Ok. Conectado a la partida eres el jugador B, es el turno del jugador A\n");
													send(partida.getUsuario2()->getDescriptor(),buffer,sizeof(buffer),0);

													partida.enviarTablero(); //Se envia el tablero a los jugadores
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
								
								else if(division[0]=="DESCUBRIR"){ //Se ha recibido DESCUBRIR
									if(usuarios[indiceUsuario(i,usuarios)].getEstado()!=PARTIDA){ //El usuario no esta en partida
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Aun no esta en partida\n");
										send(i,buffer,sizeof(buffer),0);
									}
									
									else{ //El usuario esta en partida
										if(division.size()!=2){ //No se cumple el formato de DESCUBRIR
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-Err. Formato incorrecto\n");
											send(i,buffer,sizeof(buffer),0);
										}
										
										else{ //Se cumple el formato de DESCUBRIR
											division=dividirCadenaNumeros(division[1],",");

											if(division.size()!=2){ //No se cumple el formato de DESCUBRIR
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"-Err. Formato incorrecto\n");
												send(i,buffer,sizeof(buffer),0);
											}
										
											else{ //Se cumple el formato de DESCUBRIR
												if(division[0]>="A" and division[0]<="J"){ //La letra esta entre la A y la J
													if(atoi(division[1].c_str())>=1 and atoi(division[1].c_str())<=BRD_SIZE){ //El numero esta entre el 1 y el 10
														if(!partidas[indicePartida(i,partidas)].destaparCasillas(i,division[0],atoi(division[1].c_str()))){ //No se descubre la casilla especificada
															if(partidas[indicePartida(i,partidas)].getFin()){ //Se ha acabado la partida
																bzero(buffer,sizeof(buffer));
																sprintf(buffer,"+Ok. Fin de la partida\n");
																send(partidas[indicePartida(i,partidas)].getUsuario1()->getDescriptor(),buffer,sizeof(buffer),0);
																send(partidas[indicePartida(i,partidas)].getUsuario2()->getDescriptor(),buffer,sizeof(buffer),0);
															
																//Se saca a los usuarios de la partida
																usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario1()->getDescriptor(),usuarios)].setEstado(LOGUEADO);
																usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario2()->getDescriptor(),usuarios)].setEstado(LOGUEADO);
															
																partidas.erase(partidas.begin()+indicePartida(i,partidas)); //Se borra la partida
															}
														}
													}
												
													else{ //El numero no esta entre el 1 y el 10
														bzero(buffer,sizeof(buffer));
														sprintf(buffer,"-Err. El numero debe estar entre 0 y 9\n");
														send(i,buffer,sizeof(buffer),0);
													}

												}
											
												else{ //La letra no esta entre la A y la J
													bzero(buffer,sizeof(buffer));
													sprintf(buffer,"-Err. La letra debe estar entre A y J\n");
													send(i,buffer,sizeof(buffer),0);
												}
											}
										}
									}
								}
								
								else if(division[0]=="PONER-BANDERA"){ //Se ha recibido PONER-BANDERA
									if(usuarios[indiceUsuario(i,usuarios)].getEstado()!=PARTIDA){ //El usuario no esta en partida
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Aun no esta en partida\n");
										send(i,buffer,sizeof(buffer),0);
									}
									
									else{ //El usuario esta en partida
										if(division.size()!=2){ //No se cumple el formato de PONER-BANDERA
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-Err. Formato incorrecto\n");
											send(i,buffer,sizeof(buffer),0);
										}
										
										else{ //Se cumple el formato de PONER-BANDERA
											division=dividirCadenaNumeros(division[1],",");
										
											if(division.size()!=2){ //No se cumple el formato de PONER-BANDERA
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"-Err. Formato incorrecto\n");
												send(i,buffer,sizeof(buffer),0);
											}
										
											else{ //Se cumple el formato de PONER-BANDERA
												if(division[0]>="A" and division[0]<="J"){ //La letra esta entre la A y la J
													if(atoi(division[1].c_str())>=1 and atoi(division[1].c_str())<=BRD_SIZE){ //El numero esta entre el 1 y el 10
														if(!partidas[indicePartida(i,partidas)].ponerBandera(i,division[0],atoi(division[1].c_str()))){ //No se pone la bandera en la casilla especificada
															if(partidas[indicePartida(i,partidas)].getFin()){ //Se ha acabado la partida
																bzero(buffer,sizeof(buffer));
																sprintf(buffer,"+Ok. Fin de la partida\n");
																send(partidas[indicePartida(i,partidas)].getUsuario1()->getDescriptor(),buffer,sizeof(buffer),0);
																send(partidas[indicePartida(i,partidas)].getUsuario2()->getDescriptor(),buffer,sizeof(buffer),0);
															
																//Se saca a los usuarios de la partida
																usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario1()->getDescriptor(),usuarios)].setEstado(LOGUEADO);
																usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario2()->getDescriptor(),usuarios)].setEstado(LOGUEADO);
															
																partidas.erase(partidas.begin()+indicePartida(i,partidas)); //Se borra la partida
															}
													
														}
													}
												
													else{ //El numero no esta entre el 1 y el 10
														bzero(buffer,sizeof(buffer));
														sprintf(buffer,"-Err. El numero debe estar entre 0 y 9\n");
														send(i,buffer,sizeof(buffer),0);
													}
												}
											
												else{ //La letra no esta entre la A y la J
													bzero(buffer,sizeof(buffer));
													sprintf(buffer,"-Err. La letra debe estar entre A y J\n");
													send(i,buffer,sizeof(buffer),0);
												}
											}
										}
									}
								}
                                    
								else{ //No se ha recibido ningun comando valido
									bzero(buffer,sizeof(buffer));
									sprintf(buffer,"-Err. No ha usado ningun comando valido\n");
									send(i,buffer,sizeof(buffer),0);
								}
							}
						}
						
						if(recibidos==0){ //El cliente ha introducido ctrl+c
							if(usuarios[indiceUsuario(i,usuarios)].getEstado()==PARTIDA){
								if(partidas[indicePartida(i,partidas)].buscarUsuario(i)==1){ //Quiere salir el jugador 1
										usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario2()->getDescriptor(),usuarios)].setEstado(LOGUEADO); //Se saca al jugador 2 de la partida
									}
								
									if(partidas[indicePartida(i,partidas)].buscarUsuario(i)==2){ //Quiere salir el jugador 2
										usuarios[indiceUsuario(partidas[indicePartida(i,partidas)].getUsuario1()->getDescriptor(),usuarios)].setEstado(LOGUEADO); //Se saca al jugador 1 de la partida
									}
								
								partidas[indicePartida(i,partidas)].setFin(true); //Se termina la partida
								
								partidas.erase(partidas.begin()+indicePartida(i,partidas)); //Se borra la partida
							}
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
