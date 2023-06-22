#include <stdio.h>

typedef struct {
	double datosAlmacenar[1000]; // Array circular
	int dim; // Dimension del array circular, 1000 en este caso
	int posicionMeter, posicionSacar; // �ndices para saber la posici�n de meter y sacar
}BCAE; // Buffer circular de Almacenanmiento y extracci�n

int main() {

	// Varaibles
	int i;
	double datos[10]; // Los datos ahora son un array, 10 es el tam�o m�ximo
	double salida[100]; // Array donde se extraeran los datos
	int dimDatos; // Como ahora son un array hemos de saber su dimensi�n, nos lo pasan como par�metro
	int dimSalida; // Cantidad de datos a extraer
	BCAE par;

	par.dim = sizeof(par.datosAlmacenar) / sizeof(double);
	par.posicionMeter = 0;
	par.posicionSacar = 0;

	for (;;) { // B�cle infinito de captura de datos

		//
		// Capturo datos de una fuente externa, se supone que quedan depsositados en el array "datos" y que el n�mero de ellos es "dimDatos"
		//

		for (i = 0; i < dimDatos; i++) {
			par.datosAlmacenar[par.posicionMeter] = datos[i];
			if ( (par.posicionMeter + 1)%par.dim != par.posicionSacar) {  // Este if busca comprobar que el �ndice posicionMeter sea distinto de posicionSacar (10001/1000 resto -> 1, por lo que devolver�a el valor uno, en caso de superar los l�mites del array)
				par.posicionMeter = (par.posicionMeter + 1) % par.dim; // Si entra aqu� la cola no esta llena
			}
			else {
				printf("La cola esta llena y se proceder� a la sobreescritura de los datos.");
				par.posicionMeter = (par.posicionMeter + 1) % par.dim; // Si entra aqu� la cola esta llena y se empezar�an a perder datos
			}
		}

		//
		// Otro algoritmo sacar�a los datos a su paso, se supone que me han dicho que se quieren extraer "dimSalida" datos y que es necesario que se depositen en el array "salida".
		//

		for (i = 0; i < dimSalida; i++) {
			if (par.posicionSacar != par.posicionMeter) { // En el caso de que se cumpliese el array estar�a vacio o lleno con sobreescritura
				salida[i] = par.datosAlmacenar[par.posicionSacar];
				par.posicionSacar = (par.posicionSacar + 1) % par.dim; // Aunque no superase la dimensi�n, esta idea sigue funcionando, 5 / 1000 resto -> 5
			} 
			else { // No hay m�s datos
				dimSalida = i;
				break;	
			}
		}

		//
		// Aqu� se procesar�an los datos que se encuentren en el array algo
		//

	}

	return 0;
}
