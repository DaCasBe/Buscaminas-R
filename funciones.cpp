#include "funciones.hpp"

void salirCliente(int socket, fd_set * readfds,std::vector <Usuario> &usuarios){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < usuarios.size() - 1; j++)
        if (usuarios[j].getDescriptor() == socket)
            break;
    for (; j < usuarios.size() - 1; j++)
        (usuarios[j].setDescriptor(usuarios[j+1].getDescriptor()));
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<usuarios.size(); j++)
        if(usuarios[j].getDescriptor() != socket)
            send(usuarios[j].getDescriptor(),buffer,strlen(buffer),0);

    usuarios.erase(usuarios.begin()+j);
}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}

std::vector <std::string> dividirCadena(std::string cadena,std::string separador){
    std::vector <std::string> dividido;
    
    while(cadena.find(separador)<=MSG_SIZE){
        dividido.push_back(cadena.substr(0, cadena.find(separador)));
        cadena.erase(0, cadena.find(separador)+1);
    }

    dividido.push_back(cadena.substr(0, cadena.size()));
    
    return dividido;
}

bool registro(std::string user, std::string password, int descriptor){

    char bufferaux [MSG_SIZE];
    char useraux1[80], useraux2[80];
    char passwordaux1[80], passwordaux2[80];

    strcpy(useraux1,user.c_str());
    strcpy(passwordaux1,password.c_str());

    if(user.c_str()==NULL && password.c_str()==NULL){
        bzero(bufferaux,sizeof(bufferaux));
        sprintf(bufferaux,"-Err. No se introdujo el nombre de usuario o la password\n");
        send(descriptor,bufferaux,sizeof(bufferaux),0);
        
        return false;
    }

    std::ifstream leer;
    std::ofstream escribir;

    leer.open("usuarios.txt");

    if(!leer){ //No se ha podido abrir el fichero de usuarios
        //Se crea el fichero de usuarios
        escribir.open("usuarios.txt", std::ios::out);
        escribir.close();   
    }
    else{
        while(leer >> useraux2 >> passwordaux2){

            if(strcpy(useraux1,useraux2)==0){
                bzero(bufferaux,sizeof(bufferaux));
                sprintf(bufferaux,"-Err. El usuario ya esta registrado\n");
                send(descriptor,bufferaux,sizeof(bufferaux),0);
                return false;
            }
        }

        leer.close();
    }

    escribir.open("usuarios.txt", std::ios::app);

    if(!escribir)
    {
        bzero(bufferaux,sizeof(bufferaux));
        sprintf(bufferaux,"-Err. No se pudo establecer conexion con la base de datos\n");
        send(descriptor,bufferaux,sizeof(bufferaux),0);
        return false;
    }
    else
    {
        escribir << useraux1 << " " <<passwordaux1;
    }

    escribir.close();

    return true;
}

bool funcionUsuario(std::string usuario, int descriptor){

	std::ifstream leer;
	leer.open("usuarios.txt");

	char bufferaux [MSG_SIZE];
	char useraux[80], passwordaux[80];

	if(!leer){ //No se ha podido abrir el fichero de usuarios
		std::cout << "Fallo al abrir la base de datos" << std::endl;   
    }
    else{
        while(leer >> useraux >> passwordaux){

            if(strcmp(useraux,usuario.c_str())==0){ // si hay un usuario registrado con ese nombre devolvemos true
                return true;
            }
        }

        leer.close();
    }

    // el usuario no existe, enviamos un mensaje de error

    bzero(bufferaux,sizeof(bufferaux));
    sprintf(bufferaux,"-Err. Usuario incorrecto\n");
    send(descriptor,bufferaux,sizeof(bufferaux),0);

    return false;

}

Usuario buscarUsuario(int sd,std::vector <Usuario> usuarios){
	for(int i=0;i<usuarios.size();i++){
		if(usuarios[i].getDescriptor()==sd){
			return usuarios[i];
		}
	}
}
