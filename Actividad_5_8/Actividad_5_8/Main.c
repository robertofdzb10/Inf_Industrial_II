#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

int leeCelulaFotoelectrica(void); // 0 paquete 1 no paquete
void leeCodigoBarras(void* datos);

// Si tomamos información de un unsigned long long, su estructura tiene que estar formada por unsigned long long, y de ahí ir dividiendo en bits
typedef struct {

	// Parte Uno
	unsigned long long v0: 1;
	unsigned long long v1 : 2; // Porque sin signo y por que long long y en que ingluye que me digan little endian
	unsigned long long v2 : 32;
	unsigned long long v3 : 10;
	unsigned long long v4 : 7;
	unsigned long long v5_0 : 8;
	unsigned long long v5_1_1 : 4;
	// Parte Dos
	unsigned long long v5_1_2 : 4;
	unsigned long long v5_2 : 8;
	unsigned long long v5_3 : 8;
	unsigned long long v5_4_1 : 44;
	// Parte tres
	unsigned long long v5_4_2 : 20;
	unsigned long long * v5_5; // Su dimensión es este valor dividido entre 8
} Datos;

int main() {

	// Declaración de variables

	// Temporizador
	long periodo = 50; // Valor de cada periodo, ya que la tolerancia máxima es 100ms cogemos 50ms para no jugarnosla a perder valores
	long proximoDisparo = 0; //T0 más periodo
	long espera = 0; // Periodo + T0 - Tiempo de ejecución, si es negativo  no espera

	// Varaibles para funciónes de la librería
	int nuevoPaquete = 1;
	

	Datos paquete;

	// Archivo
	FILE *archivo = fopen("codigoBarras.txt", "w");
	if (archivo == NULL) {
		printf("La creación o apertura del archivo fallo.");
		return 1;
	}

	// Gestión tiempo
	unsigned long dias = 0;
	unsigned long horas = 0;
	unsigned long minutos = 0;
	unsigned long segundos = 0;
	unsigned long milisegundos = 0;

	proximoDisparo = clock(); //Damos a proximoDisparo el valor de T0

	// Bucel principal
	while ( !_kbhit() ) {

		nuevoPaquete = leeCelulaFotoelectrica();
		if (nuevoPaquete == 1) { // No hay paquete
			// Esperar 100ms
			proximoDisparo = periodo + proximoDisparo;
			espera = proximoDisparo - clock();
			if (espera > 0) {
				Sleep(espera);
			}
		}
		else { // Hay paquete

			leeCodigoBarras(&paquete);

			// Guardar en fichero de texto
			milisegundos = (paquete.v2 % 1000);
			segundos = (paquete.v2 / 1000) % 60;
			minutos = (paquete.v2 / (1000 * 60)) % 60;
			horas = (paquete.v2 / (1000 * 60 * 60)) % 24;
			dias = paquete.v2 / (1000 * 60 * 60 * 24);
			
			// La conversion de datos lo hace el fprintf, le pasamos los datos en binario y los convierte al formato correspondiente
			fprintf(archivo, "[%lu;%02lu:%02lu:%02lu,%03lu]: 0x%04X (%d) ", dias, horas, minutos, segundos, milisegundos, paquete.v3, paquete.v4);

			

			if (paquete.v4 == 1) {
				fprintf(archivo, "%u\n", paquete.v5_0);
			}
			else if (paquete.v4 == 2) {
				unsigned int temp;
				temp = paquete.v5_1_2;
				temp = temp << 4;
				temp = temp | paquete.v5_1_1;
				temp = temp << 8; 
				temp = temp | paquete.v5_0;
				fprintf(archivo, "%u\n", temp);
			}
			else if (paquete.v4 == 3) {
				unsigned int temp;
				temp = paquete.v5_2;
				temp = temp << 4;
				temp = temp | paquete.v5_1_2;
				temp = temp << 4;
				temp = temp | paquete.v5_1_1;
				temp = temp << 8; 
				temp = temp | paquete.v5_0;
				fprintf(archivo, "%u\n", temp);
			}
			else if (paquete.v4 == 4) {
				unsigned int temp;
				temp = paquete.v5_3;
				temp = temp << 8;
				temp = temp | paquete.v5_2;
				temp = temp << 4;
				temp = temp | paquete.v5_1_2;
				temp = temp << 4;
				temp = temp | paquete.v5_1_1;
				temp = temp << 8;
				temp = temp | paquete.v5_0;
				fprintf(archivo, "%u\n", temp);
			}
			else if (paquete.v4 == 8) {
				unsigned long long temp;
				temp = paquete.v5_4_2;
				temp = temp << 44;
				temp = temp | paquete.v5_4_1;
				temp = temp << 8;
				temp = temp | paquete.v5_3;
				temp = temp << 8;
				temp = temp | paquete.v5_2;
				temp = temp << 4;
				temp = temp | paquete.v5_1_2;
				temp = temp << 4;
				temp = temp | paquete.v5_1_1;
				temp = temp << 8;
				temp = temp | paquete.v5_0;
				fprintf(archivo, "%lf\n", temp);
			}
			else if (paquete.v4 >= 16) {
				int dimension = paquete.v4 / 8;

				//char temp[dimension]; // Array de bytes, valor variable, por eso usamos memoría dinámica 

				for (int i = 0; i < dimension; i++) {
				}
				fprintf(archivo, "%d\n", paquete.v5_0);
			}

			// Esperar 500 ms
			proximoDisparo = 10 * periodo + proximoDisparo;
			espera = proximoDisparo - clock();
			if (espera > 0) {
				Sleep(espera);
			}
		}
	}

	// Gestión final de memoría dinámica y archivos
	fclose(archivo);
	// Fin
	return 0;
}
