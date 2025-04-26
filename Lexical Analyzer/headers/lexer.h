#ifndef LEXER_H
#define LEXER_H


/**
 * @file lexer.h
 *
 * @brief Lexical analyzer components for token processing
 * @defgroup Lexer Lexical Analyzer
 * @brief Complete token handling system for source code analysis
 *
 * @details This header provides the complete infrastructure for:
 * - Token type classification (#TokenClassification)
 * - Individual token representation (#TokenStructure)
 * - Token collection management (#TokenSet)
 *
 * @note The groups form a processing pipeline:
 * 2. Classification categorizes lexemes
 * 3. Structures store the results
 * 4. Collections manage token sequences
 */

#include "lexer_validation.h"

/**
 * @defgroup TokenClassification Token Classification
 * @brief Token type definitions and classification utilities
 *
 * This group contains all enums and functions related to:
 * - Token categories (preprocessor, symbols, literals)
 * - Specific token types (keywords, operators, identifiers)
 * - Token type conversion and identification
 *
 * @{
 */

/**
 * @enum tokcat_e
 *
 * @brief Token category enumeration for categorizing different token types in the pre-processing phase.
 *
 * This enumeration represents the different categories of tokens that are encountered during the
 * lexical analysis or tokenization process. The token categories include preprocessor directives,
 * symbols, literals, and numerical literals (including floating-point numbers, keywords, and identifiers).
 *
 * @note The NFKI_LITERAL category includes tokens that represent numerical literals, floating-point literals, keywords, or identifiers.
 */
typedef enum tokcat_e{
    PRE_PROC,   ///< Preprocessor directives (`#define`, `#include`).
    SYMBOLS,    ///< Operators/punctuation (`+`, `;`).
    LITERAL,    ///< String/character literals.
    
    /**
     * @brief Numerical literals, keywords, or identifiers.
     * @note Includes integers, floats, and reserved words.
     */
    NFKI_LITERAL
} tokcat_e;



/**
 * @enum tok_e
 *
 * @brief Token type enumeration for categorizing various types of tokens during lexical analysis.
 *
 * This enumeration defines the different types of tokens that can be encountered during the
 * tokenization phase of source code parsing. Each token type corresponds to a specific
 * element or construct in the source code, such as keywords, operators, punctuation, literals,
 * and identifiers.
 */
typedef enum tok_e{
    /**
     * @brief Keyword token type.
     *
     * This type represents keywords in the source code (e.g., `int`, `if`, `while`, `return`).
     */
    KEYWORD,
    
    /**
     * @brief Operator token type.
     *
     * This type represents operators such as arithmetic (`+`, `-`, `*`, `/`), logical (`&&`, `||`),
     * relational (`<`, `>`, `==`), and others.
     */
    OPERATOR,
    
    /**
     * @brief Punctuation token type.
     *
     * This type represents punctuation characters such as semicolons (`;`), commas (`,`), and
     * parentheses (`(`, `)`, `{`, `}`).
     */
    PUNCTUATION,
    
    /**
     * @brief Numeric literal token type.
     *
     * This type represents integer numeric literals (e.g., `123`, `456`).
     */
    NUMERIC_LITERAL,
    
    /**
     * @brief Floating point literal token type.
     *
     * This type represents floating-point numeric literals (e.g., `3.14`, `1.618`).
     */
    FLOATING_POINT_LITERAL,
    
    /**
     * @brief Character literal token type.
     *
     * This type represents character literals (e.g., `'a'`, `'b'`, `'1'`).
     */
    CHARACTER_LITERAL,
    
    /**
     * @brief String literal token type.
     *
     * This type represents string literals (e.g., `"Hello, world!"`, `"1234"`).
     */
    STRING_LITERAL,
    
    /**
     * @brief Invalid identifier token type.
     *
     * This type represents tokens that are invalid identifiers (e.g., identifiers starting with
     * a number or containing illegal characters).
     */
    INVALID_IDENTIFIER,
    
    /**
     * @brief Identifier token type.
     *
     * This type represents valid identifiers (e.g., variable names, function names, etc.).
     */
    IDENTIFIER,
    
    /**
     * @brief Preprocessor operator token type.
     *
     * This type represents operators related to preprocessor directives (e.g., `#define`, `#include`).
     */
    PRE_PROCESSOR_OPERATOR
} tok_e;


