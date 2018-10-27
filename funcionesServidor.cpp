#include "funciones.hpp"

void salirCliente(int socket,fd_set * readfds,std::vector <Usuario> &usuarios){
	char buffer[250];
	int j;
    
	close(socket); //Se cierra el socket
	FD_CLR(socket,readfds);

	//Se manda un mensaje al resto de clientes
	bzero(buffer,sizeof(buffer));
	sprintf(buffer,"-Err. Desconexión del cliente: %s\n",usuarios[indiceUsuario(socket,usuarios)].getUsuario().c_str());

	for(j=0;j<(int)usuarios.size();j++){
		if(usuarios[j].getDescriptor()!=socket){
			send(usuarios[j].getDescriptor(),buffer,strlen(buffer),0);
		}
	}

	usuarios.erase(usuarios.begin()+indiceUsuario(socket,usuarios));
}


void manejador(int signum){
	printf("\nSe ha recibido la señal sigint\n");
	signal(SIGINT,manejador);
    
	//Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}

std::vector <std::string> dividirCadena(std::string cadena,std::string separador){
	std::vector <std::string> dividido;
    
	while(cadena.find(separador)<=MSG_SIZE){ //Se busca el separador dentro de la cadena
		dividido.push_back(cadena.substr(0,cadena.find(separador))); //Se almacena la subcadena anterior al separador
		cadena.erase(0,cadena.find(separador)+1); //Se borran la subcadena almacenada y el separador de la cadena
	}

	dividido.push_back(cadena.substr(0,cadena.size()-1)); //Se almacena la ultima subcadena
    
	return dividido;
}

std::vector <std::string> dividirCadenaNumeros(std::string cadena,std::string separador){
	std::vector <std::string> dividido;
    
	while(cadena.find(separador)<=MSG_SIZE){ //Se busca el separador dentro de la cadena
		dividido.push_back(cadena.substr(0,cadena.find(separador))); //Se almacena la subcadena anterior al separador
		cadena.erase(0,cadena.find(separador)+1); //Se borran la subcadena almacenada y el separador de la cadena
	}

	dividido.push_back(cadena.substr(0,cadena.size())); //Se almacena la ultima subcadena
    
	return dividido;
}

bool registro(std::string user,std::string password,int descriptor){
	char buffer[MSG_SIZE];
	char useraux1[80],useraux2[80];
	char passwordaux1[80],passwordaux2[80];

	strcpy(useraux1,user.c_str());
	strcpy(passwordaux1,password.c_str());

	if(user.c_str()==NULL && password.c_str()==NULL){ //No se ha introducido el nombre de usuario o la password
		bzero(buffer,sizeof(buffer));
		sprintf(buffer,"-Err. No se introdujo el nombre de usuario o la password\n");
		send(descriptor,buffer,sizeof(buffer),0);
        
		return false;
	}

	std::ifstream leer;
	std::ofstream escribir;

	leer.open("usuarios.txt"); //Se abre el fichero de usuarios

	if(!leer){ //No se ha podido abrir el fichero de usuarios
		escribir.open("usuarios.txt", std::ios::out); //Se crea el fichero de usuarios
		
		escribir.close(); //Se cierra el fichero de usuarios
	}
	
	else{ //Se ha podido abrir el fichero de usuarios
		while(leer >> useraux2 >> passwordaux2){ //Se leen los nombres de usuario y las passwords del fichero de usuarios
			if(strcmp(useraux1,useraux2)==0){ //Se ha encontrado una coincidencia en el nombre de usuario
				bzero(buffer,sizeof(buffer));
				sprintf(buffer,"-Err. El usuario ya esta registrado\n");
				send(descriptor,buffer,sizeof(buffer),0);
				
				return false;
			}
		}

		leer.close(); //Se cierra el fichero de usuarios
	}
	
	escribir.open("usuarios.txt",std::ios::app); //Se abre el fichero de usuarios

	if(!escribir){ //No se ha podido abrir el fichero de usuarios
		bzero(buffer,sizeof(buffer));
		sprintf(buffer,"-Err. No se pudo establecer conexion con la base de datos\n");
		send(descriptor,buffer,sizeof(buffer),0);
		
		return false;
	}
	
	else{ //Se escribe el nombre de usuario y la password en el fichero de usuarios
		escribir << useraux1 << " " <<passwordaux1 << std::endl;
	}

	escribir.close(); //Se cierra el fichero de usuarios

	return true;
}

bool funcionUsuario(std::string usuario,int descriptor){
	std::ifstream leer;
	char buffer[MSG_SIZE];
	char useraux[80],passwordaux[80];
	
	leer.open("usuarios.txt"); //Se abre el fichero de usuarios
	
	if(!leer){ //No se ha podido abrir el fichero de usuarios
		bzero(buffer,sizeof(buffer));
		sprintf(buffer,"-Err. No se pudo establecer conexion con la base de datos\n");
		send(descriptor,buffer,sizeof(buffer),0);
		
		return false;
	}
	
	else{ //Se ha podido abrir el fichero de usuarios
		while(leer >> useraux >> passwordaux){ //Se leen los nombres de usuario y las passwords del fichero de usuarios
			if(strcmp(useraux,usuario.c_str())==0){ //Se ha encontrado al usuario
				return true;
			}
		}
		
		bzero(buffer,sizeof(buffer));
		sprintf(buffer,"-Err. Usuario incorrecto\n");
		send(descriptor,buffer,sizeof(buffer),0);

		leer.close(); //Se cierra el fichero de usuarios
	}
	
	return false;
}

int indiceUsuario(int sd,std::vector <Usuario> usuarios){
	//Se busca el indice del usuario especificado en la lista de usuarios
	for(int i=0;i<(int)usuarios.size();i++){
		if(usuarios[i].getDescriptor()==sd){
			return i;
		}
	}

	return -1;
}

bool funcionPassword(std::string usuario,std::string password,int descriptor){
	std::ifstream leer;
	char buffer[MSG_SIZE];
	char useraux[80],passwordaux[80];
	
	leer.open("usuarios.txt"); //Se abre el fichero de usuarios
	
	if(!leer){ //No se ha podido abrir el fichero de usuarios
		bzero(buffer,sizeof(buffer));
		sprintf(buffer,"-Err. No se pudo establecer conexion con la base de datos\n");
		send(descriptor,buffer,sizeof(buffer),0);
		
		return false;
	}
	
	else{ //Se ha podido abrir el fichero de usuarios
		while(leer >> useraux >> passwordaux){ //Se leen los nombres de usuario y las passwords del fichero de usuarios
			if(strcmp(useraux,usuario.c_str())==0 and strcmp(passwordaux,password.c_str())==0){ //Se ha encontrado al usuario y su password es correcta
				return true;
			}
		}
		
		bzero(buffer,sizeof(buffer));
		sprintf(buffer,"-Err. Password incorrecta\n");
		send(descriptor,buffer,sizeof(buffer),0);
		
		leer.close(); //Se cierra el fichero de usuarios
	}
	
	return false;
}

int indicePartida(int descriptor, std::vector <Partida> partidas){
	for(int i=0;i<(int)partidas.size();i++){ //Se busca entre todas las partidas
		if(partidas[i].getUsuario1()->getDescriptor()==descriptor || partidas[i].getUsuario2()->getDescriptor()==descriptor){ //El descriptor coincide con el descriptor de uno de los jugadores de la partida
			return i;
		}
	}

	return -1;

}

bool existeUsuario(std::string nombre,std::vector <Usuario> usuarios){
	for(int i=0;i<(int)usuarios.size();i++){ //Se busca entre todos los usuarios
		if(usuarios[i].getUsuario()==nombre){ //Se ha encontrado al usuario
			return true;
		}
	}
	
	return false;
}
