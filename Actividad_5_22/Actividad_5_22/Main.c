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
		proximoDisparo = tiempo + proximoDisparo; // T0 (o T0 m�s tiempo del anterior ciclo) m�s el tiempo que tarda el ciclo actual
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
	Pedido datosAlmacenar[5]; // Como m�ximo se podran almacenar 5 pedidos pendientes
	int dim;
	int posicionMeter, posicionSacar;
}BCAE;


int main() {

	// Variables problema
	int cantidad_p_t = 0;
	double* datosPropios_p_t[100]; // Le damos un tam�o grande (en este caso el m�ximo) para que todos los datos esten dentro del double ya que sino al estar fuera pueden ser reescritos por otros elementos
	Pedido pedido_almacenar;
	pedido_almacenar.nombre = NULL;
	pedido_almacenar.cantidad = 0;
	pedido_almacenar.datosPropios = NULL;
	Pedido pedido_extraer;
	pedido_extraer.nombre = NULL;
	pedido_extraer.cantidad = 0;
	pedido_extraer.datosPropios = NULL;
	int static contador = 0; // Mientras se espera que salga algun pedido, siguen corriendo los 1,2 segundos de preparaci�n para la salida

	// Varibales FIFO
	BCAE par;
	par.dim = sizeof(par.datosAlmacenar) / sizeof(Pedido);
	par.posicionMeter = 0;
	par.posicionSacar = 0;

	SleepCompensado(0); // T0, antes del b�cle

	// Bucle principal 
	while ( !_kbhit() ) {
		pedido_almacenar.nombre = recibePedido(&cantidad_p_t, &datosPropios_p_t); // Pasamos un puntero, y hacemos que la direcci�n de este puntero sea la de retorno, almacenando as� los valores. En verdad solo se almacenar� la direcci�n donde comienzan los valores, la cual ser� la nueva direcci�n de datosPropios_p_t, siendo el valor real de este de datosPropios_p_t el principio de la cadena, pero no entera.

		if (pedido_almacenar.nombre == 0) { // Si la direcci�n que devuelve es cero, pasamos a la proxima iteraci�n
		}
		else {
			// FIFO Insercci�n de datos
			pedido_almacenar.tiempoInFIFO = clock();
			pedido_almacenar.cantidad = cantidad_p_t;
			// Memoria din�mica 
			pedido_almacenar.datosPropios = (double*)malloc(sizeof(double) * pedido_almacenar.cantidad);
			if (pedido_almacenar.datosPropios == NULL) {
				printf("Error en la asignaci�n de memoria din�mica");
				return 1;
			}
	
			memcpy(pedido_almacenar.datosPropios, &datosPropios_p_t, sizeof(double)* pedido_almacenar.cantidad); // Copiamos sizeof(double) * pedido_almacenar.cantidad bytes, desde la direcci�n apuntada por datosPropios_p_t a la cadena pedido_almacenar.datosPropios
			time_t tiempo; // Declaraci�n de la variable time_t
			time(&tiempo); // Esto devuelve el n�mero de segundos que han trascurrido desde 1970
			pedido_almacenar.time = localtime(&tiempo); // Convierte los segundos desde 1970 a a�o, mes, d�as...
			par.datosAlmacenar[par.posicionMeter].datosPropios = (double*)malloc(sizeof(double) * pedido_almacenar.cantidad);
			if (par.datosAlmacenar[par.posicionMeter].datosPropios == NULL) {
				printf("Error en la asignaci�n de memoria din�mica");
				return 1;
			}
			// Pasamos los datos, si hicieramos asignaci�n directa, si alguno de los miembros de la estructura es un puntero, se copiar� el valor del puntero y no los datos a los que apunta
			par.datosAlmacenar[par.posicionMeter].nombre = pedido_almacenar.nombre;
			par.datosAlmacenar[par.posicionMeter].cantidad = pedido_almacenar.cantidad;
			par.datosAlmacenar[par.posicionMeter].tiempoInFIFO = pedido_almacenar.tiempoInFIFO;
			par.datosAlmacenar[par.posicionMeter].time = pedido_almacenar.time;
			memcpy(par.datosAlmacenar[par.posicionMeter].datosPropios, pedido_almacenar.datosPropios, sizeof(double) * pedido_almacenar.cantidad);
			if ((par.posicionMeter + 1) % par.dim != par.posicionSacar) {
				par.posicionMeter = (par.posicionSacar + 1) % par.dim;
			}
			else { // El �ndice de meter se solapar� con el de escribir, ocasionando una sobreescritura
				pedidoPerdido(); // Llamamos a la funci�n de pedido perdido
				par.posicionMeter = (par.posicionSacar + 1) % par.dim; // Descartamos ese pedido sobreescribiendo
			}
			// Liberaci�n de memoria din�mica
			free(pedido_almacenar.datosPropios);
		}

		if (contador > 4 ) { // Cada cuatro iteraciones
			contador++;
			// FIFO Extracci�n de datos
			if (par.posicionSacar != par.posicionMeter) {
				// Memor�a din�mica 
				pedido_extraer.cantidad = par.datosAlmacenar[par.posicionSacar].cantidad;
				pedido_extraer.datosPropios = (double*)malloc(sizeof(double) * pedido_extraer.cantidad);
				if (pedido_extraer.datosPropios == NULL) {
					printf("Error en la asignaci�n de memoria din�mica");
					return 1;
				}
				// Pasamos los datos
				pedido_extraer.nombre = par.datosAlmacenar[par.posicionSacar].nombre;
				pedido_extraer.tiempoInFIFO = par.datosAlmacenar[par.posicionSacar].tiempoInFIFO;
				pedido_extraer.time = par.datosAlmacenar[par.posicionSacar].time;
				memcpy(pedido_extraer.datosPropios, par.datosAlmacenar[par.posicionSacar].datosPropios, sizeof(double) * pedido_extraer.cantidad);
				// Liberaci�n de memoria din�mica
				free(par.datosAlmacenar[par.posicionSacar].datosPropios);
				pedido_extraer.tiempoInFIFO = clock() - pedido_extraer.tiempoInFIFO;
				ponEnMarchaPedido(pedido_extraer.nombre, pedido_extraer.cantidad, pedido_extraer.datosPropios, pedido_extraer.time->tm_mday, pedido_extraer.time->tm_mon+1, pedido_extraer.time->tm_year+1900, pedido_extraer.time->tm_hour, pedido_extraer.time->tm_min, pedido_extraer.time->tm_sec, pedido_extraer.tiempoInFIFO); // Sumamos uno al mes ya que de enero a febrero (ej.) a pasado un mes pero es el mes dos
				par.posicionSacar = (par.posicionSacar + 1) % par.dim;
				contador = contador - 4;
				// Liberaci�n de memoria din�mica
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