/**
 * @brief Returns a string representation of a token type.
 *
 * This function takes a token type from the @c tok_e enumeration and returns its corresponding
 * human-readable string name. It's primarily used for debugging, logging, or displaying token
 * types in a user-friendly format.
 *
 * @param type The token type (of enum @c tok_e) whose string representation is to be retrieved.
 *
 * @return A constant string representing the name of the token type.
 *         If the token type is unrecognized, returns "Invalid Identifier".
 */
const char *toktyp_rval(const tok_e type);



/**
 * @brief Determines the specific token type from a given token string and its category.
 *
 * This function maps a token string and its broader token category (from @c tokcat_e)
 * to a specific token type (from @c tok_e). It internally uses helper functions to check
 * the nature of the token based on its string value.
 *
 * Depending on the token category, it checks the token string against known operators,
 * punctuation, literals, keywords, and identifiers.
 *
 * @param value The token string to be classified.
 * @param type  The general category of the token, given as a @c tokcat_e value.
 *
 * @return The specific token type as a value from @c tok_e.
 *         Returns @c INVALID_IDENTIFIER if no match is found.
 */
tok_e get_toktyp(const char *const value, const tokcat_e type);

/** @} */ // End of TokenClassification group





/**
 * @defgroup TokenStructure Token Structure
 * @brief Token instance representation and operations
 *
 * This group contains the token container structure (@c tok_t) and all functions
 * for creating, managing, and destroying individual tokens.
 *
 * @{
 */

/**
 * @struct tok_t
 *
 * @brief Container for token data and metadata
 *
 * Stores the complete information about a single token including:
 * - The lexeme value
 * - Its classified type
 * - Source code location
 */
typedef struct tok_t{
    char *val;  /**< Token string value (dynamically allocated) */
    tok_e typ;  /**< Token type from @ref tok_e */
    size_t ln;  /**< Source line number (1-indexed) */
    size_t col; /**< Starting column position (1-indexed) */
} tok_t;


/**
 * @brief Allocates and returns a new token object.
 *
 * This is the default constructor for a @c tok_t token. It dynamically allocates memory
 * for a new token instance and returns a pointer to it.
 *
 * @return A pointer to a newly allocated @c tok_t object.
 *         The memory should be freed by the caller when no longer needed.
 */
tok_t *tok_ctor();


/**
 * @brief Allocates and returns an array of token objects.
 *
 * This function acts as a constructor for multiple @c tok_t tokens. It allocates memory
 * for an array of @c n tokens and returns a pointer to the array.
 *
 * @param n The number of tokens to allocate.
 *
 * @return A pointer to array. of @c tok_t tokens.
 * @note The memory should be freed by the caller when no longer needed.
 */
tok_t **tok_nctor(const size_t n);


/**
 * @brief Creates and initializes a token with given values.
 *
 * This parameterized constructor allocates a new @c tok_t object and initializes it
 * with the provided token value, type, line number, and column number.
 *
 * @param value The string value of the token (lexeme).
 * @param type  The specific token type from @c tok_e.
 * @param line  The line number where the token appears in the source code.
 * @param col   The column number where the token starts in the line.
 *
 * @return A pointer to the newly created and initialized @c tok_t object.
 *         The caller is responsible for freeing the memory.
 */
tok_t *tok_ptor(char *value,
                 const tok_e type,
                 const size_t line,
                 const size_t col);


/**
 * @brief Frees the memory associated with a token.
 *
 * This destructor deallocates the memory used by a @c tok_t object, including
 * its dynamically allocated value string. After calling this function, the token
 * pointer should no longer be used.
 *
 * @param tok A pointer to the @c tok_t object to be destroyed.
 *            If @c tok or @c tok->val is @c NULL, the function does nothing.
 */
void tok_dtor(tok_t *tok);


/**
 * @brief Prints the contents of a token to the standard output.
 *
 * This utility function displays the token's value, type (as a string), line number,
 * and column number in a human-readable format using @c printf.
 *
 * @param tok A constant pointer to the @c tok_t object to be printed.
 */
void printf_tok(const tok_t *const tok);


