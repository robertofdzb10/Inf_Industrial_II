#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

int main() {

	// Declaraci�n de variables
	long periodo = 0; // Valor de cada periodo
	long proximoDisparo = 0; //T0 m�s periodo
	long espera = 0; // Periodo + T0 - Tiempo de ejecuci�n, si es negativo  no espera

	proximoDisparo = clock(); //Damos a proximoDisparo el valor de T0

	// Bucel principal
	while (!_kbhit()) {

		//	Accci�n a realizar cada periodo

		proximoDisparo = periodo + proximoDisparo;

		// Esperar
		espera = proximoDisparo - clock();
		if (espera > 0) {
			Sleep(espera);
		}
	}

	return 0;
}