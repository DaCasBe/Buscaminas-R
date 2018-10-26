OBJETOS = partida.o funcionesServidor.o funcionesCliente.o

CPPFLAGS = -c -g -Wall -ansi


objetivo: servidor.exe cliente.exe

#Opciones para NO depurar
ndebug: CPPFLAGS += -DNDEBUG 
ndebug: objetivo

servidor.exe: $(OBJETOS)
						g++ servidor.cpp $(OBJETOS) -o servidor.exe 

cliente.exe: cliente.cpp
						g++ cliente.cpp funcionesCliente.o -o cliente.exe 

partida.o: partida.cpp partida.hpp
						g++ $(CPPFLAGS) partida.cpp

funcionesServidor.o: funcionesServidor.cpp funcionesServidor.hpp
						g++ $(CPPFLAGS) funcionesServidor.cpp
						
funcionesCliente.o: funcionesCliente.cpp funcionesCliente.hpp
						g++ $(CPPFLAGS) funcionesCliente.cpp


clean:
	rm -rf *~ $(OBJETOS)

