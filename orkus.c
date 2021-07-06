#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mongoose.h"

#define HTTP_SUCCESS 200
#define http_send(status, headers, ...)		 \
	{ \
		char ip_buffer[16];				 \
		mg_ntoa(&c->peer, ip_buffer, sizeof(ip_buffer)); \
		printf("%s %.*s %.*s (%s:%i)\n", ip_buffer,		 \
			(int) req->method.len,			 \
			req->method.ptr,			 \
			(int) req->uri.len,			 \
			req->uri.ptr,				 \
			__func__, __LINE__);			 \
		mg_http_reply(c, status, headers, __VA_ARGS__);  \
	}

#define http_route(http_method, route, response) \
	if (!(strncmp(req->method.ptr, #http_method, req->method.len))) { \
		if (mg_http_match_uri(req, route)) {\
			response; \
		}; \
	}

#define get(route, response) \
	http_route(GET, route, response)

#define post(route, response) \
	http_route(POST, route, response)

static void http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
static void route(struct mg_connection *c, struct mg_http_message *req);
static void echo(struct mg_connection *c, struct mg_http_message *req);
int main(int argc, char **argv);

char address[64] = "http://localhost:3000";
struct mg_mgr mgr;

static void
echo(struct mg_connection *c, struct mg_http_message *req)
{
	http_send(HTTP_SUCCESS, "", "%.*s\n", req->body.len, req->body.ptr);
}

static void
route(struct mg_connection *c, struct mg_http_message *req)
{
	get("/", http_send(HTTP_SUCCESS, "",
		"orkus task/scheduling manager\n"
		"https://github.com/jack-davidson/orkus"
	));

	get("/ls", http_send(HTTP_SUCCESS, "", "ls\n"));
	post("/echo", echo(c, req));
	get("/hello", http_send(HTTP_SUCCESS, "", "world\n"));
}

static void
http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	switch (ev) {
	case MG_EV_HTTP_MSG:
		route(c, ev_data);
	default:
		return;
	}
}

int
main(int argc, char **argv)
{
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
