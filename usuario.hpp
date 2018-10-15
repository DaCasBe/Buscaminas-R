#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

#define CONECTADO 0
#define NOMBRE 1
#define LOGUEADO 2

class Usuario{
	private:
		std::string _usuario;
		std::string _password;
		int _estado;
		int _descriptor;
		
	public:
		inline Usuario(int descriptor,std::string usuario=NULL,std::string password=NULL,int estado=0){
			setDescriptor(descriptor);
			setUsuario(usuario);
			setPassword(password);
			setEstado(estado);
		}
		
		inline void setDescriptor(int descriptor){
			_descriptor=descriptor;
		}
		
		inline int getDescriptor(){
			return _descriptor;
		}
		
		inline void setUsuario(std::string usuario){
			_usuario=usuario;
		}
		
		inline std::string getUsuario(){
			return _usuario;
		}
		
		inline void setPassword(std::string password){
			_password=password;
		}
		
		inline std::string getPassword(){
			return _password;
		}
		
		inline void setEstado(int estado){
			_estado=estado;
		}
		
		inline int getEstado(){
			return _estado;
		}
};

#endif
