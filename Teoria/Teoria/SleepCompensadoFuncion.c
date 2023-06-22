#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define PERIODO 500 // Periodo del ciclo en ms

void SleepCompensado(DWORD tiempo) {

	// Variables
	long espera;
	static long proximoDisparo = -1; // Se va usar el -1 como valor especial, al ser est�tica se retiene su valor entre llamadas sucesivas de la funci�n

	if (tiempo == 0) {
		proximoDisparo = clock();
	}
	else {
		if (proximoDisparo == -1) {
			proximoDisparo = clock();
		}
		proximoDisparo += tiempo;
		espera = proximoDisparo - clock();
		if (espera > 0) {
			Sleep(espera);
		}
	}
	return;
}

int main() {

	SleepCompesado(0); // Inicio de la cuenta de tiempos

	while (!_kbhit) { // B�cle del programa

		// Acciones a realizar cada 500ms

		SleepCompensado(PERIODO);
	}
}