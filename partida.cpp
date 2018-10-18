#include "partida.hpp"
#include <cstdlib>

void Partida::generarTablero(std::vector < std::vector <char> > * tablero){
	int cont_minas;
	
	colocarMinas(tablero); //Se colocan las minas en el tablero
	
	for(int i;i<BRD_SIZE;i++){
		for(int j;j<BRD_SIZE;j++){
			cont_minas=0;
			
			if((*tablero)[i][j]!='*'){ //No hay ninguna mina en la casilla
				if(i==0 and j==0){ //La casilla es la esquina superior izquierda
					//Se cuentan las minas de alrededor
					if((*tablero)[i+1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==0 and j==BRD_SIZE-1){ //La casilla es la esquina superior derecha
					//Se cuentan las minas de alrededor
					if((*tablero)[i+1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j-1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==BRD_SIZE-1 and j==0){ //La casilla es la esquina inferior izquierda
					//Se cuentan las minas de alrededor
					if((*tablero)[i-1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==BRD_SIZE-1 and j==BRD_SIZE-1){ //La casilla es la esquina inferior derecha
					//Se cuentan las minas de alrededor
					if((*tablero)[i-1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j-1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==0 and j!=0 and j!=BRD_SIZE-1){ //La casilla esta en el lateral superior
					//Se cuentan las minas de alrededor
					if((*tablero)[i+1][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i==BRD_SIZE-1 and j!=0 and j!=BRD_SIZE-1){ //La casilla esta en el lateral inferior
					//Se cuentan las minas de alrededor
					if((*tablero)[i-1][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j+1]=='*'){
						cont_minas++;
					}
				}
				
				if(i!=0 and i!=BRD_SIZE-1 and j==0){ //La casilla esta en el lateral izquierdo
					//Se cuentan las minas de alrededor
					if((*tablero)[i-1][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j]=='*'){
						cont_minas++;
					}
				}
				
				if(i!=0 and i!=BRD_SIZE-1 and j==BRD_SIZE-1){ //La casilla esta en el lateral derecho
					//Se cuentan las minas de alrededor
					if((*tablero)[i-1][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j]=='*'){
						cont_minas++;
					}
				}
				
				if(i!=0 and i!=BRD_SIZE-1 and j!=0 and j!=BRD_SIZE-1){ //La casilla esta en medio del tablero
					if((*tablero)[i-1][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i-1][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i][j+1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j-1]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j]=='*'){
						cont_minas++;
					}
					
					if((*tablero)[i+1][j+1]=='*'){
						cont_minas++;
					}
				}
				
				(*tablero)[i][j]=char(cont_minas); //Se escribe en la casilla el numero de minas que tiene alrededor
			}
		}
	}
}

void Partida::colocarMinas(std::vector < std::vector <char> > * tablero){
	srand(time(NULL));
	
	int minas=0;
	
	do{ //No se terminaran de colocar minas mientras no esten todas colocadas
		for(int i;i<BRD_SIZE;i++){
			for(int j;j<BRD_SIZE;j++){
				//Se coloca una mina en una casilla con una probabilidad de 1/100
				if(((rand()%100)+1)<=1 and (*tablero)[i][j]!='*'){
					(*tablero)[i][j]='*'; //Se coloca una mina
					minas++;
				}
			}
		}
	}while(minas<NUM_MINES);
}
