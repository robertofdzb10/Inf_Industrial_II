// Sleep para cuando se desea que no se empieze a contar en cero, tal y como sucede en el sleep compensado normal

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define PERIODO 500 // Periodo del ciclo en ms

void SleepCompensadoMultiplo(DWORD tiempo) {

	long espera;
	espera = tiempo - (clock()%tiempo); // 500 - (300%500 = 300) -> 500-300 = 200
	Sleep(espera);
	return;
}


int main() {


	while (!_kbhit) { // Búcle del programa

		// Acciones a realizar cada 500ms

		SleepCompensadoMultiplo(PERIODO);
	}
}