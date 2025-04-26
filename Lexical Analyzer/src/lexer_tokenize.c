#include "../headers/lexer_tokenize.h"

size_t tokcnt(const char *const line) {
    size_t count = 0;
    size_t i = 0;
    
    while (line[i] != '\0') {
        if (isspace(line[i])) {
            i++;
            continue;
        }
        
        //counting stirng literals
        if (line[i] == '"') {
            i++;
            while (line[i] != '"' && line[i] != '\0') i++;
            if (line[i] == '"') i++;
            count++;
            continue;
        }
        
        //counting character literals
        if (line[i] == '\'') {
            i++;
            if (line[i] == '\\') i++; // skiping escape character (if any)
            i++; //skiping actual character
            if (line[i] == '\'') i++;
            count++;
            continue;
        }
        
        // Counting tokens for operators
        if(strchr("+-*/%=!<>&|^~", line[i]) != NULL) {
            while (strchr("+-*/%=!<>&|^~", line[i]) != NULL && line[i] != '\0') i++;
            
            count++;
            continue;
        }
        
        // Counting tokens for punctuations
        if (strchr("$./@`", line[i]) != NULL) {
            while(strchr("$./@`", line[i]) != NULL && line[i] != '\0') i++;

            count++;
            continue;
        }
        
        if(chk_sprtr(line[i])) {
            if(line[i] == '#') {
                while(!isspace(line[i]) && line[i] != '\n' && line[i] != '\0') i++;
                count++;
                continue;
            }
            
            i++;
            count++;
            continue;
        }

        // counting identifier, keyword, or literal
        if (isalnum(line[i]) || line[i] == '_') {
            while (isalnum(line[i]) || line[i] == '_' || line[i] == '.') i++;
            count++;
            continue;
        }

        i++;
    }
    
    return count;
}


void toknz_segtoset(tokset_t *const set,
                    const size_t token_index,
                    const char *const line,
                    const size_t start,
                    const size_t end,
                    const size_t line_no,
                    const tokcat_e category,
                    const size_t column) {
    size_t token_len = max(1, end - start);
    char *sub = malloc(sizeof(char) * (token_len + 1));
    if (!sub) {
        perror("Memory allocation failed");
        set->toks[token_index] = NULL;
        return;
    }
    
    strncpy(sub, line + start, token_len);
    sub[token_len] = '\0';
    
    tok_e token_type = get_toktyp(sub, category);
    
    set->toks[token_index] = tok_ptor(sub,
                                      token_type,
                                      line_no,
                                      column);
}


tokset_t *toknz(const char *const line) {
    size_t i = 0;
    
    size_t total_tokens = tokcnt(line);
    tokset_t *set = tokset_ptor(total_tokens);
    
    size_t token_start = 0;
    size_t token_count = 0;
    
    size_t line_no = 1;
    size_t column = 1;
    size_t inc = 0;
    
    while (line[i] != '\0') {
        if (i>=0 && line[i] == '\n') {
            while (line[i] == '\n') {
                i++; line_no++;
            }

            token_start = i;
            column = 1;
            continue;
        }
        
        if (isspace(line[i])) {
            i++;
            token_start = i;
            column++;
            continue;
        }
        
        // Creating tokens for string literals.
        if (line[i] == '"') {
            i++;
            column += inc;
            inc = 0;
            while (line[i] != '"' && line[i] != '\0') { i++; inc++; }
            if (line[i] == '"') { i++; inc++; }
            
            toknz_segtoset(set,
                          token_count++,
                          line,
                          token_start,
                          i,
                          line_no,
                          LITERAL,
                          column);
            // printf("Token string literal: %s\n", set->toks[token_count-1]->val);
            token_start = i;
            continue;
        }
        
        // Creating tokens for character literals
        if (line[i] == '\'') {
            i++;
            column += inc;
            inc = 0;
            if (line[i] == '\\') { i++; inc++; }
            i++; inc++;
            if (line[i] == '\'') { i++; inc++; }
            
            toknz_segtoset(set,
                          token_count++,
                          line,
                          token_start,
                          i,
                          line_no,
                          LITERAL,
                          column);
            // printf("Token character literal: %s\n", set->toks[token_count-1]->val);
            token_start = i;
            continue;
        }
        
        // Creating tokens for operators
        if(strchr("+-*/%=!<>&|^~", line[i]) != NULL) {
            column += inc;
            inc = 0;
            while (strchr("+-*/%=!<>&|^~", line[i]) != NULL && line[i] != '\0') {
                i++;
                inc++;
            }
            
            toknz_segtoset(set,
                          token_count++,
                          line,
                          token_start,
                          i,
                          line_no,
                          SYMBOLS,
                          column);
            // printf("Token op/punct: %s\n", set->toks[token_count-1]->val);
            
            token_start = i;
            continue;
        }
        
        // Creating tokens for punctuations
        if (strchr("$./@`", line[i]) != NULL) {
            column += inc;
            inc = 0;
            while(strchr("$./@\\`", line[i]) != NULL && line[i] != '\0') {
                i++;
                inc++;
            }

            toknz_segtoset(set,
                          token_count++,
                          line,
                          token_start,
                          i,
                          line_no,
                          SYMBOLS,
                          column);
            
            // printf("Token op/punct: %s\n", set->toks[token_count-1]->val);
            token_start = i;
            continue;
        }
        
        // Creating tokens for seperators & pre-processor directive operators
        if(chk_sprtr(line[i])) {
            column += inc;
            
            if (line[i] == '#') {
                inc = 0;
                while(!isspace(line[i]) && line[i] != '\n' && line[i] != '\0') {
                    i++;
                    inc++;
                }
                
                toknz_segtoset(set,
                              token_count++,
                              line,
                              token_start,
                              i,
                              line_no,
                              PRE_PROC,
                              column);
                
                token_start = i;
                continue;
            }
            
            char *buff = malloc(sizeof(char)*2);
            buff[0] = line[i];
            buff[1] = '\0';
            set->toks[token_count++] = tok_ptor(buff,
                                                PUNCTUATION,
                                                line_no,
                                                column);
                                                        
            i++;
            inc++;
            token_start = i;
            continue;
        }
        
        // Creating tokens for identifier, keyword, or literal
        if (isalnum(line[i]) || line[i] == '_' || line[i] == '.') {
            column += inc;
            inc = 0;
            while (isalnum(line[i]) || line[i] == '_' || line[i] == '.') { i++; inc++; }
            
            toknz_segtoset(set,
                          token_count++,
                          line,
                          token_start,
                          i,
                          line_no,
                          NFKI_LITERAL,
                          column);
            
            // printf("Token descriptor: %s\n", set->toks[token_count-1]->val);
            token_start = i;
            continue;
        }
    }
    
    if (token_count != total_tokens) {
        printf("Expected tokens: %zu\n", total_tokens);
        printf("Tokens processed: %zu\n", token_count);
        printf("Error tokenizing line.\n");
        tokset_dtor(set);
        return NULL;
    }
    
    return set;
}
