int json_parse(
	char *json,
	unsigned int size,
	int (*callback)(char *key, int keysize, char *value, int valuesize, char *element, int elementsize, void *userdata),
	void *userdata) {
	
	char *key, *value, *element, inside_string;
	unsigned int i;
	int ret, keysize, valuesize, elementsize, acc;
	
	for (i=0; i<size; i++) {
next_pair:

		/* find key */
		if (json[i] == '\"') {
			key = json + i + 1;
			i++;
			
			for (i=i; i<size; i++) {
				if (json[i] == '\"' && json[i-1] != '\\') break;
			}
			if (json[i] == 0) return -1;
			
			keysize = json + i - key;
			i++;
			
			/* find value */
			for (i=i; i<size; i++) {
				
				/* object values */
				if (json[i] == '{') {
					acc = 0;
					value = json + i;
					
					for (i=i; i<size; i++) {
						if (json[i] == '{') acc++;
						else if (json[i] == '}') acc--;
						if (acc == 0) break;
					}
					if (json[i] == 0) return -1;
					
					valuesize = json + i - value + 1;
					
					if ((ret = callback(key, keysize, value, valuesize, 0, 0, userdata))) return ret;
					
					if ((ret = json_parse(value, valuesize, callback, userdata))) return ret;
					
					goto next_pair;
				}
				
				/* array values */
				if (json[i] == '[') {
					inside_string = 0;
					acc = 0;
					element = json + i + 1;
					
					for (i=i; i<size; i++) {
						if (json[i] == '\"' && json[i-1] != '\\') inside_string = !inside_string;
						if (inside_string) continue;
						
						if (json[i] == '[') acc++;
						else if (json[i] == '{') acc++;
						
						if ((json[i] == ',' || json[i] == ']') && acc == 1) {
							elementsize = json + i - element;
							
							if ((ret = callback(key, keysize, 0, 0, element, elementsize, userdata))) return ret;
							
							/* try parse in case of object element */
							json_parse(element, elementsize, callback, userdata);
							
							element = json + i + 1;
						}
						
						if (json[i] == ']') acc--;
						else if (json[i] == '}') acc--;
						if (acc == 0) break;
					}
					if (json[i] == 0) return -1;
					
					goto next_pair;
				}
				
				/* string values */
				if (json[i] == '\"') {
					value = json + i + 1;
					i++;
					
					for (i=i; i<size; i++) {
						if (json[i] == '\"' && json[i-1] != '\\') break;
					}
					if (json[i] == 0) return -1;
					
					valuesize = json + i - value;
					i++;
					
					if ((ret = callback(key, keysize, value, valuesize, 0, 0, userdata))) return ret;
					
					goto next_pair;
				}
				
				/* other values */
				if (json[i] != ':' && json[i] != ' ' && json[i] != '\n' && json[i] != '\r' && json[i] != '\t') {
					value = json + i;
					i++;
					
					for (i=i; i<size; i++) {
						if (json[i] == ',') break;
						if (json[i] == '}') break;
					}
					if (json[i] == 0) return -1;
					
					valuesize = json + i - value;
					i++;
					
					if ((ret = callback(key, keysize, value, valuesize, 0, 0, userdata))) return ret;
					
					goto next_pair;
				}
			}
		}
		
		/* parse recursive arrays */
		if (json[i] == '[') {
			inside_string = 0;
			acc = 0;
			element = json + i + 1;
			
			for (i=i; i<size; i++) {
				if (json[i] == '\"' && json[i-1] != '\\') inside_string = !inside_string;
				if (inside_string) continue;
				
				if (json[i] == '[') acc++;
				else if (json[i] == '{') acc++;
				
				if ((json[i] == ',' || json[i] == ']') && acc == 1) {
					elementsize = json + i - element;
					
					if ((ret = callback(0, 0, 0, 0, element, elementsize, userdata))) return ret;
					
					/* try parse in case of object element */
					json_parse(element, elementsize, callback, userdata);
					
					element = json + i + 1;
				}
				
				if (json[i] == ']') acc--;
				else if (json[i] == '}') acc--;
				if (acc == 0) break;
			}
			if (json[i] == 0) return -1;
			
			return 0;
		}
	}
	return 0;
}

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