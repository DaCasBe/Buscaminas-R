#ifndef USUARIO_HPP
#define USUARIO_HPP

#define CONECTADO 0
#define NOMBRE 1
#define LOGUEADO 2

class Usuario{
	private:
		int _descriptor; //Descriptor asociado al usuario
		std::string _usuario; //Nombre de usuario
		std::string _password; //Password del usuario
		int _estado; //Estado del usuario
		
	public:
		inline Usuario(){
			setEstado(CONECTADO); //Se establece el estado del usuario como CONECTADO
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
