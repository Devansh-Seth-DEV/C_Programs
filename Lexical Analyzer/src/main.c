#include "../headers/lexer_tokenize.h"
#include <libgen.h>

void rmext(char *fname) {
    char *dot = strrchr(fname, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>RETURN", argv[0]);
        return 1;
    }
    
    fchnk_t *content = get_fchnk(argv[1]);
    if (!content) {
        perror("Failed to allocate the file content.");
    }
    
    char *code = content->buff;
//    char *str = "-3.15";
    
    tokset_t *set = toknz(code);
    if (!set) {
        printf("Unable to create token set\n");
        return 1;
    }
    
    char *base = basename(argv[1]);
    rmext(base);
    char *logfile;
    
    size_t written = asprintf(&logfile, "log/%s.log", base);
    if (written < 0) {
        printf("Error creating file path");
        tokset_dtor(set);
        
        free(content->buff);
        code = NULL;
        free(content);
        return 1;
    }
    
    FILE *fp = fopen(logfile, "w");
    if (!fp) {
        perror("Error while opening the file.\n");
    }
    
    if (!fwrite_tokset(fp, set)) {
        perror("Error while writing the token set into file.\n");
    } else {
        printf("Sucessfully created %s\n", logfile);
    }
    
    fclose(fp);
    
    tokset_dtor(set);
    
    free(content->buff);
    code = NULL;
    free(content);
    
    return 0;
}
