#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mongoose.h"

#define HTTP_SUCCESS 200
#define HOME_MESSAGE "orkus\n"					  \
		     "https://github.com/jack-davidson/orkus\n\n" \
		     "compiled with " COMPILER "\n"		  \
		     "version " VERSION " commit " COMMIT "\n"

#define http_return(status, headers, ...)		 \
	char ip_buffer[16];				 \
	mg_ntoa(&c->peer, ip_buffer, sizeof(ip_buffer)); \
	printf("%s %.*s %.*s\n", ip_buffer,		 \
		(int) req->method.len,			 \
		req->method.ptr,			 \
		(int) req->uri.len,			 \
		req->uri.ptr);				 \
	mg_http_reply(c, status, headers, __VA_ARGS__)

char address[64] = "http://localhost:3000";

static void http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
int main(int argc, char **argv);

static void
http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	struct mg_http_message *req;

	req = (struct mg_http_message *) ev_data;
	if (ev == MG_EV_HTTP_MSG) {
		if (mg_http_match_uri(req, "/schedule_item")) {
			http_return(HTTP_SUCCESS, "", "/schedule_item: success\n");
		} else {
			http_return(HTTP_SUCCESS, "",  HOME_MESSAGE);
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

	printf("\nstarting orkus version " VERSION "\n");

	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, address, http_handler, &mgr);
	for (;;) mg_mgr_poll(&mgr, 1000);
	mg_mgr_free(&mgr);

	return 0;
}
