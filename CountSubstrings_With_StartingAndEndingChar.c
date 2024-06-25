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
	size_t count = 0;
	size_t SC = 0;
	ssize_t TOS = -1;
	size_t n = strlen(src);
	char STACK[n+1];

	memset(STACK, '\0', n);

	for (size_t i=0; i<n; i++) {
		if (strt == src[i]) { STACK[++TOS] = strt; ++SC; }
		else if (end == src[i] && -1 != TOS) STACK[++TOS] = end;
	}

	while (strt == STACK[TOS]) { TOS--; SC--; }
	
	while (0 != TOS) {
		if (end == STACK[TOS]) count += SC;
		else SC--;
		TOS--;
	}

	return count;
}
