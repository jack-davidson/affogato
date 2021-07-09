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

	hexstring = calloc(SHA512_DIGEST_LENGTH * 2, 1);

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
server(void (*router)(afctx ctx),
	char *address)
{
	struct mg_mgr mgr;
	mg_log_set("1");
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, address, http_handler, router);
	for (;;) mg_mgr_poll(&mgr, 500);
	mg_mgr_free(&mgr);
}

void
http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	afctx ctx = {
		.conn = c,
		.msg = ev_data
	};

	if (ev == MG_EV_HTTP_MSG)
		return ((void (*)(afctx ctx))fn_data)(ctx);
	else return;
}
