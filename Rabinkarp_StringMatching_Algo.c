#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RABIN_MOD 10
#define TEXT_MAXSIZE 200
#define PATTERN_MAXSIZE 50

int rk_hash(const char* str, const int strt, const int end);
unsigned int rabin_karp(const char* str, const char* key, const size_t N, const size_t M);

int main()
{
	char text[TEXT_MAXSIZE+1], key[PATTERN_MAXSIZE+1];
	memset(text, '\0', TEXT_MAXSIZE*sizeof(char));
	memset(key, '\0', PATTERN_MAXSIZE*sizeof(char));

	printf("Enter text: ");
	fgets(text, TEXT_MAXSIZE, stdin);

	printf("Enter pattern: ");
	fgets(key, PATTERN_MAXSIZE, stdin);

	const size_t N = strlen(text)-1;
	const size_t M = strlen(key)-1;

	text[N] = '\0';
	key[M] = '\0';

    printf("%s occurs: %u-times\n", key, rabin_karp(text, key, N, M));

    return 0;
}

int rk_hash(const char* str, const int strt, const int end) {
    int sum = 0;
    for(int i=strt; i<=end; i++) {
        sum = sum*10 + ((int) str[i])%RABIN_MOD;
    }
    
    return sum;
}

unsigned int rabin_karp(const char* str, const char* key, const size_t N, const size_t wndsize) {
    unsigned int count = 0;
    
    size_t wndstrt = 0;
    char wndstr[wndsize+1];
	wndstr[wndsize] = '\0';
    
    int keyHval = rk_hash(key, 0, wndsize-1);
	
	
    int wndHval = 0;
    
    for(size_t wndend = 0; wndend < N; wndend++) {
        if (wndend >= wndsize-1) {
            wndHval = (wndend == wndsize-1) ? 
                        rk_hash(str, wndstrt, wndend) :  wndHval*10 + ((int) str[wndend])%RABIN_MOD;
			
            
            if(wndHval == keyHval) {
                strncpy(wndstr, str+wndstrt, wndsize);
                if (!strcmp(key, wndstr)) {
                    ++count;
                }
            }
            
            ++wndstrt;
            wndHval = wndHval % (int) pow(10, wndsize-1);
        }
    }
    
    return count;
}
