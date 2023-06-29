#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

int main() {

	long cadena_t[7] = {1,0,0,0,0,1,0};
	long *cadena;
	int ceros = 0;
	int unos = 0;

	for (int i = 0; i < 7; i++) {
		if (cadena_t[i] == 0) {
			ceros++;
		}
		else {
			unos++;
		}
	}
	cadena = (long*)malloc(8 * sizeof(long));
	if (cadena == NULL) {
		printf("Error en la gestión de la memoría dinámica.");
	}
	memcpy(cadena, cadena_t, 7 * sizeof(long));
	if ( ((unos + 1) % 2) != 0 ){
		cadena[7] = 1;
	} 
	else {
		cadena[7] = 0;
	}

	for (int i = 0; i < 8; i++) {
		printf("%d\n", cadena[i]);
	}
}