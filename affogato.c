#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

#include "mongoose.h"
#include "affogato.h"

char **
pathparams(char *path, char *pattern)
{
	return (char **)NULL;
}

unsigned char
*hash(char *s)
{
	unsigned char *hash;

	hash = malloc(SHA512_DIGEST_LENGTH);

	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, s, strlen(s));
	SHA512_Final(hash, &ctx);

	return hash;
}

void
hashfree(unsigned char *h)
{
	free(h);
}

void
server(char *address)
{
	struct mg_mgr mgr;
	mg_log_set("1");
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, address, http_handler, &mgr);
	for (;;) mg_mgr_poll(&mgr, 500);
	mg_mgr_free(&mgr);
}

static void
echo(struct mg_connection *c, struct mg_http_message *req)
{
	res(success, "", "%.*s\n", req->body.len, req->body.ptr);
}

static void
route(struct mg_connection *c, struct mg_http_message *req)
{
	get("/", res(success, "",
		"orkus task/scheduling manager\n"
		"https://github.com/jack-davidson/orkus\n"
	));

	get("/ls", res(success, "", "ls\n"));
	post("/echo", echo(c, req));
	get("/hello", res(success, "", "world\n"));
	get("/hi/*/hello", {
		res(success, "", "hi\n");
	});

	res(notfound, "", "404 not found\n");
}

void
http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG) return route(c, ev_data);
	else return;
}
