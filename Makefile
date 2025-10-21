# Makefile for building and running performance bug tests
#
# Usage:
#   make build     - Build all tests with CMake
#   make test      - Run all tests
#   make clean     - Clean build artifacts
#   make validate  - Run validation script

.PHONY: all build test clean validate help

# Default target
all: build

# Build all tests using CMake
build:
	@echo "Building tests with CMake..."
	@mkdir -p build
	@cd build && cmake .. && make
	@echo "Build complete!"

# Run all tests
test: build
	@echo "Running tests..."
	@cd build && ctest --output-on-failure

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf build
	@echo "Clean complete!"

# Run validation script
validate:
	@echo "Running validation script..."
	@python3 validate_suite.py

# Display help
help:
	@echo "ParEval Performance Bug Suite - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make build    - Build all tests with CMake"
	@echo "  make test     - Run all tests"
	@echo "  make clean    - Clean build artifacts"
	@echo "  make validate - Run validation script"
	@echo "  make help     - Display this help message"
