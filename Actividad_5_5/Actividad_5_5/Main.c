#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>


char recogeDatos(double* tiempo, int* valores, int* n);

typedef struct {
	double tiempo; // Tiempo, medido en segundos, que ha tardado en fabricarse el lote actual.
	int* valores;
	int n; // Número de valores del array de valores
} Lote;



int main() {
	// Declaración de variables
	double tiempo;
	int valores[20]; // Como no sabemos desde el principio el tamaño de valores, usamos varibales intermedias
	int n;
	int contador = 0; // Contador de número de ejecuciones 
	char devolucion = NULL;
	int numDecimal = 0;
	Lote *lote = NULL;
	FILE* archivo; //Creación de un archivo

	// Bucle de lectura
	while (1) { // Lo recorre hasta encontrar todos los paquetes
		devolucion = recogeDatos(&tiempo, valores, &n); // & dirección de memoria de esa variable, al final un puntero es una dirección de memoria que indica de donde empieza una variable
		if (devolucion == -2) { // Ya no hay más valores
			break;
		} else if (devolucion == -1) { // No entro un valor nuevo
			continue; // Saltar a la siguiente iteración del bucle sin ejecutar el resto del código dentro del mismo
		}

		// Memoria dinámica I
		lote = (Lote*)realloc(lote, (contador + 1) * sizeof(Lote)); // Bloque de memoria anterior , nuevo tamaño (tamaño anterior + 1)
		if (lote == NULL) {
			printf("Error reservando memoria.");
			return 1; // Indica error
		}

		// Asignación de valores
		lote[contador].tiempo = tiempo; // El lote de la iteración correspondiente
		lote[contador].n = n;
		if (lote[contador].n > 20) {
			printf("Error, se han introducido valores de más");
			return 1;
		}
		
		// Memoria dinámica II
		lote[contador].valores = (int*)malloc(sizeof(int) * lote[contador].n); // Tamaño del int multiplicado por el valor indicado por n
		if (lote[contador].valores == NULL) { //Es un puntero a un puntero a una estructura, de ahí que no usemos "->"
			printf("Error reservando memoria.");
			return 1;
		}

		// Almacenamiento de valores
		for (int i = 0; i < n; i++) {
			lote[contador].valores[i] = valores[i];
		}

		// Preparación para la siguiente iteración
		contador++;
		
	}

	// Preparación del archivo de escritura
	archivo = fopen("fichero.txt", "a"); // Abre un archivo para escritura, si no existe lo crea, y si ya existe no lo sobreescribe, sino que continua escribiendo donde se quedo
	if (archivo == NULL) {
		printf("La creación o apertura del archivo de texto fallo.");
		return 1;
	}

	// Escritura de datos en el archivo
	for (int i = 0; i < contador; i++) {
		fprintf(archivo, "%03d: %8.3f segundos\n", (i + 1), lote[i].tiempo); // %d número entero, preceder de tres ceros rellenará con ceros si el numero fuera inferior a tres cifras, %f float, 8.3, son 8 carácteres y tres son decimales (Sumamos uno a la i ,ya que sino empezaríamos en el núermo 0)
		for (int e = 0; e < lote[i].n; e++) {
			fprintf(archivo, "%X ", lote[i].valores[e]); // %x hexadecimal, %X hexadecimal en mayusculas
		}
		if (lote[i].valores[0] > 0) { // Si el primer valor característico de la pieza es menor a 0 se deja en blanco
			numDecimal = (lote[i].valores[0] >> 4) & 0x7; // Siete es 0111 el resto osn todo ceros, de manera que al hacer una and con ceros el resto de los núemros de valroes[0] se ponen a cero, quedando solo los tres primeros, que al realizar previamente un desplazamiento son los bits 4,5 y 6
			fprintf(archivo, "\n%d ", numDecimal);
			if (lote[i].valores[1] > 0) { // Si el segundo valor característico de la pieza es menor a 0 se deja en blanco
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
		// Liberación de recursos I
		free(lote[i].valores);
	}

	// Liberación de recursos II
	fclose(archivo);
	free(lote);
}
