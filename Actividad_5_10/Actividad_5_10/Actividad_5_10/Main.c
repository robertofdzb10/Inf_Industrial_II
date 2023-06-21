#include <windows.h>
#include <stdio.h>
#include <conio.h>

double str_to_double(const char *str) {
	// Varaibles para el estado
	enum State {Inicio = 0, Integer = 1, Decimal = 2, Final = 3};
	enum State currentState = Inicio;

	// Variables para el resultado
	int signo = 1;
	double parte_entera = 0;
	double parte_decimal = 0;
	double divisior = 0.1;
	double resultado = 0;

	while (*str) { // Mientras no recorramos la cadena *str entera sigue, al final de toda cadena hay un /0, por lo que al leer eso el while, acaba
		
		switch (currentState) {

			case Inicio:
				if (*str == ' ') {
					currentState = Final;
				}
				else if (*str == '-') {
					signo = -1;
					currentState = Integer;
				}
				else if (*str == '+') {
					signo = 1;
					currentState = Integer;
				}
				else if (*str >= '0' && *str <= '9') {
					parte_entera = (*str - '0'); // Al restar el 0 str al número correspondiente str obtenemos el código ASCII del número en fomra de num
					currentState = Integer;
				}
				else {
					currentState = Final; // SI introducimos cualquier otra cosa, salimos
				}
				break;

			case Integer:
				if (*str >= '0' && *str <= '9') {
					parte_entera = parte_entera *10 + (*str - '0');
				}
				else if (*str == '.') {
					currentState = Decimal;
				}
				else {
					currentState = Final; // Si introducimos cualquier otra cosa, salimos
				}
				break;

			case Decimal:
				if (*str >= '0' && *str <= '9') {
					parte_decimal = parte_decimal + (*str - '0') * divisior;
					divisior = divisior * 0.1;
				}
				else {
					currentState = Final; // Si introducimos cualquier otra cosa, salimos
				}
				break;

			case Final:
				break;

			default:
				printf("Estado imposible (no debería suceder nunca): volvemos al inicio\n");
				currentState = Inicio;
				break;
			}
		str++; // Aumentamos el valor de la cadena str
	}
	if (currentState != Final) {
		resultado = signo * (parte_entera + parte_decimal);
		return resultado;
	}
	else {
		resultado = 0.0;
		return resultado;
	}
}

int main() {
	const char str[] = "3.14"; // const indica que el valor de la variable no se puede cambiar. Además, al no darle un tamaño predefinido a la cadena, el compilador le asignará la dimensión en función de su tamaño
	double num = str_to_double(str);
	printf("El numero introducido es: %lf\n", num);
	return 0;
}