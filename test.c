#include <openssl/sha.h>

#include "affogato.h"

char address[64] = "http://localhost:3000";

static void
route(conn c, msg req)
{
	get(req, "/", res(c, req, success, "", "/: hello\n"));
	get(req, "/ls", res(c, req, success, "", "ls\n"));
	get(req, "/hello", res(c, req, success, "", "world\n"));
	get(req, "/hi/*/hello", res(c, req, success, "", "hi\n"));

	res(c, req, notfound, "", "404 not found\n");
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
