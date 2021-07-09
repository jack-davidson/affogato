#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

#include "mongoose.h"
#include "affogato.h"

char
*hash(char *s)
{
	int i;
	char *hexstring;
	unsigned char hash[SHA512_DIGEST_LENGTH];

	hexstring = malloc(SHA512_DIGEST_LENGTH * 2);
	memset(hexstring, 0, SHA512_DIGEST_LENGTH * 2);

	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, s, strlen(s));
	SHA512_Final(hash, &ctx);

	for (i = 0; i < SHA512_DIGEST_LENGTH; i++) {
		sprintf(hexstring, "%s%02x", hexstring, hash[i]);
	}

	return hexstring;
}

void
hashfree(char *hexstring)
{
	free(hexstring);
}

void
server(void (*router)(struct mg_connection *c,
	struct mg_http_message *req), char *address)
{
	struct mg_mgr mgr;
	mg_log_set("1");
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, address, http_handler, router);
	for (;;) mg_mgr_poll(&mgr, 500);
	mg_mgr_free(&mgr);
}

void
http_handler(struct mg_connection *c, int ev, void *ev_data,
	void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG)
		return ((void (*)(struct mg_connection *c,
			struct mg_http_message *req))fn_data)(c, ev_data);
	else return;
}
