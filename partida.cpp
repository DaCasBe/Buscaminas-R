#include "partida.hpp"
#include <cstdlib>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define MSG_SIZE 250

void Partida::generarTablero(){
	int cont_minas;
	
	colocarMinas(); //Se colocan las minas en el tablero
	
	for(int i;i<BRD_SIZE;i++){
		for(int j;j<BRD_SIZE;j++){
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
		for(int i;i<BRD_SIZE;i++){
			for(int j;j<BRD_SIZE;j++){
				//Se coloca una mina en una casilla con una probabilidad de 1/100
				if(((rand()%100)+1)<=1 and _tablero_real[i][j]!='*'){
					_tablero_real[i][j]='*'; //Se coloca una mina
					minas++;
				}
			}
		}
	}while(minas<NUM_MINES);
}

void Partida::expadirCeros(int x, int y){
	if(_tablero_real[x][y] != '*' && _tablero_real[x][y] != '0' && (_tablero_muestra[x][y] == '-' || _tablero_muestra[x][y] == 'A' || _tablero_muestra[x][y] == 'B' || strcmp(&_tablero_muestra[x][y],"AB")==0 ) ){ // si no es una bomba y no es una casilla vacia, es decir es 1,2,3,4,5,6,7,8 se destapa la casilla
		_tablero_muestra[x][y]=_tablero_real[x][y];
	}else{

		if(_tablero_real[x][y] == '0' &&  (_tablero_muestra[x][y] == '-' || _tablero_muestra[x][y] == 'A' || _tablero_muestra[x][y] == 'B' || strcmp(&_tablero_muestra[x][y],"AB")==0 ) ){

			_tablero_muestra[x][y]=_tablero_real[x][y];
			if(x-1>=0){
				expadirCeros(x-1,y);
			}
			if(y-1>=0){
				expadirCeros(x,y-1);
			}
			if(y+1<=10){
				expadirCeros(x,y+1);
			}
			if(x+1<=10){
				expadirCeros(x+1,y);
			}
		}
	}

}

void Partida::ponerBandera(int x, int y){
	if(getTurno()==1){
		if(_tablero_muestra[x][y] == '-'){
			_tablero_muestra[x][y] = 'A';
		}
		if(_tablero_muestra[x][y] == 'B'){
			_tablero_muestra[x][y] = 'X';
		}
	}

	else{
		if(_tablero_muestra[x][y] == '-'){
			_tablero_muestra[x][y] = 'B';
		}
		if(_tablero_muestra[x][y] == 'A'){
			_tablero_muestra[x][y] = 'X';
		}
	}

}

int Partida::destaparCasillas(int descriptor, int x , int y){
	if(descriptor==_usuario1.getDescriptor() && getTurno()==1){ // la casilla es una bomba
		if(_tablero_real[x][y]=='*'){
			//game over
			return -1;
		}
		else if(_tablero_real[x][y]!='*' && _tablero_muestra[x][y]=='-'){ // la casilla es valida
			expadirCeros(x,y);
			cambiarTurno();
			return 0;
		}
		else{ // casilla invalida
			return 1;
		}
	}
	else if(descriptor==_usuario2.getDescriptor() && getTurno()==2){
		if(_tablero_real[x][y]=='*'){
			//game over
			return -1;
		}
		else if(_tablero_real[x][y]!='*' && _tablero_muestra[x][y]=='-'){ // la casilla es valida
			expadirCeros(x,y);
			cambiarTurno();
			return 0;
		}
		else{ // casilla invalida
			return 1;
		}
	}
	else{
		return 2; // no es tu turno
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
		
			if((cont+1)%(2*MSG_SIZE)==0){
				if(i!=9){
					cadena.push_back(';');
				}
				
				else{
					cadena.push_back(',');
				}
			}
			
			cont++;
		}
	}
	
	//Se envia el tablero al jugador 1
	bzero(buffer,sizeof(buffer));
	strcpy(buffer,cadena.c_str());
	send(getUsuario1()->getDescriptor(),buffer,sizeof(buffer),0);
	
	//Se envia el tablero al jugador 2
	bzero(buffer,sizeof(buffer));
	strcpy(buffer,cadena.c_str());
	send(getUsuario2()->getDescriptor(),buffer,sizeof(buffer),0);
}
