#include <stdio.h>

typedef struct {
	double datosAlmacenar[1000]; // Array circular
	int dim; // Dimension del array circular, 1000 en este caso
	int posicionMeter, posicionSacar; // Índices para saber la posición de meter y sacar
}BCAE; // Buffer circular de Almacenanmiento y extracción

int main() {

	// Varaibles
	int i;
	double datos[10]; // Los datos ahora son un array, 10 es el tamño máximo
	double salida[100]; // Array donde se extraeran los datos
	int dimDatos; // Como ahora son un array hemos de saber su dimensión, nos lo pasan como parámetro
	int dimSalida; // Cantidad de datos a extraer
	BCAE par;

	par.dim = sizeof(par.datosAlmacenar) / sizeof(double);
	par.posicionMeter = 0;
	par.posicionSacar = 0;

	for (;;) { // Búcle infinito de captura de datos

		//
		// Capturo datos de una fuente externa, se supone que quedan depsositados en el array "datos" y que el número de ellos es "dimDatos"
		//

		for (i = 0; i < dimDatos; i++) {
			par.datosAlmacenar[par.posicionMeter] = datos[i];
			if ( (par.posicionMeter + 1)%par.dim != par.posicionSacar) {  // Este if busca comprobar que el índice posicionMeter sea distinto de posicionSacar (10001/1000 resto -> 1, por lo que devolvería el valor uno, en caso de superar los límites del array)
				par.posicionMeter = (par.posicionMeter + 1) % par.dim; // Si entra aquí la cola no esta llena
			}
			else {
				printf("La cola esta llena y se procederá a la sobreescritura de los datos.");
				par.posicionMeter = (par.posicionMeter + 1) % par.dim; // Si entra aquí la cola esta llena y se empezarían a perder datos
			}
		}

		//
		// Otro algoritmo sacaría los datos a su paso, se supone que me han dicho que se quieren extraer "dimSalida" datos y que es necesario que se depositen en el array "salida".
		//

		for (i = 0; i < dimSalida; i++) {
			if (par.posicionSacar != par.posicionMeter) { // En el caso de que se cumpliese el array estaría vacio o lleno con sobreescritura
				salida[i] = par.datosAlmacenar[par.posicionSacar];
				par.posicionSacar = (par.posicionSacar + 1) % par.dim; // Aunque no superase la dimensión, esta idea sigue funcionando, 5 / 1000 resto -> 5
			} 
			else { // No hay más datos
				dimSalida = i;
				break;	
			}
		}

		//
		// Aquí se procesarían los datos que se encuentren en el array algo
		//

	}

	return 0;
}