/**
 * @brief Writes the contents of a token to a file.
 *
 * This function prints the token's value, type, line number, and column number to the specified file.
 * It performs error checking on each write operation and reports issues using @c perror.
 *
 * @param fp  A valid file pointer to which the token will be written. Must not be @c NULL.
 * @param tok A constant pointer to the @c tok_t object to be written.
 *
 * @return @c true if all write operations succeed, @c false otherwise.
 */
bool fwrite_tok(FILE *fp, const tok_t *const tok);

/** @}*/ // End of TokenStructure group




/**
 * @defgroup TokenSet Token Set
 * @brief Collection of tokens and operations
 *
 * This group contains the token set structure (tokset_t) and all functions
 * for managing collections of tokens, including:
 * - Set creation and destruction
 * - Token counting and analysis
 * - Set visualization and I/O
 *
 * @{
 */

/**
 * @struct tokset_t
 *
 * @brief Container for multiple tokens
 *
 * Stores an array of tokens along with metadata about the collection.
 * Used for holding all tokens from a file or code block.
 */
typedef struct tokset_t {
    tok_t **toks; /**< Dynamic array of token pointers */
    size_t toksz; /**< Count of tokens in the array */
} tokset_t;


/**
 * @brief Allocates and returns a new token set object.
 *
 * This constructor allocates memory for a new @c tokset_t object, which will hold a collection
 * of tokens. However, the token array itself (`toks`) is not yet initialized and will need
 * further allocation.
 *
 * @return A pointer to the newly allocated @c tokset_t object.
 *         The caller is responsible for freeing the memory when done.
 */
tokset_t *tokset_ctor();


/**
 * @brief Creates and initializes a token set with a specified number of tokens.
 *
 * This parameterized constructor allocates memory for a @c tokset_t object and initializes
 * the @c toks array to hold a specified number of tokens. It also sets the size of the token
 * set (`toksz`). If memory allocation fails at any point, it reports the error and returns @c NULL.
 *
 * @param toksz The number of tokens that the token set should be able to hold.
 *
 * @return A pointer to the newly created and initialized @c tokset_t object.
 *         Returns @c NULL if memory allocation fails.
 */
tokset_t *tokset_ptor(const size_t toksz);


/**
 * @brief Frees the memory associated with a token set.
 *
 * This destructor deallocates the memory used by a @c tokset_t object, including
 * freeing each individual token in the token set (using @c tok_dtor) and the token array itself.
 * After calling this function, the token set pointer should no longer be used.
 *
 * @param set A pointer to the @c tokset_t object to be destroyed.
 *            If @c set is @c NULL, the function does nothing.
 */
void tokset_dtor(tokset_t *set);


/**
 * @brief Counts the number of tokens of a specific type in a token set.
 *
 * This function iterates through the token set and counts how many tokens match the
 * specified type. It compares each token's type with the provided `type` argument.
 *
 * @param set  A constant pointer to the @c tokset_t object containing the tokens to be checked.
 * @param type The specific token type (from @c tok_e) to count in the token set.
 *
 * @return The number of tokens of the specified type found in the token set.
 */
size_t cnt_toktyp(const tokset_t *const set, const tok_e type);


/**
 * @brief Prints the contents of a token set to the standard output.
 *
 * This function iterates through the token set and prints each token's value, type, line,
 * and column information using the @c printf_tok function. It formats the output such that
 * each token is printed on a new line, except for the last token.
 *
 * @param set A constant pointer to the @c tokset_t object containing the tokens to be printed.
 */
void printf_tokset(const tokset_t *const set);


/**
 * @brief Writes the contents of a token set to a file.
 *
 * This function writes a summary of the token set to the specified file, including the
 * total number of tokens and a breakdown of the counts for each token type. It then writes
 * each individual token’s details to the file. If any write operation fails, it reports the
 * error and returns @c false.
 *
 * @param fp  A valid file pointer where the token set will be written. Must not be @c NULL.
 * @param set A constant pointer to the @c tokset_t object containing the tokens to be written.
 *
 * @return @c true if all write operations succeed, @c false otherwise.
 */
bool fwrite_tokset(FILE *fp, const tokset_t *const set);

/** @}*/ // End of TokenSet group


/** @}*/ // End of Lexer group

#endif
