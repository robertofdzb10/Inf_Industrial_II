#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

int main() {

	// Declaración de variables
	long periodo = 0; // Valor de cada periodo
	long proximoDisparo = 0; //T0 más periodo
	long espera = 0; // Periodo + T0 - Tiempo de ejecución, si es negativo  no espera

	proximoDisparo = clock(); //Damos a proximoDisparo el valor de T0

	// Bucel principal
	while (!_kbhit()) {

		//	Accción a realizar cada periodo

		proximoDisparo = periodo + proximoDisparo;

		// Esperar
		espera = proximoDisparo - clock();
		if (espera > 0) {
			Sleep(espera);
		}
	}

	return 0;
}