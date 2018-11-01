#include "funcionesCliente.hpp"
#include <string.h>
#include <iostream>

#define BRD_SIZE 10

void recibirTablero(char buffer[MSG_SIZE]){
	//Se imprime el tablero
	std::cout << std::endl << std::endl << "     A   B   C   D   E   F   G   H   I   J" << std::endl;
	int cont=1;
	
	for(int i=0;i<(int)strlen(buffer);i++){
		if(i==0){
			std::cout << cont << "  ";
			cont++;
		}

		if(buffer[i]!=','){
			if(buffer[i]==';'){
				if(cont!=11){
					if(cont==10){
						std::cout << "|" << std::endl << cont << " ";
						cont++;
					}else{
						std::cout << "|" << std::endl << cont << "  ";
						cont++;
					}
				}
				else{
					std::cout << "|" << std::endl;
				}
			}
			
			else if(buffer[i]=='A' and buffer[i+1]=='B'){
				std::cout << "| " << buffer[i] << buffer[i+1];
				i++;
			}
			
			else if(buffer[i]=='B' and buffer[i-1]=='A'){
			
			}
			
			else{
				std::cout << "| " << buffer[i] << " ";
			}
		}
	}
	
	std::cout << std::endl;
}
