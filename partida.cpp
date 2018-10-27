#include "partida.hpp"
#include <cstdlib>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define MSG_SIZE 250

void Partida::generarTablero(){
	int cont_minas;
	
	colocarMinas(); //Se colocan las minas en el tablero
	
	for(int i=0;i<BRD_SIZE;i++){
		for(int j=0;j<BRD_SIZE;j++){
			cont_minas=0;
			
			if(_tablero_real[i][j]!='*'){ //No hay ninguna mina en la casilla
				if(i==0 and j==0){ //La casilla es la esquina superior izquierda
					//Se cuentan las minas de alrededor
					if(_tablero_real[i+1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==0 and j==BRD_SIZE-1){ //La casilla es la esquina superior derecha
					//Se cuentan las minas de alrededor
					if(_tablero_real[i+1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j-1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==BRD_SIZE-1 and j==0){ //La casilla es la esquina inferior izquierda
					//Se cuentan las minas de alrededor
					if(_tablero_real[i-1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==BRD_SIZE-1 and j==BRD_SIZE-1){ //La casilla es la esquina inferior derecha
					//Se cuentan las minas de alrededor
					if(_tablero_real[i-1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j-1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==0 and j!=0 and j!=BRD_SIZE-1){ //La casilla esta en el lateral superior
					//Se cuentan las minas de alrededor
					if(_tablero_real[i+1][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==BRD_SIZE-1 and j!=0 and j!=BRD_SIZE-1){ //La casilla esta en el lateral inferior
					//Se cuentan las minas de alrededor
					if(_tablero_real[i-1][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i!=0 and i!=BRD_SIZE-1 and j==0){ //La casilla esta en el lateral izquierdo
					//Se cuentan las minas de alrededor
					if(_tablero_real[i-1][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j]=='*'){
						cont_minas++;
					}
				}
				
				if(i!=0 and i!=BRD_SIZE-1 and j==BRD_SIZE-1){ //La casilla esta en el lateral derecho
					//Se cuentan las minas de alrededor
					if(_tablero_real[i-1][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j]=='*'){
						cont_minas++;
					}
				}
				
				if(i!=0 and i!=BRD_SIZE-1 and j!=0 and j!=BRD_SIZE-1){ //La casilla esta en medio del tablero
					if(_tablero_real[i-1][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i-1][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i][j+1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j-1]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j]=='*'){
						cont_minas++;
					}
					
					if(_tablero_real[i+1][j+1]=='*'){
						cont_minas++;
					}
				}
				
				_tablero_real[i][j]=char(cont_minas); //Se escribe en la casilla el numero de minas que tiene alrededor
			}
		}
	}
}

void Partida::colocarMinas(){
	srand(time(NULL));
	
	int minas=0;
	
	do{ //No se terminaran de colocar minas mientras no esten todas colocadas
		for(int i=0;i<BRD_SIZE;i++){
			for(int j=0;j<BRD_SIZE;j++){
				//Se coloca una mina en una casilla con una probabilidad de 1/100
				if(((rand()%100)+1)<=1 and _tablero_real[i][j]!='*'){
					_tablero_real[i][j]='*'; //Se coloca una mina
					minas++;
				}
			}
		}
	}while(minas<NUM_MINES);
}

void Partida::expadirCeros(int x,int y){
	if(getTableroReal()[x][y]!='*' and getTableroReal()[x][y]!='0' and (getTableroMuestra()[x][y]=='-' or getTableroMuestra()[x][y]=='A' or getTableroMuestra()[x][y]=='B' or strcmp(&getTableroMuestra()[x][y],"AB")==0)){ //La casilla no tiene una bomba, no es una casilla vacia y no esta descubierta
		getTableroMuestra()[x][y]=getTableroReal()[x][y]; //Se descubre la casilla
	}
	
	else{ //La casilla tiene una bomba, es una casilla vacia o esta descubierta
		if(getTableroReal()[x][y]=='0' and  (getTableroMuestra()[x][y]=='-' or getTableroMuestra()[x][y]=='A' or getTableroMuestra()[x][y]=='B' or strcmp(&getTableroMuestra()[x][y],"AB")==0)){ //La casilla tiene un 0 y no esta descubierta
			getTableroMuestra()[x][y]=getTableroReal()[x][y]; //Se descubre la casilla
			
			if(x-1>=0){ //Hay casillas arriba
				expadirCeros(x-1,y); //Se descubre la casilla de arriba
			}
			
			if(y-1>=0){ //Hay casillas a la izquierda
				expadirCeros(x,y-1); //Se descubre la casilla de la izquierda
			}
			
			if(y+1<BRD_SIZE){ //Hay casillas a la derecha
				expadirCeros(x,y+1); //Se descubre la casilla de la derecha
			}
			
			if(x+1<BRD_SIZE){ //Hay casillas abajo
				expadirCeros(x+1,y); //Se descubre la casilla de abajo
			}
		}
	}
}

void Partida::ponerBandera(int x,int y){
	if(getTurno()==1){ //Es el turno del jugador 1
		if(getTableroMuestra()[x][y]=='-'){ //La casilla no esta descubierta
			getTableroMuestra()[x][y]='A'; //Se coloca una bandera del jugador 1
		}
		
		if(getTableroMuestra()[x][y]=='B'){ //Hay una bandera del jugador 2 en la casilla
			getTableroMuestra()[x][y]='X'; //Se dejan colocadas las banderas de ambos jugadores
		}
	}

	else{ //Es el turno del jugador 2
		if(getTableroMuestra()[x][y]=='-'){ //La casilla no esta descubierta
			getTableroMuestra()[x][y]='B'; //Se coloca una bandera del jugador 2
		}
		
		if(getTableroMuestra()[x][y]=='A'){ //Hay una bandera del jugador 1 en la casilla
			getTableroMuestra()[x][y]='X'; //Se dejan colocadas las banderas de ambos jugadores
		}
	}
}

int Partida::destaparCasillas(int descriptor,std::string x,int y){

	int fila,columna;

	columna=y-1;

	if(!x.compare("A")){
		fila=0;
	}
	if(!x.compare("B")){
		fila=1;
	}
	if(!x.compare("C")){
		fila=2;
	}
	if(!x.compare("D")){
		fila=3;
	}
	if(!x.compare("E")){
		fila=4;
	}
	if(!x.compare("F")){
		fila=5;
	}
	if(!x.compare("G")){
		fila=6;
	}
	if(!x.compare("H")){
		fila=7;
	}
	if(!x.compare("I")){
		fila=8;
	}
	if(!x.compare("J")){
		fila=9;
	}


	std::cout << fila << " y " << columna << std::endl;


	if(descriptor==getUsuario1()->getDescriptor() and getTurno()==1){ //Descubre el jugador 1 en su turno
		if(getTableroReal()[fila][columna]=='*'){ //Hay una mina en la casilla
			return -1;
		}
		
		else if(getTableroReal()[fila][columna]!='*' and getTableroMuestra()[fila][columna]=='-'){ //La casilla no esta descubierta y no tiene una mina
			expadirCeros(columna,fila);
			
			cambiarTurno(); //Se cambia el turno

			enviarTablero();
			
			return 0;
		}
		else{ //La casilla es invalida
			return 1;
		}
	}
	
	else if(descriptor==getUsuario2()->getDescriptor() and getTurno()==2){ //Descubre el jugador 2 en su turno
		if(getTableroReal()[fila][columna]=='*'){ //Hay una mina en la casilla
			return -1;
		}
		
		else if(getTableroReal()[fila][columna]!='*' and getTableroMuestra()[fila][columna]=='-'){ //La casilla no esta descubierta y no tiene una mina
			expadirCeros(columna,fila);
			
			cambiarTurno(); //Se cambia el turno
			enviarTablero();
			
			return 0;
		}
		
		else{ //La casilla es invalida
			return 1;
		}
	}
	
	else{ //Algun jugador trata de descubrir cuando no le toca
		return 2;
	}
}

void Partida::enviarTablero(){
	char buffer[MSG_SIZE];
	std::string cadena;
	int cont=0;
	
	//Se transforma el tablero en una cadena según el protocolo a seguir
	for(int i=0;i<(int)getTableroMuestra().size();i++){
		for(int j=0;j<(int)getTableroMuestra().size();j++){
			cadena.push_back(getTableroMuestra()[i][j]);
			
			cont++;
			
			if(cont%2!=0){
				if(j!=BRD_SIZE-1){
					cadena.push_back(';');
				}
				
				else{
					cadena.push_back(',');
				}
			}
			
			cont++;
		}
	}
	
	//Se envia el tablero a ambos jugadores
	strcpy(buffer,cadena.c_str());
	send(getUsuario1()->getDescriptor(),buffer,sizeof(buffer),0);
	send(getUsuario2()->getDescriptor(),buffer,sizeof(buffer),0);
}
