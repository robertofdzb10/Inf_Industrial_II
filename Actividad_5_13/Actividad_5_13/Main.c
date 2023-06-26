#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define PERIODO 100

void leeSensores(int* Plat_0, int* Plat_1, int* Cel_0, int* Cel_1, int* setaEmerg);

unsigned long leeCodigoBarras(int n_lector);

void ponAlarma(int estadoAlarma);

void ponMotor(int giroMotor);

void SleepCompensado(DWORD time){
	long espera;
	static proximoDisparo = -1;
	if (time == 0) {
		proximoDisparo = clock();
	}
	else {
		proximoDisparo = time + proximoDisparo;
		espera = proximoDisparo - clock();
		if (espera > 0) {
			Sleep(espera);
		}
	}
}

int main() {
	// Varaibles
	enum State{Inicio = 0, MotorIzq = 1, MotorDcha = 2, LecturaCodigoIzq = 3, LecturaCodigoDcha = 4, EsperaIzq = 5, EsperaDcha = 6, Alarma = 7, AlarmaOffEspera = 8 , Paro = 9, Fin = 10};
	enum State currentState = Inicio;

	int salir = 0;

	int Plat_0 = 0;
	int Plat_1 = 0;
	int Cel_0 = 0;
	int Cel_1 = 0;
	int setaEmerg = 0;

	int giroMotor = 0;
	int estadoAlarma;

	int contador = 0;

	unsigned long*arrayIzq = (unsigned long*)malloc(sizeof(unsigned long));
	unsigned long*arrayDcha = (unsigned long*)malloc(sizeof(unsigned long));
	if ( (arrayIzq == NULL) || (arrayDcha == NULL) ) {
		printf("Error en la asignación de memoria dinámica.");
		return 1;
	}

	unsigned long codigo = 1;

	int dim_0 = 1;
	int dim_1 = 1;

	SleepCompensado(0); // T0
	while (salir != 1) {
		leeSensores(&Plat_0, &Plat_1, &Cel_0, &Cel_1, &setaEmerg);
		switch (currentState)
		{
		case Inicio:
			giroMotor = 0;
			estadoAlarma = 0;
			if (Plat_0 == 1) {
				currentState = MotorDcha;
			}
			else if (Plat_1 == 1) {
				currentState = MotorIzq;
			}
			break;
		case MotorIzq:
			giroMotor = -1;
			estadoAlarma = 0;
			if (setaEmerg == 0) {
				currentState = Alarma;
			}
			else if (Cel_0 == 1) {
				currentState = EsperaIzq;
			}
			break;
		case MotorDcha:
			giroMotor = 1;
			estadoAlarma = 0;
			if (setaEmerg == 0) {
				currentState = Alarma;
			}
			else if (Cel_1 == 1) {
				currentState = EsperaDcha;
			}
			break;
		case EsperaIzq:
			giroMotor = -1;
			estadoAlarma = 0;
			if (setaEmerg == 0) {
				currentState = Alarma;
			}
			else if (Cel_0 == 0) {
				currentState = LecturaCodigoIzq;
			}
			break;
		case EsperaDcha:
			giroMotor = 1;
			estadoAlarma = 0;
			if (setaEmerg == 0) {
				currentState = Alarma;
			}
			else if (Cel_1 == 0) {
				currentState = LecturaCodigoDcha;
			}
			break;
		case LecturaCodigoIzq:
			estadoAlarma = 0;			
			codigo = leeCodigoBarras(0);
			arrayIzq[dim_0 - 1] = codigo;
			dim_0 = dim_0 + 1;
			arrayIzq = (unsigned long*)realloc(arrayIzq, (dim_0 * sizeof(unsigned long)) );
			if (arrayIzq == NULL) {
				printf("Error en la asignación de memoria dinámica.");
				return 1;
			}
			if (setaEmerg == 0) {
				currentState = Alarma;
			}
			if (codigo == 0xffffffff) {
				currentState = Fin;
			}
			else if (codigo != 0xffffffff) {
				currentState = Paro;
			}
			break;
		case LecturaCodigoDcha:
			estadoAlarma = 0;
			codigo = leeCodigoBarras(1);
			arrayDcha[dim_1 - 1] = codigo;
			dim_1 = dim_1 + 1;
			arrayDcha = (unsigned long*)realloc(arrayDcha, (dim_1 * sizeof(unsigned long)) );
			if (arrayDcha == NULL) {
				printf("Error en la asignación de memoria dinámica.");
				return 1;
			}
			if (setaEmerg == 0) {
				currentState = Alarma;
			}
			if (codigo == 0xffffffff) {
				currentState = Fin;
			}
			else if (codigo != 0xffffffff) {
				currentState = Paro;
			}
			break;
		case Alarma:
			giroMotor = 0;
			estadoAlarma = 1;
			if (setaEmerg == 1) {
				currentState = AlarmaOffEspera;
			}
			break;
		case AlarmaOffEspera:
			giroMotor = 0;
			estadoAlarma = 0;
			currentState = Inicio;
			break;
		case Paro:
			if (contador == 10) {
				giroMotor = 0;
				estadoAlarma = 0;
				contador = 0;
				currentState = Inicio;	
			}
			else {
				contador++;
			}
			break;
		case Fin:
			if (contador == 10) {
				giroMotor = 0;
				contador = 0;
				salir = 1;	
			}
			else {
				contador++;
			}
			break;
		default:
			printf("Estado imposible, no debería suceder nunca, volvemos al inicio\n");
			currentState = Inicio;
			break;
		}
		// Funcionalidades
		ponAlarma(estadoAlarma);
		ponMotor(giroMotor);
		SleepCompensado(PERIODO);
	}
	dim_1 = dim_1 - 1; // Restamos la última dimensión ya que no la usamos, debido a que termino el programa
	dim_0 = dim_0 - 1;
	printf("\n\n");
	printf("Piezas que han viajado hacia la derecha:\n");
	printf("----------------------------------------:\n");
	for (int i = 0; i < dim_1; i++) {
		printf("Caja %d, Codigo: 0x%x\n",i, arrayDcha[i]);
	}
	printf("\n\n");
	printf("Piezas que han viajado hacia la izquierda:\n");
	printf("------------------------------------------:\n");
	for (int i = 0; i < dim_0; i++) {
		printf("Caja %d, Codigo: 0x%x\n", i, arrayIzq[i]);
	}
	printf("\n\n\n\n");
	free(arrayDcha);
	free(arrayIzq);
	return 0;
}