#include <stdio.h>
#include "affogato.h"

char address[64] = "http://localhost:3000";

routes ({
	get("/", {
		res(success, "", "/: done\n");
	});
	get("/oneplusone", {
		int sum;
		sum = 1 + 1;
		res(success, "", "/oneplusone: %i\n", sum);
	});
	post("/add", {
		char buffer[ctx->msg->body.len];
		char *c;

		memcpy(buffer, ctx->msg->body.ptr, ctx->msg->body.len);

		printf("%.*s\n", (int)ctx->msg->body.len, ctx->msg->body.ptr);
		res(success, "", "/add: done\n");
	});

	/* fallback if no routes can be resolved */
	res(notfound, "", "404 not found\n");
})

int main(int argc, char **argv) {
	affogato(address);
}
