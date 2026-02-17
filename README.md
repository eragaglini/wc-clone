# ccwc - Word Count Tool

`ccwc` is a simple command-line utility written in C that mimics the basic functionality of the `wc` (word count) command in Unix-like operating systems. It can count bytes, words, and lines from a given file or from standard input.

## Features

- Count bytes (`-c`)
- Count words (`-w`)
- Count lines (`-l`)
- Reads from a file or from `stdin`.
- Comprehensive unit tests.

## Prerequisites

- A C compiler (like `gcc` or `clang`)
- `make`

## Building the Project

To compile the project, run the `make` command:

```bash
make
```

This will create the executable at `bin/ccwc`.

## Usage

You can run the tool by providing a filename as an argument:

```bash
./bin/ccwc [options] <filename>
```

Or you can pipe data into it:

```bash
cat <filename> | ./bin/ccwc [options]
```

### Options

- `-c`: Prints the byte count.
- `-l`: Prints the line count.
- `-w`: Prints the word count.

If no options are provided, the tool will print the line, word, and byte counts by default.

### Examples

Count everything in `test.txt`:
```bash
./bin/ccwc test.txt
```

Count only the lines in `test.txt`:
```bash
./bin/ccwc -l test.txt
```

Count words from standard input:
```bash
echo "hello world" | ./bin/ccwc -w
```

## Running Tests

The project includes a suite of unit tests. To compile and run the tests, use the `tests` target in the Makefile:

```bash
make tests
```

## Cleaning Up

To remove all build artifacts (object files and executables), run:

```bash
make clean
```
