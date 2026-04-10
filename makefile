BUILD_DIR := build

.PHONY: all

all: test

test:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_EXAMPLES=ON .. && \
	$(MAKE) -j

	for f in b*/e*/*.test; do ./"$$f"; done

install:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake -DCMAKE_BUILD_TYPE=Release .. && \
	sudo $(MAKE) -j install

clean:
	@rm -rf $(BUILD_DIR) .cache

check:
	@/opt/homebrew/opt/llvm/bin/clang-tidy \
	$$(find src include -name '*.cpp' -o -name '*.hpp') \
	-checks='clang-analyzer-core.uninitialized.*,cppcoreguidelines-pro-type-member-init' -- -std=c++20 -Iinclude

gtest:
	mkdir -p $(BUILD_DIR)

	cmake -B $(BUILD_DIR) \
	-DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	-DBUILD_TESTS=ON .

	make -C $(BUILD_DIR) -j

	ctest --test-dir $(BUILD_DIR) --output-on-failure

tree:
	tree include src

FORMAT_FILES := $(shell find src include examples -type f \( -name '*.c' -o -name '*.h' -o -name '*.cpp' -o -name '*.hpp' \))

format:
	clang-format -i $(FORMAT_FILES)

format-check:
	clang-format --dry-run --Werror $(FORMAT_FILES)
