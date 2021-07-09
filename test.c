#include "affogato.h"

char address[64] = "http://localhost:3000";

routes ({
	afget("/", {
		afres(success, "", "/: hello\n")
	});
	afget("/ls", {
		afres(success, "", "ls\n")
	});
	afget("/hello", {
		afres(success, "", "world\n")
	});
	afget("/hi/*/hello", {
		afres(success, "", "hi\n")
	});

	/* fallback if no routes can be resolved */
	afres(notfound, "", "404 not found\n");
})

int
main(int argc, char **argv)
{
	aflisten(address);
	return 0;
}
