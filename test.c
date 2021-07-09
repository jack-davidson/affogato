#include <openssl/sha.h>

#include "affogato.h"

char address[64] = "http://localhost:3000";

static void
route(afctx ctx)
{
	afget("/", afres(success, "", "/: hello\n"));
	afget("/ls", afres(success, "", "ls\n"));
	afget("/hello", afres(success, "", "world\n"));
	afget("/hi/*/hello", afres(success, "", "hi\n"));

	afres(notfound, "", "404 not found\n");
}

int
main(int argc, char **argv)
{
	char *password = hash("hello");

	printf("%s\n", password);
	hashfree(password);

	server(route, address);
	return 0;
}
