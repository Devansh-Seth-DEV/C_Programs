#include "../headers/lexer.h"

const char *toktyp_rval(const tok_e type) {
    switch (type) {
        case KEYWORD: return "Keyword"; break;
        case OPERATOR: return "Operator"; break;
        case PUNCTUATION: return "Punctuator"; break;
        case NUMERIC_LITERAL: return "Numeric Literal"; break;
        case FLOATING_POINT_LITERAL: return "Floating-Point Literal"; break;
        case CHARACTER_LITERAL: return "Character Literal"; break;
        case STRING_LITERAL: return "String Literal"; break;
        case IDENTIFIER: return "Identifier"; break;
        case PRE_PROCESSOR_OPERATOR: return "Preprocessor Operator"; break;
        
        default: return "Invalid Identifier";
    }
}

tok_e get_toktyp(const char *const value, const tokcat_e type) {
    switch (type) {
        case SYMBOLS:
            if (chk_oprtr(value)) return OPERATOR;
            if (chk_punct(value)) return PUNCTUATION;
            break;
            
        case LITERAL:
            if (chk_strlt(value)) return STRING_LITERAL;
            if (chk_chrlt(value)) return CHARACTER_LITERAL;
            break;
            
        case NFKI_LITERAL:
            if (chk_numlt(value)) return NUMERIC_LITERAL;
            if (chk_flolt(value)) return FLOATING_POINT_LITERAL;
            if (chk_keywd(value)) return KEYWORD;
            if (chk_idnfr(value)) return IDENTIFIER;
            break;
            
        case PRE_PROC:
            if (chk_prpop(value)) return PRE_PROCESSOR_OPERATOR;
    }
    
    return INVALID_IDENTIFIER;
}

tok_t *tok_ctor() {
    return (tok_t *) malloc(sizeof(tok_t));
}

tok_t **tok_nctor(const size_t n) {
    return (tok_t **) malloc(sizeof(tok_t *) * n);
}

tok_t *tok_ptor(char *value,
                const tok_e type,
                const size_t line,
                const size_t col) {
    tok_t *tok = tok_ctor();
    tok->val = value;
    tok->typ = type;
    tok->ln = line;
    tok->col = col;
    
    return tok;
}

void tok_dtor(tok_t *tok) {
    free(tok->val);
    tok->val = NULL;
    free(tok);
    tok = NULL;
}

void printf_tok(const tok_t *const tok) {
    printf("Token: %s\n", tok->val);
    printf("Type: %s\n", toktyp_rval(tok->typ));
    printf("Line: %zu\n", tok->ln);
    printf("Column: %zu\n", tok->col);
}

bool fwrite_tok(FILE *fp, const tok_t *const tok) {
    if (!fp) {
        perror("Error opening null file");
        return false;
    }
    
    size_t written = fprintf(fp, "Token: %s\n", tok->val);
    bool is_written = true;
    if (written < 0) {
        perror("Error writing token value");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Type: %s\n", toktyp_rval(tok->typ));
    if (written < 0) {
        perror("Error writing token type");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Line: %zu\n", tok->ln);
    if (written < 0) {
        perror("Error writing token line number");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Column: %zu\n", tok->col);
    if (written < 0) {
        perror("Error writing token column number");
        if ((int) is_written) is_written = false;
    }
    
    return is_written;
}


tokset_t *tokset_ctor() {
    return (tokset_t *) malloc(sizeof(tokset_t));
}

tokset_t *tokset_ptor(const size_t toksz) {
    tokset_t *set = tokset_ctor();
    if (!set) {
        perror("Memory allocation failed");
        return NULL;
    }
    
    set->toks = tok_nctor(toksz);
    if (!set->toks) {
        perror("Memory allocation failed");
        free(set);
        return NULL;
    }
    
    set->toksz = toksz;
    return set;
}

void tokset_dtor(tokset_t *set) {
    for(size_t i=0; i<set->toksz; i++) {
        tok_dtor(set->toks[i]);
    }
    
    free(set->toks);
    set->toks = NULL;
    free(set);
    set = NULL;
}

size_t cnt_toktyp(const tokset_t *const set, const tok_e type) {
    size_t cnt = 0;
    for(size_t i=0; i<set->toksz; i++) {
        if (set->toks[i]->typ == type) cnt++;
    }
    
    return cnt;
}

void printf_tokset(const tokset_t *const set) {
    for(size_t i=0; i<set->toksz; i++) {
        printf_tok(set->toks[i]);
        if (i<set->toksz-1) printf("\n");
    }
}

bool fwrite_tokset(FILE *fp, const tokset_t *const set) {
    if (!fp) {
        perror("Error opening null file");
        return false;
    }
    
    bool is_written = true;
    
    size_t total_identifiers = cnt_toktyp(set, IDENTIFIER);
    size_t total_invalid_identifiers = cnt_toktyp(set, INVALID_IDENTIFIER);
    size_t total_keywords = cnt_toktyp(set, KEYWORD);
    size_t total_operators = cnt_toktyp(set, OPERATOR);
    size_t total_preprocessor_op = cnt_toktyp(set, PRE_PROCESSOR_OPERATOR);
    size_t total_punctuations = cnt_toktyp(set, PUNCTUATION);
    size_t total_numeric_literals = cnt_toktyp(set, NUMERIC_LITERAL);
    size_t total_floating_literals = cnt_toktyp(set, FLOATING_POINT_LITERAL);
    size_t total_char_literals = cnt_toktyp(set, CHARACTER_LITERAL);
    size_t total_string_literals = cnt_toktyp(set, STRING_LITERAL);
    
    size_t written = fprintf(fp, "Total tokens: %zu\n", set->toksz);
    if (written < 0) {
        perror("Error writing total tokens");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total preprocessor operators: %zu\n", total_preprocessor_op);
    if (written < 0) {
        perror("Error writing total preprocessor operators");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total identifiers: %zu\n", total_identifiers);
    if (written < 0) {
        perror("Error writing total identifiers");
        if ((int) is_written) is_written = false;
    }
    
    
    written = fprintf(fp, "Total invalid-identifiers: %zu\n", total_invalid_identifiers);
    if (written < 0) {
        perror("Error writing total invalid-identifiers");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total keywords: %zu\n", total_keywords);
    if (written < 0) {
        perror("Error writing total keywords");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total operators: %zu\n", total_operators);
    if (written < 0) {
        perror("Error writing total operators");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total punctuations: %zu\n", total_punctuations);
    if (written < 0) {
        perror("Error writing total punctuations");
        if ((int) is_written) is_written = false;
    }

    written = fprintf(fp, "Total numeric literals: %zu\n", total_numeric_literals);
    if (written < 0) {
        perror("Error writing total numeric literals");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total floating-point literals: %zu\n", total_floating_literals);
    if (written < 0) {
        perror("Error writing total floating-point literals");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total character literals: %zu\n", total_char_literals);
    if (written < 0) {
        perror("Error writing total character literals");
        if ((int) is_written) is_written = false;
    }
    
    written = fprintf(fp, "Total string literals: %zu\n\n", total_string_literals);
    if (written < 0) {
        perror("Error writing total string literals");
        if ((int) is_written) is_written = false;
    }
    
    for(size_t i=0; i<set->toksz; i++) {
        if (!fwrite_tok(fp, set->toks[i]) && is_written) is_written = false;
        fprintf(fp, "\n");
    }
    
    return is_written;
}



