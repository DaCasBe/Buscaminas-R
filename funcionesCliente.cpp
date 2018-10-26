#include "funcionesCliente.hpp"
#include <string.h>
#include <iostream>

#define BRD_SIZE 10

void recibirTablero(char buffer[MSG_SIZE]){
	int j=0;
	int k=0;
	char matriz[BRD_SIZE][BRD_SIZE];
	
	//Se transforma el tablero en una matriz
	for(int i=0;i<(int)strlen(buffer);i++){
		if(i%2==0){
			matriz[j][k]=buffer[i];
			
			if(k==BRD_SIZE-1){
				k=0;
				
				if(j==BRD_SIZE-1){
					break;
				}
				
				else{
					j++;
				}
			}
			
			else{
				k++;
			}
		}
	}
	
	//Se imprime el tablero
	std::cout << std::endl << std::endl << "     A   B   C   D   E   F   G   H   I   J" << std::endl;
	
	for(int i=0;i<BRD_SIZE;i++){
		if(i<BRD_SIZE-1){
			std::cout << i+1 << "  ";
		}
		
		else{
			std::cout << i+1 << " ";
		}
		
		for(int j=0;j<BRD_SIZE;j++){
			std::cout << "| "<< matriz[i][j] << " ";
		}
		
		std::cout << "|" << std::endl;
	}

	std::cout << std::endl;
}
