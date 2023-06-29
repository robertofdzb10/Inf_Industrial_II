#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>

void sleepCompensado(DWORD tiempo) {
	long espera;
	static long proximoDisparo = -1;

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

long productoEnviado(void);
long sincronizaRelojes(void);
void manejaPortezuela(int abrir);
double* leeDatos(int* dim);
void transfiereDatos(long tiempo, double* par, int dim);

typedef struct {
	int num;
	double* dir;
	long tiempo;
}ESTRUCTURA;

int main() {
	double* direccion = 0;
	long clockSuyo = -1;
	long clockReal = 0;
	int numDatos = 0;

	long tiempo = 0;
	double par = 0;

	ESTRUCTURA* array = NULL;

	int contadorMS = 0;
	int contadorMSParaCerrar = -1;

	int contadorMSLlegadaPrimerProd = -1;

	int contadorElementsPendientes = 0;
	while (1) {
		if (_kbhit()) {
			char c = _getch();
			if (c == 's') break;
		}

		if (contadorMS % 50 == 0) {
			long t = productoEnviado();
			if (t != -1) {
				clockSuyo = t;
				// hay un producto en camino
				//printf("Producto en camino: %ld\n", clockSuyo);
			}
		}

		//printf("%ld - %ld\n", clockSuyo, sincronizaRelojes());
		clockReal = sincronizaRelojes();
		if (clockSuyo != -1 && clockSuyo <= clockReal) {
			clockSuyo = -1;
			if (contadorMSLlegadaPrimerProd == -1) {
				contadorMSLlegadaPrimerProd = 0;
			}
			//printf("Ha llegado - %ld \n", clockReal);
			manejaPortezuela(1);
			contadorMSParaCerrar = contadorMS + 500;

			//sleepCompensado(500);
		}

		if (contadorMSParaCerrar != -1 && contadorMSParaCerrar == contadorMS) {
			//printf("Momento para cerrar\n");
			contadorMSParaCerrar = -1;
			manejaPortezuela(0);
			direccion = leeDatos(&numDatos);
			

			contadorElementsPendientes += 1;
			array = realloc(array, sizeof(ESTRUCTURA) * contadorElementsPendientes);
			if (array != NULL) {
				array[contadorElementsPendientes - 1].num = numDatos;
				array[contadorElementsPendientes - 1].tiempo = clock() - 500;
				double* contadorDuplicado = malloc(sizeof(double) * numDatos);
				if (contadorDuplicado != NULL) {
					// opcion 1
					for (size_t i = 0; i < numDatos; i++)
					{
						contadorDuplicado[i] = direccion[i];
					}
					// opcion 2
					// memcpy(contadorDuplicado, direccion, sizeof(double) * numDatos);
					array[contadorElementsPendientes - 1].dir = contadorDuplicado;
				}
			}

		}
		
		if (contadorMSLlegadaPrimerProd % 7000 == 0) {
			for (size_t i = 0; i < contadorElementsPendientes; i++)
			{
				transfiereDatos(array[i].tiempo, array[i].dir, array[i].num);
				free(array[i].dir);
			}

			free(array);
			array = NULL;
			contadorElementsPendientes = 0;
		}

		sleepCompensado(1);
		contadorMS += 1;
		if (contadorMSLlegadaPrimerProd != -1) contadorMSLlegadaPrimerProd += 1;

		/*
		ESTRUCTURA es1;
		es1.dir = direccion;
		es1.num = numDatos;

		array[indice].dir = es1.dir;
		array[indice].num = es1.num;
		indice += 1;

		sleepCompensado(7000);


		for (int i = 1; i++; i < indice) {
			transfiereDatos(tiempo, array[indice2].dir, *array[indice2].num);
			indice2 += 1;
			array[i].dir = array[i + 1].dir;
			array[i].num = array[i + 1].num;

		}
	}
	*/
		

	}

}