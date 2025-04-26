#include "../headers/lexer_io.h"

fchnk_t *fchnk_ctor() {
    return malloc(sizeof(fchnk_t));
}

fchnk_t *fchnk_ptor(char *const buff, const size_t chksz) {
    fchnk_t *chnk = fchnk_ctor();
    chnk->buff = buff;
    chnk->chksz = chksz;
    
    return chnk;
}

bool fwrite_fchnk(const char *fname, const fchnk_t *chnk) {
    FILE *fp = fopen(fname, "w");
    if (!fp) {
        perror("Error opening file");
        return false;
    }
    
    size_t written = fwrite(chnk->buff, sizeof(char), chnk->chksz, fp);
    if (written < chnk->chksz) {
        perror("Error writing file");
        fclose(fp);
        return false;
    }
    
    fclose(fp);
    
    return true;
}

fchnk_t *get_fchnk(const char *fname) {
    FILE *fp = fopen(fname, "rb");
    if (!fp) {
        perror("Error opening file");
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    long long file_size = ftell(fp);
    rewind(fp);
    
    char *buff = (char *) malloc((file_size+1) * sizeof(char));
    if (!buff) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }
    
    size_t read_size = fread(buff, 1, file_size, fp);
    buff[read_size] = '\0';
    
    fclose(fp);
    
    fchnk_t *chnk = fchnk_ctor();
    if (!chnk) {
        perror("Memory allocation failed");
        free(buff);
        return NULL;
    }
    
    chnk->buff = buff;
    chnk->chksz = read_size;
    
    return chnk;
}
