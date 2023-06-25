#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define PERIODO 300

char* recibePedido(int* cantidad, double* datosPropios);

void pedidoPerdido(void);

void ponEnMarchaPedido(char* nombre, int cantidad, double* datosPropios, int dia, int mes, int ano, int hora, int minutos, int segundos, long tiempoPreparacion);


void SleepCompensado(DWORD tiempo) {

	long espera;
	static proximoDisparo = -1;

	if (tiempo == 0) {
		proximoDisparo = clock(); // T0
	}
	else {
		proximoDisparo = tiempo + proximoDisparo; // T0 (o T0 más tiempo del anterior ciclo) más el tiempo que tarda el ciclo actual
		espera = proximoDisparo - clock();
		if (espera > 0) {
			Sleep(espera);
		}
	}
}


typedef struct {
	char *nombre; // Los char almacenan un caracter, por lo que si deseamos alamcenar varios caracteres, usamos cadenas de chars, expresadas en forma de arrays.
	int cantidad;
	double *datosPropios;
	long tiempoInFIFO;
	struct tm *time;
}Pedido;


typedef struct {
	Pedido datosAlmacenar[5]; // Como máximo se podran almacenar 5 pedidos pendientes
	int dim;
	int posicionMeter, posicionSacar;
}BCAE;


int main() {

	// Variables problema
	int cantidad_p_t = 0;
	double* datosPropios_p_t[100]; // Le damos un tamño grande (en este caso el máximo) para que todos los datos esten dentro del double ya que sino al estar fuera pueden ser reescritos por otros elementos
	Pedido pedido_almacenar;
	pedido_almacenar.nombre = NULL;
	pedido_almacenar.cantidad = 0;
	pedido_almacenar.datosPropios = NULL;
	Pedido pedido_extraer;
	pedido_extraer.nombre = NULL;
	pedido_extraer.cantidad = 0;
	pedido_extraer.datosPropios = NULL;
	int static contador = 0; // Mientras se espera que salga algun pedido, siguen corriendo los 1,2 segundos de preparación para la salida

	// Varibales FIFO
	BCAE par;
	par.dim = sizeof(par.datosAlmacenar) / sizeof(Pedido);
	par.posicionMeter = 0;
	par.posicionSacar = 0;

	SleepCompensado(0); // T0, antes del búcle

	// Bucle principal 
	while ( !_kbhit() ) {
		pedido_almacenar.nombre = recibePedido(&cantidad_p_t, &datosPropios_p_t); // Pasamos un puntero, y hacemos que la dirección de este puntero sea la de retorno, almacenando así los valores. En verdad solo se almacenará la dirección donde comienzan los valores, la cual será la nueva dirección de datosPropios_p_t, siendo el valor real de este de datosPropios_p_t el principio de la cadena, pero no entera.

		if (pedido_almacenar.nombre == 0) { // Si la dirección que devuelve es cero, pasamos a la proxima iteración
		}
		else {
			// FIFO Insercción de datos
			pedido_almacenar.tiempoInFIFO = clock();
			pedido_almacenar.cantidad = cantidad_p_t;
			// Memoria dinámica 
			pedido_almacenar.datosPropios = (double*)malloc(sizeof(double) * pedido_almacenar.cantidad);
			if (pedido_almacenar.datosPropios == NULL) {
				printf("Error en la asignación de memoria dinámica");
				return 1;
			}
	
			memcpy(pedido_almacenar.datosPropios, &datosPropios_p_t, sizeof(double)* pedido_almacenar.cantidad); // Copiamos sizeof(double) * pedido_almacenar.cantidad bytes, desde la dirección apuntada por datosPropios_p_t a la cadena pedido_almacenar.datosPropios
			time_t tiempo; // Declaración de la variable time_t
			time(&tiempo); // Esto devuelve el número de segundos que han trascurrido desde 1970
			pedido_almacenar.time = localtime(&tiempo); // Convierte los segundos desde 1970 a año, mes, días...
			par.datosAlmacenar[par.posicionMeter].datosPropios = (double*)malloc(sizeof(double) * pedido_almacenar.cantidad);
			if (par.datosAlmacenar[par.posicionMeter].datosPropios == NULL) {
				printf("Error en la asignación de memoria dinámica");
				return 1;
			}
			// Pasamos los datos, si hicieramos asignación directa, si alguno de los miembros de la estructura es un puntero, se copiará el valor del puntero y no los datos a los que apunta
			par.datosAlmacenar[par.posicionMeter].nombre = pedido_almacenar.nombre;
			par.datosAlmacenar[par.posicionMeter].cantidad = pedido_almacenar.cantidad;
			par.datosAlmacenar[par.posicionMeter].tiempoInFIFO = pedido_almacenar.tiempoInFIFO;
			par.datosAlmacenar[par.posicionMeter].time = pedido_almacenar.time;
			memcpy(par.datosAlmacenar[par.posicionMeter].datosPropios, pedido_almacenar.datosPropios, sizeof(double) * pedido_almacenar.cantidad);
			if ((par.posicionMeter + 1) % par.dim != par.posicionSacar) {
				par.posicionMeter = (par.posicionSacar + 1) % par.dim;
			}
			else { // El índice de meter se solapará con el de escribir, ocasionando una sobreescritura
				pedidoPerdido(); // Llamamos a la función de pedido perdido
				par.posicionMeter = (par.posicionSacar + 1) % par.dim; // Descartamos ese pedido sobreescribiendo
			}
			// Liberación de memoria dinámica
			free(pedido_almacenar.datosPropios);
		}

		if (contador > 4 ) { // Cada cuatro iteraciones
			contador++;
			// FIFO Extracción de datos
			if (par.posicionSacar != par.posicionMeter) {
				// Memoría dinámica 
				pedido_extraer.cantidad = par.datosAlmacenar[par.posicionSacar].cantidad;
				pedido_extraer.datosPropios = (double*)malloc(sizeof(double) * pedido_extraer.cantidad);
				if (pedido_extraer.datosPropios == NULL) {
					printf("Error en la asignación de memoria dinámica");
					return 1;
				}
				// Pasamos los datos
				pedido_extraer.nombre = par.datosAlmacenar[par.posicionSacar].nombre;
				pedido_extraer.tiempoInFIFO = par.datosAlmacenar[par.posicionSacar].tiempoInFIFO;
				pedido_extraer.time = par.datosAlmacenar[par.posicionSacar].time;
				memcpy(pedido_extraer.datosPropios, par.datosAlmacenar[par.posicionSacar].datosPropios, sizeof(double) * pedido_extraer.cantidad);
				// Liberación de memoria dinámica
				free(par.datosAlmacenar[par.posicionSacar].datosPropios);
				pedido_extraer.tiempoInFIFO = clock() - pedido_extraer.tiempoInFIFO;
				ponEnMarchaPedido(pedido_extraer.nombre, pedido_extraer.cantidad, pedido_extraer.datosPropios, pedido_extraer.time->tm_mday, pedido_extraer.time->tm_mon+1, pedido_extraer.time->tm_year+1900, pedido_extraer.time->tm_hour, pedido_extraer.time->tm_min, pedido_extraer.time->tm_sec, pedido_extraer.tiempoInFIFO); // Sumamos uno al mes ya que de enero a febrero (ej.) a pasado un mes pero es el mes dos
				par.posicionSacar = (par.posicionSacar + 1) % par.dim;
				contador = contador - 4;
				// Liberación de memoria dinámica
				free(pedido_extraer.datosPropios);
			}	
		}
		else {
			contador++;
		}
		SleepCompensado(PERIODO); // Al final siempre
	}
	return 0;
}