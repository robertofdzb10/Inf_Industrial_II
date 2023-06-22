// Array que se va llenando y vaciando, se usa cuando solo queremos operar con los �ltimos datos obtenidos, 
// Por ejemplo un sensor que hace 10.000 medicones/s y nosotros solo queremos ver las mediciones a partid del segundo 20

#include <stdio.h>

typedef struct {
	double datosAlmacenar[1000]; // Array circular
	int dim; // Dimensi�n del array circular, 1000 en este caso
	int posicion; // Posici�n actual del buffer, para recorrerlo
	int nCapturas; // Capturas totales de datos del sensor, aunque el array se sobreescriba, en esta variable se podra ver cuantos datos se han ido almacenando
} BCA; // Buffer Circular Array

int main() {
	
	// Varaibles
	int i;
	double dato;
	BCA par;

	par.dim = sizeof(par.datosAlmacenar) / sizeof(double); // Esto nos dar� 1000 en este caso
	par.posicion = 0; // Inicializamos a 0
	par.nCapturas = 0; // Inicializamos a 0

	for (;;) { // B�cle infinito de captura de datos

		//
		// Capturo del sensor, se supone que queda depsositado en la variable "dato".
		//

		par.datosAlmacenar[par.posicion] = dato;
		if (++(par.posicion) >= par.dim) { // Movimiento del indice hasta llegar al final
			par.posicion = 0;  // Una vez llega al final, restablecemos
		}
		++(par.nCapturas); // Aumentamos el valor de datos tomados

		//
		// Se verifica si se cumple la condici�n para salir del bucle infinito
		//
		
		//
		// Se espera el tiempo necesario entre muestras
		//

	}
	for (i = par.posicion; i < par.dim; i++) {
		//
		// Se lleva a cabo el algoritmo deseado con los datos "m�s antiguos".
		//
	}
	for (i = 0; i < par.posicion; i++) {
		//
		// Se lleva a cabo el algoritmo deseado con los datos "m�s nuevos".
		//
	}
	return 0;
}