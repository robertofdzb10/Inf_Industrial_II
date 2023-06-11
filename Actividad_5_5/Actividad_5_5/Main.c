#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>


char recogeDatos(double* tiempo, int* valores, int* n);

typedef struct {
	double tiempo; // Tiempo, medido en segundos, que ha tardado en fabricarse el lote actual.
	int* valores;
	int n; // N�mero de valores del array de valores
} Lote;



int main() {
	// Declaraci�n de variables
	double tiempo;
	int valores[20]; // Como no sabemos desde el principio el tama�o de valores, usamos varibales intermedias
	int n;
	int contador = 0; // Contador de n�mero de ejecuciones 
	char devolucion = NULL;
	int numDecimal = 0;
	Lote *lote = NULL;
	FILE* archivo; //Creaci�n de un archivo

	// Bucle de lectura
	while (1) { // Lo recorre hasta encontrar todos los paquetes
		devolucion = recogeDatos(&tiempo, valores, &n); // & direcci�n de memoria de esa variable, al final un puntero es una direcci�n de memoria que indica de donde empieza una variable
		if (devolucion == -2) { // Ya no hay m�s valores
			break;
		} else if (devolucion == -1) { // No entro un valor nuevo
			continue; // Saltar a la siguiente iteraci�n del bucle sin ejecutar el resto del c�digo dentro del mismo
		}

		// Memoria din�mica I
		lote = (Lote*)realloc(lote, (contador + 1) * sizeof(Lote)); // Bloque de memoria anterior , nuevo tama�o (tama�o anterior + 1)
		if (lote == NULL) {
			printf("Error reservando memoria.");
			return 1; // Indica error
		}

		// Asignaci�n de valores
		lote[contador].tiempo = tiempo; // El lote de la iteraci�n correspondiente
		lote[contador].n = n;
		if (lote[contador].n > 20) {
			printf("Error, se han introducido valores de m�s");
			return 1;
		}
		
		// Memoria din�mica II
		lote[contador].valores = (int*)malloc(sizeof(int) * lote[contador].n); // Tama�o del int multiplicado por el valor indicado por n
		if (lote[contador].valores == NULL) { //Es un puntero a un puntero a una estructura, de ah� que no usemos "->"
			printf("Error reservando memoria.");
			return 1;
		}

		// Almacenamiento de valores
		for (int i = 0; i < n; i++) {
			lote[contador].valores[i] = valores[i];
		}

		// Preparaci�n para la siguiente iteraci�n
		contador++;
		
	}

	// Preparaci�n del archivo de escritura
	archivo = fopen("fichero.txt", "a"); // Abre un archivo para escritura, sino existe lo crea, y si ya existe no lo sobreescribe, sino que continua escribiendo donde se quedo
	if (archivo == NULL) {
		printf("La creaci�n o apertura del archivo de texto fallo.");
		return 1;
	}

	// Escritura de datos en el archivo
	for (int i = 0; i < contador; i++) {
		fprintf(archivo, "%03d: %8.3f segundos\n", (i + 1), lote[i].tiempo); // %d n�mero entero, preceder de tres ceros rellenar� con ceros si el numero fuera inferior a tres cifras, %f float, 8.3, son 8 car�cteres y tres son decimales (Sumamos uno a la i ,ya que sino empezar�amos en el n�ermo 0)
		for (int e = 0; e < lote[i].n; e++) {
			fprintf(archivo, "%X ", lote[i].valores[e]); // %x hexadecimal, %X hexadecimal en mayusculas
		}
		if (lote[i].valores[0] > 0) { // Si el primer valor caracter�stico de la pieza es menor a 0 se deja en blanco
			numDecimal = (lote[i].valores[0] >> 4) & 0x7; // Siete es 0111 el resto osn todo ceros, de manera que al hacer una and con ceros el resto de los n�emros de valroes[0] se ponen a cero, quedando solo los tres primeros, que al realizar previamente un desplazamiento son los bits 4,5 y 6
			fprintf(archivo, "\n%d ", numDecimal);
			if (lote[i].valores[1] > 0) { // Si el segundo valor caracter�stico de la pieza es menor a 0 se deja en blanco
				numDecimal = (lote[i].valores[1] >> 8) & 0xFFFF;
				fprintf(archivo, "%d\n\n", numDecimal);
			}
			else {

				fprintf(archivo, "\n\n");
			}
		}
		else {
			fprintf(archivo, "\n\n");
		}
		// Liberaci�n de recursos I
		free(lote[i].valores);
	}

	// Liberaci�n de recursos II
	fclose(archivo);
	free(lote);
}
