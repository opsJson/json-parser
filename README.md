# json-parser
Simple and easy Json Parser in ANSI C

Just copy and paste one function into your project.

# Quick Start
```console
gcc example.c -o example
./example [file.json]
```

# Example

## Input
```json
{
    "glossary": {
        "title": "example glossary",
		"GlossDiv": {
            "title": "S",
			"GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
					"SortAs": "SGML",
					"GlossTerm": "Standard Generalized Markup Language",
					"Acronym": "SGML",
					"Abbrev": "ISO 8879:1986",
					"GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
						"GlossSeeAlso": ["GML", "XML"]
                    },
					"GlossSee": "markup"
                }
            }
        }
    }
}	
```
## Callback function
```c
int callback(char *key, int keysize, char *value, int valuesize, char *element, int elementsize, void *userdata) {
	(void)userdata;
	if (element) {
		if (key) printf("[%.*s] => [%.*s]\n", keysize, key, elementsize, element);
		else printf("[%.*s]\n", elementsize, element);
	}
	else printf("[%.*s] => [%.*s]\n", keysize, key, valuesize, value);
	return 0;
}
```

## Output
```console
[glossary] => [{
        "title": "example glossary",
                "GlossDiv": {
            "title": "S",
                        "GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
                                        "SortAs": "SGML",
                                        "GlossTerm": "Standard Generalized Markup Language",
                                        "Acronym": "SGML",
                                        "Abbrev": "ISO 8879:1986",
                                        "GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                                                "GlossSeeAlso": ["GML", "XML"]
                    },
                                        "GlossSee": "markup"
                }
            }
        }
    }]
[title] => [example glossary]
[GlossDiv] => [{
            "title": "S",
                        "GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
                                        "SortAs": "SGML",
                                        "GlossTerm": "Standard Generalized Markup Language",
                                        "Acronym": "SGML",
                                        "Abbrev": "ISO 8879:1986",
                                        "GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                                                "GlossSeeAlso": ["GML", "XML"]
                    },
                                        "GlossSee": "markup"
                }
            }
        }]
[title] => [S]
[GlossList] => [{
                "GlossEntry": {
                    "ID": "SGML",
                                        "SortAs": "SGML",
                                        "GlossTerm": "Standard Generalized Markup Language",
                                        "Acronym": "SGML",
                                        "Abbrev": "ISO 8879:1986",
                                        "GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                                                "GlossSeeAlso": ["GML", "XML"]
                    },
                                        "GlossSee": "markup"
                }
            }]
[GlossEntry] => [{
                    "ID": "SGML",
                                        "SortAs": "SGML",
                                        "GlossTerm": "Standard Generalized Markup Language",
                                        "Acronym": "SGML",
                                        "Abbrev": "ISO 8879:1986",
                                        "GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                                                "GlossSeeAlso": ["GML", "XML"]
                    },
                                        "GlossSee": "markup"
                }]
[ID] => [SGML]
[SortAs] => [SGML]
[GlossTerm] => [Standard Generalized Markup Language]
[Acronym] => [SGML]
[Abbrev] => [ISO 8879:1986]
[GlossDef] => [{
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                                                "GlossSeeAlso": ["GML", "XML"]
                    }]
[para] => [A meta-markup language, used to create markup languages such as DocBook.]
[GlossSeeAlso] => ["GML"]
[GlossSeeAlso] => [ "XML"]
[GlossSee] => [markup]
```

# Features
- ANSI C (C89).
- Just one easy function.
- Simple, small and fast.
- Don't need libc.
- 0 allocations.
- 0 dependencies.
- 0 obfuscated custom types.

# Documentation

## Json Parse function:
```c
int json_parse(
	char *json,
	int size,
	int (*callback)(char *key, int keysize, char *value, int valuesize, char *element, int elementsize, void *userdata),
	void *userdata);
```

### 1° parameter:
  - string source.
### 2° parameter:
  - length of string source. (no need to count null terminator)
### 3° parameter:
  - pointer to callback function, that will recieve parsed json.
### 4° parameter:
  - pointer of any custom data you want to pass to your callback.
### return:
  - 0 if success.
  - -1 if fail while parsing.
  - callback return value if it's different than zero.

## Callback function:
```c
int callback(char *key, int keysize, char *value, int valuesize, char *element, int elementsize, void *userdata);
```
### 1° parameter:
  - pointer to the begin of key string.
### 2° parameter:
  - length of key string.
### 3° parameter:
  - pointer to begin of value string.
### 4° parameter:
  - length of value string.
### 5° parameter:
  - pointer to begin of element string. (while parsing an array inside the json)
### 6° parameter:
  - length of element string.
### 7° parameter:
  - pointer of any custom data passed over json_parse() function.
### return:
  - if nonzero the parsing is terminated early.
