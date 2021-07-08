#include <openssl/sha.h>

#include "affogato.h"

char address[64] = "http://localhost:3000";

int
main(int argc, char **argv)
{
	if (argc == 1) {
	} else if (argc == 2) {
		strcpy(address, argv[1]);
		printf("using address: %s\n", address);
	}

	unsigned char *password = hash("hello");

	char hexstring[1024] = {0};
	for(int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
		sprintf(hexstring, "%s%02x", hexstring, password[i]);
	}
	printf("%s\n", hexstring);

	hashfree(password);

	server(address);
	return 0;
}
