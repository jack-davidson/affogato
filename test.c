#include <openssl/sha.h>

#include "affogato.h"

char address[64] = "http://localhost:3000";

static void
route(struct mg_connection *c, struct mg_http_message *req)
{
	get("/", res(success, "",
		"orkus task/scheduling manager\n"
		"https://github.com/jack-davidson/orkus\n"
	));

	get("/ls", res(success, "", "ls\n"));
	get("/hello", res(success, "", "world\n"));
	get("/hi/*/hello", res(success, "", "hi\n"));

	res(notfound, "", "404 not found\n");
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
