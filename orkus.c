#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mongoose.h"

#define success 200
#define notfound 404

#define res(status, headers, ...)				\
	char ip_buffer[16];				 	\
	mg_ntoa(&c->peer, ip_buffer, sizeof(ip_buffer));	\
	printf("%s %.*s %.*s (%s:%i)\n", ip_buffer,		\
		(int) req->method.len,				\
		req->method.ptr,				\
		(int) req->uri.len,				\
		req->uri.ptr,					\
		__func__, __LINE__);				\
	mg_http_reply(c, status, headers, __VA_ARGS__);		\

#define http_route(http_method, route, response)				\
	if (!(strncmp(req->method.ptr, #http_method, req->method.len))) {	\
		if (mg_http_match_uri(req, route)) {				\
			response;						\
			return;							\
		};								\
	}

#define get(route, response)			\
	http_route(GET, route, response)

#define post(route, response)			\
	http_route(POST, route, response)

#define delete(route, response)			\
	http_route(DELETE, route, response)

#define update(route, response)			\
	http_route(UPDATE, route, response)

static void http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
static void route(struct mg_connection *c, struct mg_http_message *req);
static void echo(struct mg_connection *c, struct mg_http_message *req);
int main(int argc, char **argv);

char address[64] = "http://localhost:3000";
struct mg_mgr mgr;

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
	get("/hi*", {
		printf("hi\n");
		if (3 == 3) {
			printf("3 == 3\n");
		}
		if (PATH_MAX == 4096) {
			printf("hello\n");
		}
		char *x;
		x = malloc(1024);
		strcpy(x, "The quick brown fox jumped over the lazy dog.");
		free(x);
		res(success, "", "hi\n");
	});

	res(notfound, "", "404 not found\n");
}

static void
http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG) return route(c, ev_data);
	else return;
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

	mg_log_set("1");
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, address, http_handler, &mgr);
	for (;;) mg_mgr_poll(&mgr, 500);
	mg_mgr_free(&mgr);

	return 0;
}
