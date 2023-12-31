#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

void recogeDatos(int* dispositivo, unsigned char* buffer, int* dim);

void enviaDatos(int dispositivo, unsigned char* buffer, int dim);

typedef struct{
	int dispositivo;
	int dim;
	unsigned char tiempo;
	unsigned char* buffer;
} Datos;

int main() {

	// Variables del problema 
	int dispositivo_t = 0;
	unsigned char buffer_t[30];
	int dim_t = 0;

	Datos* datosAlmacen = (Datos*)malloc(sizeof(Datos));
	if (datosAlmacen == NULL) {
		printf("Error al asignar la memoria dinamica");
	}
	int punteroSacar = 0;
	int punteroMeter = 0;

	int sacar = 0;
	
	int contador_iteraciones = 1; // Crear un contador de iteraciones 

	unsigned char variable_temp_1;
	unsigned char variable_temp_2;

	long tiempoInicio = clock();

	while ( 1 ) {
		recogeDatos(&dispositivo_t, buffer_t, &dim_t);
		if (dim_t == 0) {
		}
		else {
			printf("%d\n",punteroMeter);
			datosAlmacen[punteroMeter].buffer = (unsigned char*)malloc(sizeof(unsigned char)* dim_t);
			if (datosAlmacen[punteroMeter].buffer == NULL) {
				printf("Error al asignar la memoria dinamica");
			}
			datosAlmacen[punteroMeter].dim = dim_t;
			datosAlmacen[punteroMeter].dispositivo = dispositivo_t;
			memcpy(datosAlmacen[punteroMeter].buffer, buffer_t, dim_t * sizeof(unsigned char));
			datosAlmacen[punteroMeter].tiempo = clock();
			//for (int i = 0; i < datosAlmacen[punteroMeter].dim; i++){
			//	printf("%d\n", datosAlmacen[punteroMeter].buffer[i]);
			//}
			punteroMeter++;
			contador_iteraciones++;
			datosAlmacen = (Datos*)realloc(datosAlmacen, (contador_iteraciones * sizeof(Datos)) ); //Sizeof datos almacen me devuelve el tama�o del puntero
			if (datosAlmacen == NULL) {
				printf("Error al asignar la memoria dinamica");
			}
		}

		if (datosAlmacen[punteroSacar].dispositivo == 4 && tiempoInicio == 1187) {
			sacar = 1;
			tiempoInicio = 0;
			printf("Hola1");
		}
		else if (datosAlmacen[punteroSacar].dispositivo == 1 && tiempoInicio == 991) {
			sacar = 1;
			printf("Hola2");
		}
		else if (datosAlmacen[punteroSacar].dispositivo == 2 && tiempoInicio == 829) {
			sacar = 1;
			printf("Hola3");
		}
		else if (datosAlmacen[punteroSacar].dispositivo == 3 && tiempoInicio == 613) {
			sacar = 1;
			printf("Hola4");
		}

		if (sacar == 1) {
			// Movemos uno el array para sumar el tiempo
			datosAlmacen[punteroSacar].dim = datosAlmacen[punteroSacar].dim + 1;
			datosAlmacen[punteroSacar].buffer = (unsigned char*)realloc(datosAlmacen[punteroSacar].buffer, sizeof(unsigned char) * datosAlmacen[punteroSacar].dim);
			if (datosAlmacen[punteroSacar].buffer == NULL) {
				printf("Error al asignar la memoria dinamica");
			}
			variable_temp_2 = datosAlmacen[punteroSacar].buffer[0];
			for (int i = 0; i < datosAlmacen[punteroSacar].dim; i++) {
				variable_temp_1 = datosAlmacen[punteroSacar].buffer[i];
				datosAlmacen[punteroSacar].buffer[i + 1] = variable_temp_2;
				variable_temp_2 = variable_temp_1;
			}
			datosAlmacen[punteroSacar].buffer[0] = datosAlmacen[punteroSacar].tiempo;
			enviaDatos(datosAlmacen[punteroSacar].dispositivo, datosAlmacen[punteroSacar].buffer, datosAlmacen[punteroSacar].dim);
			punteroSacar++;
			sacar = 0;
			free(datosAlmacen[punteroSacar].buffer);
		}

	}
	free(datosAlmacen);
	return 0;
}  