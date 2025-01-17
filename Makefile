CC = gcc
CFLAGS = -Wall -Wextra -O3 -lm
CTESTFLAGS = -Wall -Wextra -g -lm
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = build/test

.DEFAULT_GOAL := all
.SECONDARY:

clean:
	rm -rf $(BUILD_DIR)

new-day-%:
	@echo "Creating day $*"
	cp ./src/template.c ./src/day_$*.c
	mkdir aoc-2024-inputs/day-$*/
	touch aoc-2024-inputs/day-$*/input.txt

build-%: $(BUILD_DIR)/day_%
	$(NOOP)

run-%: $(BUILD_DIR)/day_%
	@./$(BUILD_DIR)/day_$*

test-%: $(TEST_DIR)/day_%
	@echo "Starting tests..."
	@./$(TEST_DIR)/day_$* || true
	
test-%-debug: $(TEST_DIR)/day_%
	@echo "Starting tests..."
	./$(TEST_DIR)/day_$* --debug=gdb&
	gf2
	pkill day_$* 

# Build folder dependency
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Test folder dependency
$(TEST_DIR): $(BUILD_DIR)
	mkdir -p $(TEST_DIR)

# Build bin dependency
$(BUILD_DIR)/day_%: $(SRC_DIR)/main.c $(SRC_DIR)/utils.c $(SRC_DIR)/day_%.c $(BUILD_DIR)
	@echo "Building day $*"
	$(CC) $(CFLAGS) -DAOC_DAY=$(shell ./day_convert.sh $*) $(SRC_DIR)/day_$*.c $(SRC_DIR)/utils.c $(SRC_DIR)/main.c -o $(BUILD_DIR)/day_$*
	chmod +x $(BUILD_DIR)/day_$*

# Build test bin dependency
$(TEST_DIR)/day_%: $(SRC_DIR)/day_%.c $(SRC_DIR)/utils.c $(TEST_DIR)
	@echo "Building day $*"
	$(CC) $(CTESTFLAGS) -D TEST -lcriterion $(SRC_DIR)/day_$*.c $(SRC_DIR)/utils.c -o $(TEST_DIR)/day_$*
	chmod +x $(TEST_DIR)/day_$*
