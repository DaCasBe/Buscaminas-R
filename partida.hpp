#ifndef PARTIDA_HPP
#define PARTIDA_HPP

class Partida{
	private:
		Usuario _usuario1;
		Usuario _usuario2;

	public:
		Partida(Usuario usuario1,Usuario usuario2){
			_usuario1=usuario1;
			_usuario2=usuario2;
		}
		
		inline Usuario * getUsuario1(){
			return &_usuario1;
		}
		
		inline Usuario * getUsuario2(){
			return &_usuario2;
		}
}

#endif
