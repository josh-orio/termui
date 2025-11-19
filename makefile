BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/termui_test

.PHONY: all install

all: test

EXECUTABLE:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	-DBUILD_EXAMPLES=OFF .. && \
	$(MAKE) -j

test: EXECUTABLE
	@./$(EXECUTABLE)

debug: EXECUTABLE
	@lldb ./$(EXECUTABLE)

install:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake -DCMAKE_BUILD_TYPE=Release .. && \
	sudo $(MAKE) -j install

demos:
	@clear
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_EXAMPLES=ON .. && \
	$(MAKE) -j

clean:
	@rm -rf $(BUILD_DIR) .cache

check:
	@/opt/homebrew/opt/llvm/bin/clang-tidy \
	$$(find src include -name '*.cpp' -o -name '*.hpp') \
	-checks='clang-analyzer-core.uninitialized.*,cppcoreguidelines-pro-type-member-init' -- -std=c++20 -Iinclude

bind:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	-DBUILD_EXAMPLES=OFF \
	-DPYBIND=ON .. && \
	$(MAKE) -j

test_bind:
	python3 bind_test.py
