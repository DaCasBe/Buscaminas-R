OBJETOS = partida.o funciones.o

CPPFLAGS = -c -g -Wall -ansi


objetivo: servidor.exe cliente.exe

#Opciones para NO depurar
ndebug: CPPFLAGS += -DNDEBUG 
ndebug: objetivo

servidor.exe: $(OBJETOS)
						g++ servidor.cpp $(OBJETOS) -o servidor.exe 

cliente.exe: cliente.cpp
						g++ cliente.cpp -o cliente.exe 

partida.o: partida.cpp partida.hpp
						g++ $(CPPFLAGS) partida.cpp

funciones.o: funciones.cpp funciones.hpp
						g++ $(CPPFLAGS) funciones.cpp


clean:
	rm -rf *~ $(OBJETOS)

