#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>

char* recibePedido();
void pedidoPerdido();
void ponEnMarchaPedido();

void SleepCompensado(DWORD tiempo) {
	long espera;
	static long proxDisparo = -1;

	if (tiempo == 0) {
		proxDisparo = clock();
	}
	else {
		if (proxDisparo == -1) proxDisparo = clock();
		proxDisparo += tiempo;
		espera = proxDisparo - clock();
		if (espera > 0) Sleep(espera);
	}
}


typedef struct pedido {
	struct tm *fecha_recep;
	long tiempo;
	char* nombre;
	int cantidad;
	double datosPropios[100];
}PEDIDO;


int main() {
	int llena = 0;
	int meter = 0;
	int sacar = 0;
	int ciclos = 0;
	PEDIDO pedidos[6];
	while (!_kbhit()) {
		PEDIDO ped;
		ped.nombre = recibePedido(&ped.cantidad, ped.datosPropios);
		if (ped.nombre != NULL) {
			time_t hora;
			time(&hora);
			ped.fecha_recep = localtime(&hora);
			ped.tiempo = clock();

			//Meter en lista
			if ((meter + 1) % 5 != sacar) {
				pedidos[meter] = ped;
				meter = (meter + 1) % 5;
				llena = 0;
			}
			else {
				if (llena == 0) {
					pedidos[meter] = ped;
				}
				else {
					pedidoPerdido();
				}
				llena = 1;
			}
		}
		//Procesar pedidos
		if (ciclos==4) {
			ciclos = 0;
			if (sacar % 5 != meter) {
				pedidos[sacar].tiempo = clock() - pedidos[sacar].tiempo;
				ponEnMarchaPedido(pedidos[sacar].nombre, pedidos[sacar].cantidad, pedidos[sacar].datosPropios, pedidos[sacar].fecha_recep->tm_mday, pedidos[sacar].fecha_recep->tm_mon, pedidos[sacar].fecha_recep->tm_year, pedidos[sacar].fecha_recep->tm_hour, pedidos[sacar].fecha_recep->tm_min, pedidos[sacar].fecha_recep->tm_sec, pedidos[sacar].tiempo);
				sacar = (sacar + 1) % 5;
			}
			
		}
		else {
			ciclos++;
		}
		SleepCompensado(300);
		
	}
	return 0;
}