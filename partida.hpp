#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#define BRD_SIZE 10
#define NUM_MINES 20
#define NUM_FLAGS 10

#include "usuario.hpp"
#include <vector>

class Partida{
	private:
		Usuario _usuario1; //Jugador 1
		Usuario _usuario2; //Jugador 2
		std::vector < std::vector <char> > _tablero_real; //Tablero con los valores reales del juego
		std::vector < std::vector <char> > _tablero_muestra; //Tablero con los valores que se muestran a los jugadores
		int _turno; //Indica de quien es el turno;
		int _banderas1; //Banderas que tiene el jugador 1
		int _banderas2; //Banderas que tiene el jugador 2


	public:
		Partida(Usuario usuario1,Usuario usuario2){
			//Se inicializan las variables
			setUsuario1(usuario1);
			setUsuario2(usuario2);
			setBanderas1(NUM_FLAGS);
			setBanderas2(NUM_FLAGS);
			_turno=1;
			
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
		
		inline void setBanderas1(int banderas){
			_banderas1=banderas;
		}
		
		inline void setBanderas2(int banderas){
			_banderas2=banderas;
		}
		
		void generarTablero();
		
		void colocarMinas();
		
		inline void cambiarTurno(){
			if(getTurno()==1){ //Era el turno del jugador 1
				_turno=2; //Le toca al jugador 2
			}
			
			if(getTurno()==2){ //Era el turno del jugador 2
				_turno=1; //Le toca al jugador 1
			}
		}
		
		void expadirCeros(int x, int y);
		
		void ponerBandera(int x, int y);
		
		int destaparCasillas(int descriptor, int x , int y);
		
		void enviarTablero();
};

#endif
