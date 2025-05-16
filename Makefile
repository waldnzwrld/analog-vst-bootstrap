# Build directory
BUILD_DIR = build

# Get number of CPU cores
NUM_CORES := $(shell sysctl -n hw.ncpu)

# Default target
all: build

# Create build directory and run CMake
configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

# Build the project
build: configure
	@cd $(BUILD_DIR) && cmake --build . -j$(NUM_CORES)

# Clean build files
clean:
	@rm -rf $(BUILD_DIR)

# Clean and rebuild
rebuild: clean build

# Run the plugin (if built as standalone)
run: build
	@./$(BUILD_DIR)/ArchitextureStudiosAnalogCore_artefacts/Standalone/ArchitextureStudiosAnalogCore.app/Contents/MacOS/ArchitextureStudiosAnalogCore


# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Default target, builds the project"
	@echo "  configure  - Creates build directory and runs CMake"
	@echo "  build      - Builds the project"
	@echo "  clean      - Removes build directory"
	@echo "  rebuild    - Cleans and rebuilds the project"
	@echo "  run        - Runs the standalone version of the plugin"
	@echo "  test       - Runs the test suite"
	@echo "  help       - Shows this help message"

.PHONY: all configure build clean rebuild run help test 