#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_BUFFER_SIZE 20
#define TEXT_BUFFER_SIZE 200


unsigned char *STDIN_RDText(const size_t);
unsigned short int *ComputeLPS(const unsigned char*, const unsigned short int);
size_t KMPMatchString(const unsigned char*, const unsigned char*, const size_t const*, const unsigned short int const*);

int main() {
	unsigned char *textPtr;	
	unsigned char *patternPtr;
	unsigned short int patlen;
	size_t textlen, patcount;

	printf("Enter text:\n");
	textPtr = STDIN_RDText(TEXT_BUFFER_SIZE);
	textlen = strlen(textPtr);

	printf("Enter pattern: ");
	patternPtr  = STDIN_RDText(PATTERN_BUFFER_SIZE);
	patlen 		= strlen(patternPtr); 

	patcount = KMPMatchString(textPtr, patternPtr, &textlen, &patlen);	
	printf("Occurance: %zu\n", patcount);

	free(patternPtr);
	free(textPtr);
	patternPtr = NULL;
	textPtr = NULL;

	return 0;
}

unsigned char* STDIN_RDText(const size_t BUFFER_SIZE) {
	unsigned char *textPtr = (unsigned char *) calloc(BUFFER_SIZE, sizeof(unsigned char));
	fgets(textPtr, BUFFER_SIZE, stdin);
	textPtr[strlen(textPtr)-1] = '\0';

	return textPtr;
}

unsigned short int *ComputeLPS(const unsigned char *patternPtr, const unsigned short int patlen) {
	unsigned short int *lpsPtr = (unsigned short int *) calloc(patlen, sizeof(unsigned short int));

	for (unsigned short int i=0; i<patlen; i++) {
		if (0 == i) lpsPtr[0] = 0;
		else if (1 == i) lpsPtr[1] = (patternPtr[0] == patternPtr[1]);
		else {
			unsigned short int lpslen = 0;
			unsigned short int k = 0;

			for (unsigned short int j=0; j<i; j++) {
				k = 0;
				while (k<j+1) {
					if (patternPtr[k] != patternPtr[i-j+k]) break;
					++k;
				}

				if (k == j+1) lpslen = k;
			}

			lpsPtr[i] = lpslen;
		}
	}

	return lpsPtr;
}

size_t KMPMatchString(const unsigned char *textPtr, const unsigned char* patternPtr, const size_t const *textlen, const unsigned short int const *patlen) {
	unsigned short int *lpsPtr = ComputeLPS(patternPtr, *patlen);
	size_t i = 0, j = 0;
	size_t count = 0;

	while (i < (*textlen)) {
		if (0 == j && (textPtr[i] != patternPtr[j])) i++;
		else if (textPtr[i] != patternPtr[j]) { j = lpsPtr[j-1]; }
		else { ++i; ++j; }

		if (j>=*patlen) {
			count++;
			j = 0;
		}
	}
	
	free(lpsPtr);
	lpsPtr = NULL;

	return count;
}
