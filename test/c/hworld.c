#include <stdio.h>

int main(int argc, char* argv[]){
	int bf_array[30000] = {0};
	int* bf_ptr = bf_array;
		*bf_ptr += 10;
	while(*bf_ptr){
		bf_ptr += 1;
		*bf_ptr += 7;
		bf_ptr += 1;
		*bf_ptr += 10;
		bf_ptr += 1;
		*bf_ptr += 3;
		bf_ptr += 1;
		*bf_ptr += 1;
		bf_ptr -= 4;
		*bf_ptr-= 1;
	}
	bf_ptr += 1;
	*bf_ptr += 2;
	putchar(*bf_ptr);
	*bf_ptr = getchar();
	*bf_ptr += 2;
	putchar(*bf_ptr);

	return 0;
}
    