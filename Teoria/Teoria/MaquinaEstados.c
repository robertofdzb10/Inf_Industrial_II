#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main() {

	enum Estados { Inicio = 0,  Encendido = 1, Apagado = 2};
	enum Estados estado = Inicio;

	while ( _kbhit ) {

		// Fuera del switch el c�digo comun para todos los estados

		switch (estado) {

			case Inicio:
				// C�digo de inicializaci�n
				estado = 1;
				break;

			case Encendido:
				// C�digo a ejecutar mientras este en el estado 1

				break;

			case Apagado:

				break;

			default: 
				// Situaci�n de error (When others)
				break;
		}
	}
	return 0;
}