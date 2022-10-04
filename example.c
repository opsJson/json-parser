#include "json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int callback(char *key, int keysize, char *value, int valuesize, char *element, int elementsize, void *userdata) {
	(void)userdata;
	if (element) {
		if (key) printf("[%.*s] => [%.*s]\n", keysize, key, elementsize, element);
		else printf("[%.*s]\n", elementsize, element);
	}
	else printf("[%.*s] => [%.*s]\n", keysize, key, valuesize, value);
	return 0;
}

int main(int argc, char **argv) {
	FILE *fp;
	char *buffer;
	long int size;
	
	if (argc != 2) {
		fprintf(stderr, "JSON PARSER IN PURE C\n");
		fprintf(stderr, "USAGE: %s [FILE.json]\n", argv[0]);
		return 1;
	}
	
	if ((fp = fopen(argv[1], "r")) == NULL) return 1;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	buffer = malloc(size + 1);
	fread(buffer, size, 1, fp);
	fclose(fp);
	
	json_parse(buffer, size, callback, NULL);
	
	free(buffer);
	return 0;
}