CC       := cc
CFLAGS   := -O2 -Wall -Wextra -std=c11 -Iinclude

# ---- Check (unit test) flags (fallback if pkg-config is missing) ----
CHECK_CFLAGS := $(shell pkg-config --cflags check 2>/dev/null)
CHECK_LIBS   := $(shell pkg-config --libs   check 2>/dev/null)
ifeq ($(CHECK_CFLAGS),)
  CHECK_CFLAGS := -I/opt/homebrew/Cellar/check/0.15.2/include -D_THREAD_SAFE
  CHECK_LIBS   := -L/opt/homebrew/Cellar/check/0.15.2/lib -lcheck
endif

SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build
APP_DIR   := $(BUILD_DIR)/app
TEST_DIRB := $(BUILD_DIR)/tests

APP_BIN   := $(APP_DIR)/calendar_app
TEST_BIN  := $(TEST_DIRB)/run_tests

APP_SRCS  := $(SRC_DIR)/calendar.c $(SRC_DIR)/notes.c $(SRC_DIR)/main.c
LIB_SRCS  := $(SRC_DIR)/calendar.c $(SRC_DIR)/notes.c      # library code only (no main)
TEST_SRCS := $(TEST_DIR)/test_calendar.c $(TEST_DIR)/test_runner.c $(TEST_DIR)/test_smoke.c

.PHONY: all test clean

all: $(APP_BIN)

$(APP_BIN): | $(APP_DIR)
	$(CC) $(CFLAGS) $(APP_SRCS) -o $@

test: $(TEST_BIN)
	$(TEST_BIN) -q

$(TEST_BIN): | $(TEST_DIRB)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) $(TEST_SRCS) $(LIB_SRCS) -o $@ $(CHECK_LIBS)

$(APP_DIR) $(TEST_DIRB):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
