#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

int main() {

	while (1) {
		if ( _kbhit() ) {
			char a = _getch();
			if (a == 's') {
				break;
			}
		}
	}
}