#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

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
		//printf("Tiempo esperado: %d\n",espera);
		if (espera > 0) {
			Sleep(espera);
		}
	}
}


typedef struct {
	char *nombre; // Los char almacenan un caracter, por lo que si deseamos alamcenar varios caracteres, usamos cadenas de chars, expresadas en forma de arrays.
	int cantidad;
	double datosPropios;
	long tiempoInFIFO;
	time_t time;
}Pedido;


typedef struct {
	Pedido datosAlmacenar[5]; // Como máximo se podran almacenar 5 pedidos pendientes
	int dim;
	int posicionMeter, posicionSacar;
}BCAE;


int main() {

	// Variables problema
	Pedido pedido_almacenar;
	pedido_almacenar.nombre = NULL;
	pedido_almacenar.cantidad = 0;
	pedido_almacenar.datosPropios = 0.0;
	Pedido pedido_extraer;
	pedido_extraer.nombre = NULL;
	pedido_extraer.cantidad = 0;
	pedido_extraer.datosPropios = 0.0;
	int static contador = 0; // Mientras se espera que salga algun pedido, siguen corriendo los 1,2 segundos de preparación para la salida

	// Varibales FIFO
	BCAE par;
	par.dim = sizeof(par.datosAlmacenar) / sizeof(Pedido);
	par.posicionMeter = 0;
	par.posicionSacar = 0;

	SleepCompensado(0); // T0, antes del búcle

	// Bucle principal 
	while ( !_kbhit() ) {
		pedido_almacenar.nombre = recibePedido(&pedido_almacenar.cantidad, &pedido_almacenar.datosPropios);

		if (pedido_almacenar.nombre == 0) { // Si la dirección que devuelve es cero, pasamos a la proxima iteración
			//SleepCompensado(PERIODO);
			//continue; // Saltamos a la siguiente iteración, sin ejecutar nada del cóidgo de delante, aunque este fuera del if
		}
		else {

			// FIFO Insercción de datos
			par.datosAlmacenar[par.posicionMeter] = pedido_almacenar;
			pedido_almacenar.tiempoInFIFO = clock();

			if ((par.posicionMeter + 1) % par.dim != par.posicionSacar) {
				par.posicionMeter = (par.posicionSacar + 1) % par.dim;
			}
			else { // El índice de meter se solapará con el de escribir, ocasionando una sobreescritura
				pedidoPerdido(); // Llamamos a la función de pedido perdido
				par.posicionMeter = (par.posicionSacar + 1) % par.dim; // Descartamos ese pedido sobreescribiendo
			}
		}

		if (contador > 4 ) { // Cada cuatro iteraciones
			contador++;
			// FIFO Extracción de datos
			if (par.posicionSacar != par.posicionMeter) {
				pedido_extraer = par.datosAlmacenar[par.posicionSacar];
				pedido_extraer.tiempoInFIFO = clock() - pedido_extraer.tiempoInFIFO;
				ponEnMarchaPedido(pedido_extraer.nombre, pedido_extraer.cantidad, &pedido_extraer.datosPropios, pedido_almacenar.dia, pedido_almacenar.mes, pedido_almacenar.ano, pedido_almacenar.hora, pedido_almacenar.minutos, pedido_almacenar.segundos, pedido_extraer.tiempoInFIFO);
				par.posicionSacar = (par.posicionSacar + 1) % par.dim;
				contador = contador - 4;
			}	
		}
		else {
			contador++;
		}
		SleepCompensado(PERIODO); // Al final siempre

		
	}
	return 0;
}