#ifndef LEXER_VALIDATION_H
#define LEXER_VALIDATION_H

/**
 * @file lexer_validation.h
 *
 * @defgroup TokenValidation Token Validation
 * @brief Token validation functions for lexical analysis
 * @ingroup Lexer
 *
 * @details Provides pattern-matching functions that identify:
 * - Language keywords (e.g., `if`, `while`)
 * - Operators and punctuation (e.g., `+`, `;`)
 * - Literals (numeric, string, character)
 * - Identifiers and preprocessor directives
 *
 * Used during tokenization to classify raw lexemes into specific token types.
 * All functions are case-sensitive and follow standard C syntax rules.
 *
 * @note These are pure validation functions - they don't modify input or
 *       handle memory allocation.
 * @{
 */

#include "lexer_io.h"

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))


/**
 * @brief Checks if the given value is a C programming language keyword.
 *
 * This function checks if the provided string matches any of the 31 reserved keywords in the C language.
 * It compares the input string with a predefined list of C keywords and returns a boolean indicating
 * whether the value is a valid keyword or not.
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value matches a C keyword, `false` otherwise.
 *
 * @note The function performs a case-sensitive comparison with the list of C keywords.
 */
bool chk_keywd(const char *const value);


/**
 * @brief Checks if the given value is a valid C operator.
 *
 * This function checks if the provided string matches any of the common operators in C. It compares
 * the input string with a predefined list of C operators (including arithmetic, logical, comparison,
 * and assignment operators) and returns a boolean indicating whether the value is a valid operator or not.
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value matches a C operator, `false` otherwise.
 *
 * @note The function performs a case-sensitive comparison with the list of C operators.
 */
bool chk_oprtr(const char *const value);


/**
 * @brief Checks if the given character is a valid separator.
 *
 * This function checks if the provided character is one of the common separators in C syntax, such as
 * parentheses, brackets, braces, or punctuation characters. It is used by the parser to identify
 * and separate tokens based on these characters, which indicate boundaries between elements like
 * function parameters, array elements, or preprocessor directives.
 *
 * @param value A constant character to be checked.
 *
 * @return `true` if the value is a separator (one of `()[]{}<>#$;,?:`), `false` otherwise.
 *
 * @note The function checks for a single character and performs a direct comparison with a set of predefined separators.
 */
bool chk_sprtr(const char value);


/**
 * @brief Checks if the given value is a punctuation character.
 *
 * This function checks if the provided string matches any of the predefined punctuation characters
 * in C syntax. Punctuation characters include symbols like backslash, hash, dollar sign, parentheses,
 * semicolon, etc., which serve various roles such as operators, delimiters, and special characters
 * in the C language.
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value is one of the predefined punctuation characters, `false` otherwise.
 *
 * @note The function compares the given string against a set of predefined punctuation characters.
 */
bool chk_punct(const char *const value);

/**
 * @brief Checks if the given value is a valid numerical literal.
 *
 * This function verifies if the provided string represents a valid numerical literal. A valid
 * numerical literal consists of an optional sign followed by a sequence of digits. Leading and
 * trailing spaces are ignored during the check. The function will return true if the string
 * represents a valid number, and false otherwise.
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value is a valid numerical literal, `false` otherwise.
 *
 * @note The function uses `strtoll` to attempt conversion of the string to a long long integer.
 *       It considers spaces and ignores quotes (if any).
 */
bool chk_numlt(const char *const value);


/**
 * @brief Checks if the given value is a valid floating-point literal.
 *
 * This function verifies if the provided string represents a valid floating-point literal. A valid
 * floating-point literal can have a decimal point and may include an optional exponent part (e.g., `3.14`, `1e10`, `2.5e-3`).
 * Leading and trailing spaces are ignored during the check. The function will return true if the string
 * represents a valid floating-point number, and false otherwise.
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value is a valid floating-point literal, `false` otherwise.
 *
 * @note The function uses `strtold` to attempt conversion of the string to a `long double` value.
 *       It considers spaces and ignores quotes (if any).
 */
bool chk_flolt(const char *const value);


/**
 * @brief Checks if the given value is a valid character literal.
 *
 * This function checks whether the provided string represents a valid character literal in C/C++.
 * A valid character literal consists of exactly one character enclosed in single quotes (e.g., `'a'`, `'%'`),
 * or it may be a special character (e.g., `'\n'`, `'\\'`) represented with an escape sequence enclosed in single quotes.
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value is a valid character literal, `false` otherwise.
 *
 * @note A valid character literal must meet one of the following conditions:
 *       - It is exactly 3 characters long and starts and ends with a single quote (e.g., `'a'`).
 *       - It is exactly 4 characters long, starts with a single quote, has an escape sequence in the middle (e.g., `'\\'`),
 *         and ends with a single quote (e.g., `'\n'` or `'\\'`).
 */
bool chk_chrlt(const char *const value);


/**
 * @brief Checks if the given value is a valid string literal.
 *
 * This function checks whether the provided string represents a valid string literal in C/C++.
 * A valid string literal is a sequence of characters enclosed in double quotes (`"`), including any escape sequences (e.g., `\"`, `\\`).
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value is a valid string literal, `false` otherwise.
 *
 * @note A valid string literal must meet the following condition:
 *       - It starts and ends with double quotes (e.g., `"hello"` or `"line\n"`), and it may contain escape sequences.
 */
bool chk_strlt(const char *const value);


/**
 * @brief Checks if the given value is a valid identifier.
 *
 * This function checks whether the provided string is a valid identifier in C/C++.
 * A valid identifier must:
 * - Begin with a letter (a-z, A-Z) or an underscore (`_`).
 * - Subsequent characters can be letters, digits (0-9), underscores (`_`), or periods (`.`).
 * - If periods are present, they must separate valid identifiers (i.e., they should not appear consecutively or at the start/end of the string).
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value is a valid identifier, `false` otherwise.
 *
 * @note A valid identifier is typically used for variable names, function names, or other symbols in C/C++ programming.
 *       This function allows for periods (`.`) in the identifier but enforces the rule that periods must separate valid identifier segments.
 */
bool chk_idnfr(const char *const value);


/**
 * @brief Checks if the given value is a preprocessor directive.
 *
 * This function checks whether the provided string is a valid preprocessor directive
 * in C/C++ code. Preprocessor directives are typically used to control the compilation
 * process and are denoted by a `#` symbol followed by a keyword.
 *
 * @param value A constant pointer to the string to be checked.
 *
 * @return `true` if the value is a valid preprocessor directive, `false` otherwise.
 *
 * @note This function specifically checks for the presence of a `#` followed by a known
 *       preprocessor keyword. It does not verify the validity of the entire directive
 *       (such as checking if the syntax or arguments are correct).
 */
bool chk_prpop(const char *const value);

/** @}*/ // End of TokenValidation group

#endif
