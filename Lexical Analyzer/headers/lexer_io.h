#ifndef LEXER_IO_H
#define LEXER_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * @file lexer_io.h
 *
 * @defgroup LexerIO File I/O
 * @brief File chunk handling utilities for lexical analysis
 * @ingroup Lexer
 *
 * @details Provides structures and functions for:
 * - Reading source files into manageable chunks
 * - Writing processed tokens back to files
 * - Memory-managed file operations
 *
 * @note All functions maintain binary-safe operations
 * @see Tokenization For token processing pipeline
 * @{
 */


/**
 * @struct fchnk_t
 * 
 * @brief A structure that represents a chunk of data from a file.
 *
 * This structure is used for managing chunks of data read from a file. It contains
 * a buffer to store the data and a size field to store the size of the data in the buffer.
 */
typedef struct {
    /**
     * @brief A pointer to a buffer that holds the content of the file chunk.
     *
     * This buffer stores the actual data read from the file. The size of this
     * buffer is specified by the `chksz` field. The buffer must be allocated
     * dynamically (e.g., using `malloc`), and should be freed when no longer needed.
     */
    char *buff;
    
    /**
     * @brief The size of the data stored in the `buff` buffer.
     *
     * This field stores the length of the data currently held in the `buff` array. It
     * represents the number of characters or bytes stored in the buffer. This field is useful
     * for knowing how much data has been read into the buffer from a file or how much data
     * remains to be processed.
     */
    size_t chksz;
} fchnk_t;

/**
 * @brief Constructs a new file chunk object.
 *
 * This function dynamically allocates memory for a new `fchnk_t` structure, initializing it
 * to hold a chunk of data from a file. The `buff` pointer in the newly allocated `fchnk_t`
 * structure is not initialized (it may be `NULL` initially). The caller should ensure to allocate
 * memory for `buff` and set `chksz` appropriately after the constructor is called.
 *
 * @return A pointer to a newly allocated `fchnk_t` structure.
 *         If the memory allocation fails, it will return `NULL`.
 */
fchnk_t *fchnk_ctor();


/**
 * @brief Initializes and returns a file chunk object with provided buffer and size.
 *
 * This function allocates memory for a new `fchnk_t` structure and assigns the provided
 * character buffer and its corresponding size to the structure.
 *
 * @param buff Pointer to a character buffer containing the file chunk data.
 * @param chksz The size of the buffer, representing the number of characters stored in it.
 *
 * @return A pointer to the initialized `fchnk_t` structure, or NULL if allocation fails.
 *
 * @note The function does not copy the buffer contents, it only assigns the pointer.
 *       The caller is responsible for managing the memory for `buff`.
 * @warning Transfers buffer ownership
 */
fchnk_t *fchnk_ptor(char *const buff, const size_t chksz);


/**
 * @brief Writes the contents of a file chunk to a specified file.
 *
 * Opens a file in write mode and writes the buffer stored in the given `fchnk_t` structure
 * to that file. The write operation attempts to write exactly `chksz` characters.
 *
 * @param fname The name of the file to write to.
 * @param chnk Pointer to the file chunk structure containing the data and its size.
 *
 * @return `true` if the entire buffer was successfully written to the file, `false` otherwise.
 *
 * @note If the file cannot be opened or writing fails, appropriate errors are printed using `perror()`.
 */
bool fwrite_fchnk(const char *fname, const fchnk_t *chnk);

/**
 * @brief Reads the contents of a binary file into a dynamically allocated buffer
 *        and stores it in a new fchnk_t structure.
 *
 * This function opens the specified file in binary read mode, reads its entire content
 * into a dynamically allocated buffer, and initializes a new `fchnk_t` object with the buffer
 * and its size. The caller is responsible for managing and freeing the memory of the returned object.
 *
 * @param fname The path to the file to be read.
 *
 * @return A pointer to an `fchnk_t` structure containing the file data and size,
 *         or NULL if an error occurs (e.g., file open failure, memory allocation failure).
 * @retval NULL on read failure
 * @note The returned buffer is null-terminated, but the file is read in binary mode,
 *       so the null terminator is not counted in `chksz`.
 * @note Ensure `fchnk_ctor()` properly initializes the `fchnk_t` object.
 *
 * @see fchnk_ctor()
 */
fchnk_t *get_fchnk(const char *fname);


/** @}*/ // End of LexerIO group

#endif
