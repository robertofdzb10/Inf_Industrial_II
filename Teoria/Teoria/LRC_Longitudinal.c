#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

int main() {

	long bit0_t[7] = { 1,0,0,0,0,1,0 };
	long bit1_t[7] = { 1,0,0,0,0,1,0 };	
	long bit2_t[7] = { 1,0,0,0,0,1,0 };
	long bit3_t[7] = { 1,0,0,0,0,1,1 };
	long bit4_t[7] = { 1,0,0,0,0,1,0 };
	long bit5_t[7] = { 1,0,0,0,0,1,1 };
	long bit6_t[7] = { 1,0,0,0,0,1,0 };
	long bit7_t[7] = { 1,0,0,0,0,1,1 };

	long* bit0 = NULL, *bit1 = NULL, *bit2 = NULL, *bit3 = NULL, *bit4 = NULL, *bit5 = NULL, *bit6 = NULL,* bit7 = NULL;
	int ceros0 = 0, ceros1 = 0, ceros2 = 0, ceros3 = 0, ceros4 = 0, ceros5 = 0, ceros6 = 0, ceros7 = 0;
	int unos0 = 0, unos1 = 0, unos2 = 0, unos3 = 0, unos4 = 0, unos5 = 0, unos6 = 0, unos7 = 0;

	for (int i = 0; i < 7; i++) {
		if (bit0_t[i] == 0) {
			ceros0++;
		}
		else {
			unos0++;
		}

		if (bit1_t[i] == 0) {
			ceros1++;
		}
		else {
			unos1++;
		}

		if (bit2_t[i] == 0) {
			ceros2++;
		}
		else {
			unos2++;
		}

		if (bit3_t[i] == 0) {
			ceros3++;
		}
		else {
			unos3++;
		}

		if (bit4_t[i] == 0) {
			ceros4++;
		}
		else {
			unos4++;
		}

		if (bit5_t[i] == 0) {
			ceros5++;
		}
		else {
			unos5++;
		}

		if (bit6_t[i] == 0) {
			ceros6++;
		}
		else {
			unos6++;
		}

		if (bit7_t[i] == 0) {
			ceros7++;
		}
		else {
			unos7++;
		}
	}

	bit0 = (long*)malloc(8 * sizeof(long));
	bit1 = (long*)malloc(8 * sizeof(long));
	bit2 = (long*)malloc(8 * sizeof(long));
	bit3 = (long*)malloc(8 * sizeof(long));
	bit4 = (long*)malloc(8 * sizeof(long));
	bit5 = (long*)malloc(8 * sizeof(long));
	bit6 = (long*)malloc(8 * sizeof(long));
	bit7 = (long*)malloc(8 * sizeof(long));
	if (bit0 == NULL || bit1 == NULL || bit2 == NULL || bit3 == NULL || bit4 == NULL || bit5 == NULL || bit6 == NULL || bit7 == NULL ) {
		printf("Error en la gestión de la memoría dinámica.");
	}

	memcpy(bit0, bit0_t, 7 * sizeof(long));
	memcpy(bit1, bit1_t, 7 * sizeof(long)); 
	memcpy(bit2, bit2_t, 7 * sizeof(long));
	memcpy(bit3, bit3_t, 7 * sizeof(long));
	memcpy(bit4, bit4_t, 7 * sizeof(long));
	memcpy(bit5, bit5_t, 7 * sizeof(long));
	memcpy(bit6, bit6_t, 7 * sizeof(long));
	memcpy(bit7, bit7_t, 7 * sizeof(long));

	if (((unos0 + 1) % 2) != 0) {
		bit0[7] = 1;
	}
	else {
		bit0[7] = 0;
	}

	if (((unos1 + 1) % 2) != 0) {
		bit1[7] = 1;
	}
	else {
		bit1[7] = 0;
	}

	if (((unos2 + 1) % 2) != 0) {
		bit2[7] = 1;
	}
	else {
		bit2[7] = 0;
	}

	if (((unos3 + 1) % 2) != 0) {
		bit3[7] = 1;
	}
	else {
		bit3[7] = 0;
	}

	if (((unos4 + 1) % 2) != 0) {
		bit4[7] = 1;
	}
	else {
		bit4[7] = 0;
	}

	if (((unos5 + 1) % 2) != 0) {
		bit5[7] = 1;
	}
	else {
		bit5[7] = 0;
	}

	if (((unos6 + 1) % 2) != 0) {
		bit6[7] = 1;
	}
	else {
		bit6[7] = 0;
	}

	if (((unos7 + 1) % 2) != 0) {
		bit7[7] = 1;
	}
	else {
		bit7[7] = 0;
	}

	for (int i = 0; i < 8; i++) {
		printf("%d", bit0[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", bit1[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", bit2[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", bit3[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", bit4[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", bit5[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", bit6[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", bit7[i]);
	}
	printf("\n");
}