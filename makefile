BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/termui

.PHONY: all install

all: test

test:
	@clear
	@rm -rf .cache build
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Debug .. && $(MAKE) -j
	@./build/test_program

install:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Release .. && sudo $(MAKE) -j install
