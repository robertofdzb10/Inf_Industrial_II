#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define PERIODO 50

long sincronizaRelojes(void);

long productoEnviado(void);

void manejaPortezuela(int abrir);

double* leeDatos(int* dim);

void transfiereDatos(long tiempo, double* par, int dim);

void SleepCompensado(DWORD tiempo) {
	long espera;
	static proximoDisparo = -1;
	if (tiempo == 0) {
		proximoDisparo = clock();
	}
	else {
		proximoDisparo = tiempo + proximoDisparo;
		espera = proximoDisparo - clock();
		if (espera > 0) {

			Sleep(espera);
		}
	}
}

typedef struct {
	long tiempo;
	double* par;
	int dim;
} Datos;



int main() {


	// Declaración de variables
	long producto_bool = -1;
	long tiempo_esperar = 0;
	long tiempo_actual = 0;
	int dim_t = 0;
	int puerta_abierta = 0;
	int contador_sleep = 0;
	double* datos_t; 
	long vaciado = 0;

	Datos *datosAlmacen = (Datos*)malloc(sizeof(Datos));
	if (datosAlmacen == NULL) {
		printf("Error a la hora de gestionar la memoría dinámica");
	}
	int sizeDatos = 0;
	

	SleepCompensado(0);

	while ( !_kbhit() ) {
		producto_bool = productoEnviado();
		if (producto_bool != -1 && puerta_abierta == 0) {
			contador_sleep = 0;
			if (vaciado == 0) {
				vaciado = clock();	
			}
			tiempo_esperar = producto_bool - sincronizaRelojes();
			Sleep(tiempo_esperar); // No es compensado ya que no es de este PC el tiempo
			manejaPortezuela(1);
			puerta_abierta = 1;
		}
		contador_sleep++;

		if (tiempo_esperar != 0) { // Hay que llegar a 600
			SleepCompensado(0); // Hay un nuevo T0
			tiempo_esperar = 0;
		}
		else {
			SleepCompensado(50);
		}


		if (contador_sleep >= 10 && puerta_abierta == 1) {
			manejaPortezuela(0);
			datos_t = leeDatos(&dim_t);
			sizeDatos++;
			datosAlmacen = (Datos*)realloc(datosAlmacen, sizeof(Datos) * sizeDatos);
			if (datosAlmacen == NULL) {
				printf("Error a la hora de gestionar la memoría dinámica");
			}
			datosAlmacen[sizeDatos - 1].dim = dim_t;
			datosAlmacen[sizeDatos - 1].tiempo = clock();
			datosAlmacen[sizeDatos - 1].par = (double*)malloc(sizeof(double) * dim_t);
			if (datosAlmacen[sizeDatos - 1].par == NULL) {
				printf("Error a la hora de gestionar la memoría dinámica");
			}
			memcpy(datosAlmacen[sizeDatos - 1].par, datos_t, sizeof(double) * dim_t);
			datosAlmacen[sizeDatos - 1].par = datos_t;
			contador_sleep = 0;
			puerta_abierta = 0;
		}
		if (clock() - vaciado >= 7000 && vaciado != 0 ) { // 7s
			for (int i = 0; i < sizeDatos; i++) {
				transfiereDatos(datosAlmacen[i].tiempo, datosAlmacen[i].par, datosAlmacen[i].dim);
				//free(datosAlmacen[i].par);
			}
			sizeDatos = 0;
			vaciado = 0;
		}
	}
	if (sizeDatos > 0){
		for (int i = 0; i < sizeDatos; i++) {
			transfiereDatos(datosAlmacen[i].tiempo, datosAlmacen[i].par, datosAlmacen[i].dim);
			//free(datosAlmacen[i].par);
		}
	}
	free(datosAlmacen);
	return 0;
}