BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/termui

.PHONY: all install

all: install

test:
	@rm -rf .cache build
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Debug .. && $(MAKE) -j

install:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Release .. && sudo $(MAKE) -j install
