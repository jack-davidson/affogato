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

char **pathparams(char *path, char *pattern);
unsigned char *hash(char *s);
void hashfree(unsigned char *hash);
void http_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
void server(char *address);
static void route(struct mg_connection *c, struct mg_http_message *req);
static void echo(struct mg_connection *c, struct mg_http_message *req);

