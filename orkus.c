#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mongoose.h"

#define HTTP_SUCCESS 200
#define PARAM_SIZE 1024

char address[64] = "http://localhost:3000";

static void http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
int main(int argc, char **argv);

static void http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	struct mg_http_message *hm;

	if (ev == MG_EV_HTTP_MSG) {
		hm = (struct mg_http_message *) ev_data;
		if (mg_http_match_uri(hm, "/*")) {
			char *param = malloc(PARAM_SIZE);
			strncpy(param, hm->uri.ptr + 1, PARAM_SIZE);
			param[hm->uri.len - 1] = '\0';
			mg_http_reply(c, 200, NULL, "Success\n", param);
			free(param);
		} else {
			mg_http_reply(c, 200, NULL, "");
		}
	}
}

int
main(int argc, char **argv)
{
	struct mg_mgr mgr;

	if (argc == 1) {
	} else if (argc == 2) {
		strcpy(address, argv[1]);
		printf("using address: %s\n", address);
	}

	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, address, http_handler, &mgr);
	while (1) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

	return 0;
}
