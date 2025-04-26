#ifndef LEXER_TOKENIZE_H
#define LEXER_TOKENIZE_H

#include "lexer.h"

/**
 * @file lexer_tokenize.h
 *
 * @brief Tokenization core implementation
 * @ingroup Lexer
 *
 * @details Handles the conversion of source code into token streams:
 * - Counting tokens in source strings
 * - Segmenting code into lexical units
 * - Full tokenization pipeline
 *
 * @see lexer.h For token type definitions
 * @see TokenValidation For pattern checking rules
 */


/**
 * @defgroup Tokenization Tokenization
 * @ingroup Lexer
 * @brief Source code to token conversion
 * @{
 */


/**
 * @brief Counts the number of tokens in a given string (or file content).
 *
 * This function counts the number of tokens in a provided line of text (or a whole block of code,
 * if the entire file content is passed). It categorizes tokens into string literals, character literals,
 * operators, punctuations, and identifiers/keywords/literals. The function processes the string character by
 * character and considers escape sequences where applicable.
 *
 * @param line A constant pointer to a string containing the code to be analyzed. This string can represent
 *             a single line of code or the entire content of a file
 *
 * @return The total number of tokens found in the given string.
 *
 * @details Handles:
 * - String/character literals (with escape sequences)
 * - Operators and punctuation
 * - Identifiers/keywords
 * - Preprocessor directives
 *
 */
size_t tokcnt(const char *const line);


/**
 * @brief Tokenizes a segment of a line and stores the resulting token in the token set.
 *
 * This function extracts a substring from the given line, determines its token type based on the
 * specified token category, and stores the token in the provided token set at the specified index.
 * It handles memory allocation for the substring, creates a token using the @c tok_ptor constructor,
 * and stores the token in the @c set->toks array. If memory allocation fails, it sets the token to @c NULL.
 *
 * @param set         A pointer to the @c tokset_t object in which the token will be stored.
 * @param token_index The index in the token set where the token will be stored.
 * @param line        A constant pointer to the string containing the source line of code.
 * @param start       The starting index of the token within the line.
 * @param end         The ending index of the token within the line.
 * @param line_no     The line number where the token appears in the source code.
 * @param category    The category of the token, from @c tokcat_e (e.g., literals, symbols).
 * @param column      The starting column number of the token within the line.
 *
 * @note Allocates memory for token value
 * @warning Sets token to NULL on allocation failure
 */
void toknz_segtoset(tokset_t *const set,
                    const size_t token_index,
                    const char *const line,
                    const size_t start,
                    const size_t end,
                    const size_t line_no,
                    const tokcat_e category,
                    const size_t column);

/**
 * @brief Tokenizes a line (or multiple lines of code) into a set of tokens.
 *
 * This function processes the given string (or entire code block) and divides it into individual tokens.
 * It supports string literals, character literals, operators, punctuations, separators, preprocessor
 * directives, identifiers, and keywords. The tokens are stored in the provided token set.
 *
 * The function uses `toknz_segtoset` to tokenize individual segments of the line and store them in the
 * token set. It ensures that each token is associated with its type, position (line number, column),
 * and appropriate category (e.g., literal, operator, identifier).
 *
 * If the number of tokens in the resulting set does not match the expected token count, the function
 * reports an error and returns `NULL`. It processes both single lines and whole files, depending on
 * the input provided.
 *
 * @param line A constant pointer to the string (or code block) to be tokenized. This can represent a
 *             single line or the entire content of a file.
 *
 * @return A pointer to the token set (`tokset_t`) containing all identified tokens. Returns `NULL` if
 *         an error occurs during tokenization (e.g., mismatch in expected token count).
 * @retval NULL on tokenization error
 *
 * @details Processes:
 * - Multi-line inputs
 * - All token types
 * - Maintains position information
 */
tokset_t *toknz(const char *const line);


/** @}*/ // End of Tokenization group

#endif
