#include "../headers/lexer_validation.h"

bool chk_keywd(const char *const value) {
    const char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while",
    };
    
    const size_t value_len = strlen(value);
    bool is_keyword = false;
    
    for(int i=0; i<31; i++) {
        if (value_len == strlen(keywords[i]) &&
            strcmp(keywords[i], value) == 0) {
                is_keyword = true;
                break;
            }
    }
    return is_keyword;
}

bool chk_oprtr(const char *const value) {
    const char *operators[] = {
      "+", "-", "*", "/", "%", "++", "--", //9
      "<", ">", "<=", ">=", "==", "!=", //6
      "&&", "||", "!", //3
      "&", "|", "^", "~", "<<", ">>", //6
      "=", "+=", "-=", "*=", "/=", "%=", "&=", "!=", "^=", ">>=", "<<=", //11
      "->", ".", //2
      "sizeof"
    };
    const size_t value_len = strlen(value);
    bool is_operator = false;
    
    for(int i=0; i<38; i++) {
        if (value_len == strlen(operators[i]) &&
            strcmp(operators[i], value) == 0) {
                is_operator = true;
                break;
            }
    }
    return is_operator;
}

bool chk_sprtr(const char value) {
    const char *sep = "()[]{}<>#$;,?:";
    if (strchr(sep, value) != NULL) return true;
    return false;
}

bool chk_punct(const char *const value) {
    const char *punctuations[] = {
      "\\", "#", "$", "'",
      "(", ")", "/", ":",
      ";", "?", "@", "[",
      "\"", "]", "_", "`",
      "{", "}"
    };
    const size_t value_len = strlen(value);
    bool is_punctuation = false;
    
    for(int i=0; i<18; i++) {
        if (value_len == strlen(punctuations[i]) &&
            strcmp(punctuations[i], value) == 0) {
                is_punctuation = true;
                break;
            }
    }
    return is_punctuation;
}

bool chk_numlt(const char *const value) {
    char *endptr;
    char *str = (char *) value;
    
    while (isspace(*str) || *str == '"') str++; // skip leading space
    if (*str == '\0') return false;
    
    strtoll(str, &endptr, 10);
    
    while (isspace(*endptr) || *endptr == '"') endptr++;  // skip trailing spaces
    
    return (*endptr == '\0') ? true : false;
}

bool chk_flolt(const char *const value) {
    char *endptr;
    char *str = (char *) value;
    
    while (isspace(*str) || *str == '"') str++; // skip leading spaces
    if (*str == '\0') return false;
    
    strtold(str, &endptr);
    
    while (isspace(*endptr) || *endptr == '"') endptr++;  // skip trailing spaces
    
    return (*endptr == '\0') ? true : false;
}

bool chk_chrlt(const char *const value) {
    const size_t value_len = strlen(value);
    bool is_character = false;
    if ((value_len == 3 && value[0] == '\'' &&
         value[value_len-1] == '\'') ||
        (value_len == 4 && value[0] == '\'' &&
         value[1] == '\\' && value[value_len-1] == '\'')) {
            is_character = true;
        }
        
    return is_character;
}

bool chk_strlt(const char *const value) {
    const size_t value_len = strlen(value);
    bool is_string = false;
    if (value_len >= 2 && value[0] == '"' &&
        value[value_len - 1] == '"') is_string = true;
    
    return is_string;
}

bool chk_idnfr(const char *const value) {
    const size_t value_len = strlen(value);
    
    if (value_len == 0 || (!isalpha((unsigned char)value[0]) && value[0] != '_')) {
        return false;
    }
    
    if (strchr(value, '.') != NULL) {
        for (size_t i = 1; i < value_len; i++) {
            char ch = value[i];
            if (ch == '.') {
                i++;
                if (!isalpha((unsigned char) value[i]) && value[i] != '_') return false;
                continue;
            }
            
            if (ch != '.' && !isalnum((unsigned char)ch) && ch != '_') {
                return false;
            }
        }
    }

    return true;
}

bool chk_prpop(const char *const value) {
    const char *preprocs[] = {
        "#include", "#define", "#undef", "#if",
        "#ifdef", "#ifndef", "#else", "#elif",
        "#endif", "#error", "#warning", "#line",
        "#pragma"
    };
    
    const size_t value_len = strlen(value);
    bool is_preproc = false;
    
    for(int i=0; i<13; i++) {
        if (value_len == strlen(preprocs[i]) &&
            strcmp(preprocs[i], value) == 0) {
                is_preproc = true;
                break;
            }
    }
    return is_preproc;
}
