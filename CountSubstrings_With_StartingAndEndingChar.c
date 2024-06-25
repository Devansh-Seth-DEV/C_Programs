#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 50

char *ReadBuffer(const size_t);
size_t CountSubStart2End(const char *, const char, const char);

int main() {
	char *string;
	char strt, end;
	size_t count;
	
	printf("Enter a string: ");
	string = ReadBuffer(BUFFER_SIZE);
	printf("Enter starting character sequence of substring: ");
	scanf("%c%*c", &strt);
	printf("Enter ending character sequence of substring: ");
	scanf("%c%*c", &end);

	count = CountSubStart2End(string, strt, end);
	printf("Number of substrings starting with %c and ending with %c: %zu\n", strt, end, count);

	free(string);
	return 0;
}

char *ReadBuffer(const size_t buffsize) {
	char *buffer = (char *) calloc(buffsize, sizeof(char));

	fgets(buffer, buffsize, stdin);
	buffer[strlen(buffer)-1] = '\0';

	return buffer;	
}

size_t CountSubStart2End(const char *src, const char strt, const char end) {
	size_t count = 0, strtcnt = 0;

	for (size_t i=0; i<strlen(src); i++) {
		if (strt == src[i]) strtcnt++;
		else if(end == src[i]) count += strtcnt;
	}

	return count;
}
