# Build directory
BUILD_DIR = build

# Default target
all: init-submodules build

# Initialize and update submodules
init-submodules:
	@echo "Initializing submodules..."
	@git submodule update --init --recursive

# Create build directory and run CMake
configure: init-submodules
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

# Build the project
build: configure
	@cd $(BUILD_DIR) && cmake --build .

# Clean build files
clean:
	@rm -rf $(BUILD_DIR)

# Clean and rebuild
rebuild: clean build

# Run the plugin (if built as standalone)
run: build
	@./$(BUILD_DIR)/PedalPower_artefacts/Standalone/PedalPower.app/Contents/MacOS/PedalPower

# Update submodules to their latest versions
update-submodules:
	@echo "Updating submodules..."
	@git submodule update --remote --merge

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Default target, builds the project"
	@echo "  init-submodules  - Initialize and update submodules"
	@echo "  configure        - Creates build directory and runs CMake"
	@echo "  build           - Builds the project"
	@echo "  clean           - Removes build directory"
	@echo "  rebuild         - Cleans and rebuilds the project"
	@echo "  run             - Runs the standalone version of the plugin"
	@echo "  update-submodules - Updates all submodules to their latest versions"
	@echo "  help            - Shows this help message"

.PHONY: all configure build clean rebuild run help init-submodules update-submodules 