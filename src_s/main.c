#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void _xor(char *str, size_t const size, char *key);

void encryptDecrypt(char *str, size_t const size, char *key)
{
	for (size_t index = 0; index < size; index++)
		str[index] = str[index] ^ key[index % 16];
}

int main(void)
{
	char *str = strdup("qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm");
	char *key = "1234567898765432";

	puts(str);
	encryptDecrypt(str, strlen(str), key);
	puts(str);
	encryptDecrypt(str, strlen(str), key);
	puts(str);

	puts("");

	puts(str);
	_xor(str, strlen(str), key);
	puts(str);
	_xor(str, strlen(str), key);
	puts(str);
}
