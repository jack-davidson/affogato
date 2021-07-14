#include <stdio.h>
#include <err.h>
#include <syslog.h>
#include "affogato.h"

#define LOGLEVEL 1

char address[64] = "http://localhost:3000";

routes ({
	get("/", {
		res(success, "", "/: done\n");
	});
	get("/admin", {
		res(success, "", "/admin: done\n");
	});
	res(notfound, "", "404 not found\n");
})

static void
createpassword(void)
{
	char *password, *hash, prompt[64];
	FILE *fd;

	snprintf(prompt, sizeof(prompt), "password for %s: ", getenv("USER"));
	password = getpass(prompt);

	hash = afhash(password);
	fd = fopen("password", "w+");
	fprintf(fd, "%s", hash);
	fclose(fd);
	afhashfree(hash);
}

static void
authenticate(void)
{
	char *password_input, *hashed_input, password_hash[64], prompt[64];
	FILE *fd;

	snprintf(prompt, sizeof(prompt), "password for %s: ", getenv("USER"));
	password_input = getpass(prompt);
	hashed_input = afhash(password_input);

	if((fd = fopen("password", "r")) == NULL) {
		puts("failed to open file 'password'\n");
	}

	fread(password_hash, 1, 64, fd);
	if (!strncmp(password_hash, hashed_input, 64))
		printf("hey");
	fclose(fd);
	afhashfree(hashed_input);
}

int main(int argc, char **argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "createpassword")) {
			createpassword();
			exit(0);
		}
		if (!strcmp(argv[1], "auth")) {
			authenticate();
			exit(0);
		}
	}
	affogato(address);
}
