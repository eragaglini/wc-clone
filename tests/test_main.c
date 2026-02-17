#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "counter.h"

// Helper macro for assertions
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed in %s, function %s, line %d: %s\n", __FILE__, __func__, __LINE__, message); \
            exit(1); \
        } \
    } while (0)

#define ASSERT_EQ(a, b, message) \
    do { \
        if ((a) != (b)) { \
            fprintf(stderr, "Assertion failed in %s, function %s, line %d: %s\n", __FILE__, __func__, __LINE__, message); \
            fprintf(stderr, "  Expected: %lld\n", (long long)(b)); \
            fprintf(stderr, "  Actual:   %lld\n", (long long)(a)); \
            exit(1); \
        } \
    } while (0)

// Function to create a temporary file with given content for testing
FILE* create_temp_file(const char* content) {
    char tmp_name[] = "/tmp/ccwc_test_XXXXXX";
    int fd = mkstemp(tmp_name);
    if (fd == -1) {
        perror("mkstemp failed");
        return NULL;
    }

    FILE* fp = fdopen(fd, "wb+");
    if (fp == NULL) {
        perror("fdopen failed");
        close(fd);
        return NULL;
    }

    if (fwrite(content, 1, strlen(content), fp) != strlen(content)) {
        fprintf(stderr, "fwrite failed\n");
        fclose(fp);
        return NULL;
    }
    fflush(fp);
    rewind(fp);
    
    // Keep the file open for the test, it will be closed (and deleted) by the caller
    return fp;
}


void test_empty_file() {
    long long lines = 0, words = 0, bytes = 0;
    FILE* fp = create_temp_file("");
    ASSERT(fp != NULL, "Failed to create temp file for empty file test");

    count_all(fp, &lines, &words, &bytes);
    fclose(fp);

    ASSERT_EQ(lines, 0, "Empty file should have 0 lines");
    ASSERT_EQ(words, 0, "Empty file should have 0 words");
    ASSERT_EQ(bytes, 0, "Empty file should have 0 bytes");
    printf("test_empty_file: PASSED\n");
}

void test_simple_line() {
    long long lines = 0, words = 0, bytes = 0;
    const char* text = "hello world from C";
    FILE* fp = create_temp_file(text);
    ASSERT(fp != NULL, "Failed to create temp file for simple line test");

    count_all(fp, &lines, &words, &bytes);
    fclose(fp);

    ASSERT_EQ(lines, 0, "Simple line should have 0 lines (no newline character)");
    ASSERT_EQ(words, 4, "Simple line should have 4 words");
    ASSERT_EQ(bytes, 18, "Simple line should have 18 bytes");
    printf("test_simple_line: PASSED\n");
}

void test_multiple_lines() {
    long long lines = 0, words = 0, bytes = 0;
    const char* text = "line 1\nline 2 has three words\n";
    FILE* fp = create_temp_file(text);
    ASSERT(fp != NULL, "Failed to create temp file for multiple lines test");
    
    count_all(fp, &lines, &words, &bytes);
    fclose(fp);

    ASSERT_EQ(lines, 2, "Should count 2 lines");
    ASSERT_EQ(words, 7, "Should count 7 words");
    ASSERT_EQ(bytes, 30, "Should count 30 bytes");
    printf("test_multiple_lines: PASSED\n");
}

void test_tricky_whitespace() {
    long long lines = 0, words = 0, bytes = 0;
    const char* text = "  leading spaces  \n and \t tabs \n and multiple   spaces between\n";
    FILE* fp = create_temp_file(text);
    ASSERT(fp != NULL, "Failed to create temp file for tricky whitespace test");

    count_all(fp, &lines, &words, &bytes);
    fclose(fp);

    ASSERT_EQ(lines, 3, "Tricky whitespace should have 3 lines");
    ASSERT_EQ(words, 8, "Tricky whitespace should have 8 words");
    ASSERT_EQ(bytes, 63, "Tricky whitespace should have 63 bytes");
    printf("test_tricky_whitespace: PASSED\n");
}

void test_only_newlines() {
    long long lines = 0, words = 0, bytes = 0;
    FILE* fp = create_temp_file("\n\n\n");
    ASSERT(fp != NULL, "Failed to create temp file for only newlines test");

    count_all(fp, &lines, &words, &bytes);
    fclose(fp);

    ASSERT_EQ(lines, 3, "Should count 3 lines for only newlines");
    ASSERT_EQ(words, 0, "Should count 0 words for only newlines");
    ASSERT_EQ(bytes, 3, "Should count 3 bytes for only newlines");
    printf("test_only_newlines: PASSED\n");
}

int main() {
    printf("--- Running ccwc tests ---\n");
    test_empty_file();
    test_simple_line();
    test_multiple_lines();
    test_tricky_whitespace();
    test_only_newlines();
    printf("--- All tests finished ---\n");
    return 0;
}
