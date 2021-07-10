#include <stdio.h>
#include "affogato.h"

char address[64] = "http://localhost:3000";

routes ({
	get("/", {
		for (int i = 0; i < 100; i++) {
			if (!(i % 5)) 
				fputs("Fizz", stdout);
			if (!(i % 7)) 
				fputs("Buzz", stdout);
			else
				printf("%i", i);
			putc('\n', stdout);
			
		}
		res(success, "", "/: done\n");
	});
	get("/ls", {
		res(success, "", "ls\n");
	});
	get("/hello", {
		res(success, "", "world\n");
	});
	get("/hi/*/hello", {
		res(success, "", "hi\n");
	});

	/* fallback if no routes can be resolved */
	res(notfound, "", "404 not found\n");
})

int main(int argc, char **argv) {
	affogato(address);
}
