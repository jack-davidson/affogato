#include "mongoose.h"

#define success 200
#define notfound 404

#define res(status, headers, ...)				\
	char ip_buffer[16];				 	\
	mg_ntoa(&ctx->conn->peer, ip_buffer, sizeof(ip_buffer));	\
	printf("%s %.*s %.*s (%s:%i)\n", ip_buffer,		\
		(int) ctx->msg->method.len,				\
		ctx->msg->method.ptr,				\
		(int) ctx->msg->uri.len,				\
		ctx->msg->uri.ptr,					\
		__func__, __LINE__);				\
	mg_http_reply(ctx->conn, status, headers, __VA_ARGS__);		\

#define afhttp_route(http_method, route, response)				\
	if (!(strncmp(ctx->msg->method.ptr, #http_method, ctx->msg->method.len))) {	\
		if (mg_http_match_uri(ctx->msg, route)) {				\
			response;						\
			return;							\
		};								\
	}

#define get(route, response)			\
	afhttp_route(GET, route, response)

#define post(route, response)			\
	afhttp_route(POST, route, response)

#define delete(route, response)			\
	afhttp_route(DELETE, route, response)

#define update(route, response)			\
	afhttp_route(UPDATE, route, response)

#define routes(body)		\
	static void		\
	_routes(afctx *ctx)	\
		body

#define affogato(address)	\
	afserver(_routes, address)

typedef struct {
	struct mg_connection *conn;
	struct mg_http_message *msg;
} afctx;

char *afhash(char *s);
void afhashfree(char *hexstring);
void afserver(void (*route)(afctx *ctx), char *address);
static void http_handler(struct mg_connection *c, int ev, void *ev_data,
	void (*fn_data));
