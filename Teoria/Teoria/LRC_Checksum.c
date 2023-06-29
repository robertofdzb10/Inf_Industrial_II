#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

int main() {

	long bit0_suma = 89;
	long bit1_suma = 58;
	long bit2_suma = 235;
	long bit3_suma = 87;
	long bit4_suma = 17;
	long bit5_suma = 168;

	int suma_total = bit0_suma + bit1_suma + bit2_suma + bit3_suma + bit4_suma + bit5_suma;
	int bit8_suma = suma_total % 256;

	printf("Resultado: %d", bit8_suma);
}