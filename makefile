BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/termui

.PHONY: all install

all: test

test:
	@clear
	@rm -rf .cache build
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && $(MAKE) -j
	@./build/test_program

install:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Release .. && sudo $(MAKE) -j install

clean:
	@rm -rf $(BUILD_DIR) 

debug:
	@clear
	@rm -rf .cache build
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && $(MAKE) -j
	@lldb  ./build/test_program
