
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

int leeCelulaFotoelectrica(void); // 0 paquete 1 no paquete
void leeCodigoBarras(void* datos);

typedef struct {
	long v0: 1;
	long v1 : 2;
	long v2 : 32;
	long v3 : 10;
	long v4 : 7;
	// Posibles valores de v5, indicados de acuerdo a v4
	unsigned long v5_0 : 8; 
	unsigned int v5_1 : 16;
	unsigned int v5_2 : 24;
	unsigned int v5_3 : 32;
	double v5_4;
	unsigned long * v5_5; // Su dimensión es este valor dividido entre 8
} Datos;

int main() {

	// Declaración de variables
	long periodo = 100; // Valor de cada periodo
	long proximoDisparo = 0; //T0 más periodo
	long espera = 0; // Periodo + T0 - Tiempo de ejecución, si es negativo  no espera

	int nuevoPaquete = 1;

	long* paquete_t = (long*)malloc(5 * sizeof(long)); // Variable paquete intermediaria, de tamaño 160 bits (32 * 5)
	Datos* paquete = (Datos*)malloc(sizeof(Datos));
	if (paquete_t == NULL || paquete == NULL) {
		printf("Error resevando memoria");
		return 1;
	}

	proximoDisparo = clock(); //Damos a proximoDisparo el valor de T0

	// Bucel principal
	while ( !_kbhit() ) {

		nuevoPaquete = leeCelulaFotoelectrica();
		if (nuevoPaquete == 1) { // No hay paquete
			// Esperar 100ms
			proximoDisparo = periodo + proximoDisparo;
			printf("I was here %d \n", clock());
			espera = proximoDisparo - clock();
			if (espera > 0) {
				Sleep(espera);
			}
		}
		else { // Hay paquete
			leeCodigoBarras(paquete_t);

			// Pasar de paquete temporal a paquete final
			paquete = paquete_t;

			// Guardar en fichero de texto


			
			// Esperar 500 ms
			proximoDisparo = 5 * periodo + proximoDisparo;
			espera = proximoDisparo - clock();
			if (espera > 0) {
				Sleep(espera);
			}
		}
	}
	free(paquete_t);
	return 0;
}
