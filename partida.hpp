#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#define BRD_SIZE 10
#define NUM_MINES 10

#include "usuario.hpp"
#include <vector>

class Partida{
	private:
		Usuario _usuario1; //Jugador 1
		Usuario _usuario2; //Jugador 2
		std::vector < std::vector <char> > _tablero_real; //Tablero con los valores reales del juego
		std::vector < std::vector <char> > _tablero_muestra; //Tablero con los valores que se muestran a los jugadores
		int _turno; //Indica de quien es el turno;

	public:
		Partida(Usuario usuario1,Usuario usuario2){
			_usuario1=usuario1;
			_usuario2=usuario2;
			_turno=1;
		}
		
		inline Usuario * getUsuario1(){
			return &_usuario1;
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
		
		void generarTablero(std::vector < std::vector <char> > * tablero);
		
		void colocarMinas(std::vector < std::vector <char> > * tablero);
		
		inline void cambiarTurno(){
			if(getTurno()==1){ //Era el turno del jugador 1
				_turno=2; //Le toca al jugador 2
			}
			
			if(getTurno()==2){ //Era el turno del jugador 2
				_turno=1; //Le toca al jugador 1
			}
		}
};

#endif
