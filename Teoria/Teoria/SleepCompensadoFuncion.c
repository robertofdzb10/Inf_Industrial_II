#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define PERIODO 500 // Periodo del ciclo en ms

void SleepCompensado(DWORD tiempo) {

	// Variables
	long espera;
	static long proximoDisparo = -1; // Se va usar el -1 como valor especial, al ser estática se retiene su valor entre llamadas sucesivas de la función

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

	while (!_kbhit) { // Búcle del programa

		// Acciones a realizar cada 500ms

		SleepCompensado(PERIODO);
	}
}