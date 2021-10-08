#include <stdio.h>

int main(int argc, char* argv[]){
	int bf_array[30000] = {0};
	int* bf_ptr = bf_array;
		bf_ptr += 1;
	*bf_ptr += 8;
	while(*bf_ptr){
		bf_ptr -= 1;
		*bf_ptr += 9;
		bf_ptr += 1;
		*bf_ptr-= 1;
	}
	bf_ptr -= 1;
	putchar(*bf_ptr);
	bf_ptr += 1;
	*bf_ptr += 4;
	while(*bf_ptr){
		bf_ptr -= 1;
		*bf_ptr += 7;
		bf_ptr += 1;
		*bf_ptr-= 1;
	}
	bf_ptr -= 1;
	*bf_ptr += 1;
	putchar(*bf_ptr);
	*bf_ptr += 7;
	putchar(*bf_ptr);
	putchar(*bf_ptr);
	*bf_ptr += 3;
	putchar(*bf_ptr);
	bf_ptr += 2;
	*bf_ptr += 6;
	while(*bf_ptr){
		bf_ptr -= 1;
		*bf_ptr += 7;
		bf_ptr += 1;
		*bf_ptr-= 1;
	}
	bf_ptr -= 1;
	*bf_ptr += 2;
	putchar(*bf_ptr);
	*bf_ptr-= 12;
	putchar(*bf_ptr);
	bf_ptr += 1;
	*bf_ptr += 6;
	while(*bf_ptr){
		bf_ptr -= 1;
		*bf_ptr += 9;
		bf_ptr += 1;
		*bf_ptr-= 1;
	}
	bf_ptr -= 1;
	*bf_ptr += 1;
	putchar(*bf_ptr);
	bf_ptr -= 1;
	putchar(*bf_ptr);
	*bf_ptr += 3;
	putchar(*bf_ptr);
	*bf_ptr-= 6;
	putchar(*bf_ptr);
	*bf_ptr-= 8;
	putchar(*bf_ptr);
	bf_ptr += 3;
	*bf_ptr += 4;
	while(*bf_ptr){
		bf_ptr -= 1;
		*bf_ptr += 8;
		bf_ptr += 1;
		*bf_ptr-= 1;
	}
	bf_ptr -= 1;
	*bf_ptr += 1;
	putchar(*bf_ptr);

	return 0;
}
    