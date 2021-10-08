const bf_array_len = 30000
const readline = require('readline');

const getchar = () => {
    const rl = readline.createInterface({
        input: process.stdin,
        output: undefined,
    });

    return new Promise(resolve => rl.question("", ans => {
        rl.close();
        resolve(ans.charCodeAt(0));
    }))
}

const putchar = (char, iters = 1) => {
	process.stdout.write(String.fromCharCode(char).repeat(iters));
}


const main = async () => {
	let bf_ptr = bf_array_len;
	const bf_array = new Array(bf_array_len);
	while(--bf_ptr) bf_array[bf_ptr] = 0;
	bf_array[0] = 0;
	bf_ptr += 1;
	bf_array[bf_ptr] += 8;
	while(bf_array[bf_ptr]){
		bf_ptr -= 1;
		bf_array[bf_ptr] += 9;
		bf_ptr += 1;
		bf_array[bf_ptr] -= 1;
	}
	bf_ptr -= 1;
	putchar(bf_array[bf_ptr], 1);
	bf_ptr += 1;
	bf_array[bf_ptr] += 4;
	while(bf_array[bf_ptr]){
		bf_ptr -= 1;
		bf_array[bf_ptr] += 7;
		bf_ptr += 1;
		bf_array[bf_ptr] -= 1;
	}
	bf_ptr -= 1;
	bf_array[bf_ptr] += 1;
	putchar(bf_array[bf_ptr], 1);
	bf_array[bf_ptr] += 7;
	putchar(bf_array[bf_ptr], 2);
	bf_array[bf_ptr] += 3;
	putchar(bf_array[bf_ptr], 1);
	bf_ptr += 2;
	bf_array[bf_ptr] += 6;
	while(bf_array[bf_ptr]){
		bf_ptr -= 1;
		bf_array[bf_ptr] += 7;
		bf_ptr += 1;
		bf_array[bf_ptr] -= 1;
	}
	bf_ptr -= 1;
	bf_array[bf_ptr] += 2;
	putchar(bf_array[bf_ptr], 1);
	bf_array[bf_ptr] -= 12;
	putchar(bf_array[bf_ptr], 1);
	bf_ptr += 1;
	bf_array[bf_ptr] += 6;
	while(bf_array[bf_ptr]){
		bf_ptr -= 1;
		bf_array[bf_ptr] += 9;
		bf_ptr += 1;
		bf_array[bf_ptr] -= 1;
	}
	bf_ptr -= 1;
	bf_array[bf_ptr] += 1;
	putchar(bf_array[bf_ptr], 1);
	bf_ptr -= 1;
	putchar(bf_array[bf_ptr], 1);
	bf_array[bf_ptr] += 3;
	putchar(bf_array[bf_ptr], 1);
	bf_array[bf_ptr] -= 6;
	putchar(bf_array[bf_ptr], 1);
	bf_array[bf_ptr] -= 8;
	putchar(bf_array[bf_ptr], 1);
	bf_ptr += 3;
	bf_array[bf_ptr] += 4;
	while(bf_array[bf_ptr]){
		bf_ptr -= 1;
		bf_array[bf_ptr] += 8;
		bf_ptr += 1;
		bf_array[bf_ptr] -= 1;
	}
	bf_ptr -= 1;
	bf_array[bf_ptr] += 1;
	putchar(bf_array[bf_ptr], 1);

}

main();
