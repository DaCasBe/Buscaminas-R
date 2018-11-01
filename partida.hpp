#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#include "usuario.hpp"
#include <vector>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>

#define BRD_SIZE 10
#define NUM_FLAGS 10

class Partida{
	private:
		Usuario _usuario1; //Jugador 1
		Usuario _usuario2; //Jugador 2
		std::vector < std::vector <char> > _tablero_real; //Tablero con los valores reales del juego
		std::vector < std::vector <char> > _tablero_muestra; //Tablero con los valores que se muestran a los jugadores
		int _turno; //Indica de quien es el turno
		int _banderas1; //Banderas que tiene el jugador 1
		int _banderas2; //Banderas que tiene el jugador 2
		bool _fin; //Indica si ha acabado la partida


	public:
		Partida(){
			//Se inicializan las variables
			_banderas1=NUM_FLAGS;
			_banderas2=NUM_FLAGS;
			_turno=1;
			_fin=false;
			
			//Se reserva memoria para el tablero real
			_tablero_real.resize(BRD_SIZE);
			
			for(int i=0; i < BRD_SIZE ; i++){
				_tablero_real[i].resize(BRD_SIZE);
			}
			
			//Se reserva memoria para el tablero de muestra y se inicializa
			_tablero_muestra.resize(BRD_SIZE);
			
			for(int i=0; i < BRD_SIZE ; i++){
				for(int j=0 ; j < BRD_SIZE ; j++){
					_tablero_muestra[i].push_back('-');
				}
			}

			generarTablero(); //Se genera el tablero de juego
			
			for(int i=0;i<BRD_SIZE;i++){
				std::cout << "| ";
				for(int j=0;j<BRD_SIZE;j++){
					std::cout << getTableroReal()[i][j] << " | ";
				}
				std::cout << std::endl;
			}
		}
		
		inline void setUsuario1(Usuario usuario){
			_usuario1=usuario;
		}
		
		inline Usuario * getUsuario1(){
			return &_usuario1;
		}
		
		inline void setUsuario2(Usuario usuario){
			_usuario2=usuario;
		}
		
		inline Usuario * getUsuario2(){
			return &_usuario2;
		}
		
		inline std::vector < std::vector <char> > getTableroReal(){
			return _tablero_real;
		}
		
		inline std::vector < std::vector <char> > getTableroMuestra(){
			return _tablero_muestra;
		}
		
		inline int getTurno(){
			return _turno;
		}
		
		inline int getBanderas1(){
			return _banderas1;
		}
		
		inline int getBanderas2(){
			return _banderas2;
		}
		
		inline void setFin(bool fin){
			_fin=fin;
		}
		
		inline bool getFin(){
			return _fin;
		}
		
		void generarTablero();
		
		void colocarMinas();
		
		inline void cambiarTurno(){
			char buffer[255];

			if(getTurno()==1){ //Era el turno del jugador 1
				_turno=2; //Le toca al jugador 2
				strcpy(buffer,"+Ok. Es tu turno\n");
				send(getUsuario2()->getDescriptor(),buffer,strlen(buffer),0);
			}
			
			else{ //Era el turno del jugador 2
				_turno=1; //Le toca al jugador 1
				strcpy(buffer,"+Ok. Es tu turno\n");
				send(getUsuario1()->getDescriptor(),buffer,strlen(buffer),0);
			}
		}
		
		void expadirCeros(int x, int y);
		
		bool ponerBandera(int descriptor,std::string x, int y);
		
		int destaparCasillas(int descriptor, std::string x , int y);
		
		void enviarTablero();
		
		int buscarUsuario(int descriptor){
			if(getUsuario1()->getDescriptor()==descriptor){
				return 1;
			}
			
			else{
				return 2;
			}
		}
		
		void finBanderas();
};

#endif
