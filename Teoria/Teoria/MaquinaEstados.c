#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main() {

	enum Estados { Inicio = 0,  Encendido = 1, Apagado = 2};
	enum Estados estado = Inicio;

	while ( _kbhit ) {

		// Fuera del switch el código comun para todos los estados

		switch (estado) {

			case Inicio:
				// Código de inicialización
				estado = 1;
				break;

			case Encendido:
				// Código a ejecutar mientras este en el estado 1

				break;

			case Apagado:

				break;

			default: 
				// Situación de error (When others)
				break;
		}
	}
	return 0;
}