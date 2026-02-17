# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin
TESTSDIR = tests

# Target executables
TARGET = $(BINDIR)/ccwc
TEST_TARGET = $(BINDIR)/test

# Source files and object files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Application objects (excluding main.o for linking with tests)
APP_OBJECTS = $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))

TEST_SOURCES = $(wildcard $(TESTSDIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTSDIR)/%.c=$(BUILDDIR)/test_%.o)

# Default target: compile the main project
all: $(TARGET)

# Rule to link the main executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@
	@echo "Build successful. Executable is at $(TARGET)"

# Rule to compile source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Target to run the main project
ccwc: $(TARGET)
	@./$(TARGET)

# Target to build and run tests
tests: $(TEST_TARGET)
	@./$(TEST_TARGET)

# Rule to link the test executable
$(TEST_TARGET): $(TEST_OBJECTS) $(APP_OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@
	@echo "Test build successful. Executable is at $(TEST_TARGET)"

# Rule to compile test files into object files
$(BUILDDIR)/test_%.o: $(TESTSDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Clean up build artifacts
clean:
	@rm -rf $(BUILDDIR)/* $(BINDIR)/*
	@echo "Cleaned build and bin directories."

.PHONY: all run tests clean
