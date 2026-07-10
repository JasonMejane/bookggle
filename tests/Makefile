# Host tests for Bookggle PocketBook. Builds and runs the game logic
# on a regular dev machine, no PocketBook SDK or device needed,
# using the no-op stub in tests/stub/ for InkView.
#
#   make                    build + run all 6 test suites
#   make test-flow          full game scenario (4x4) only
#   make test-dice          dice roll, per ruleset (en-4x4/en-5x5/fr-4x4/fr-5x5), only
#   make test-integration   game_start() <-> dice, both board sizes, only
#   make test-i18n          translation completeness + fit-font only
#   make test-ruleset       ruleset selection logic (language x board_size) only
#   make test-5x5           full 5x5 game cycle + min_word_len=4 rule only
#   make clean              remove compiled test binaries
#
# Needs gcc and make only.

CC      := gcc
CFLAGS  := -Wall -Wextra -O2
SRC_DIR := ../src
INC_DIR := ../src/include
STUB_DIR := stub
BIN_DIR := bin

INCLUDES := -I$(INC_DIR) -I$(STUB_DIR)
STUB_SRC := $(STUB_DIR)/inkview_stub.c

I18N_SRCS := $(SRC_DIR)/i18n.c $(SRC_DIR)/i18n_strings_en.c \
             $(SRC_DIR)/i18n_strings_fr.c

RULESET_SRCS := $(SRC_DIR)/ruleset.c $(SRC_DIR)/ruleset_en_4x4.c \
                 $(SRC_DIR)/ruleset_en_5x5.c $(SRC_DIR)/ruleset_fr_4x4.c \
                 $(SRC_DIR)/ruleset_fr_5x5.c

COMMON_SRCS := $(SRC_DIR)/game_state.c $(SRC_DIR)/ui_fonts.c \
               $(SRC_DIR)/game_logic.c $(SRC_DIR)/timer.c \
               $(SRC_DIR)/dice.c $(I18N_SRCS) $(RULESET_SRCS) \
               $(SRC_DIR)/screen_mode_select.c \
               $(SRC_DIR)/screen_game.c $(SRC_DIR)/screen_multi_end.c \
               $(SRC_DIR)/input.c

# dice.c reads letters from the active ruleset, so even dice-only tests
# need i18n + ruleset sources linked.
DICE_SRCS := $(SRC_DIR)/game_state.c $(SRC_DIR)/dice.c \
             $(I18N_SRCS) $(RULESET_SRCS)

# game_logic.c calls i18n_str() and ruleset_active(), so both sets
# of sources are needed here too.
INTEGRATION_SRCS := $(SRC_DIR)/game_state.c $(SRC_DIR)/ui_fonts.c \
                     $(SRC_DIR)/game_logic.c $(SRC_DIR)/timer.c \
                     $(SRC_DIR)/dice.c $(I18N_SRCS) $(RULESET_SRCS)

I18N_TEST_SRCS := $(SRC_DIR)/game_state.c $(SRC_DIR)/ui_fonts.c $(I18N_SRCS)
RULESET_TEST_SRCS := $(SRC_DIR)/game_state.c $(I18N_SRCS) $(RULESET_SRCS)

.PHONY: all test-flow test-dice test-integration test-i18n test-ruleset test-5x5 test-timer-refresh clean

all: test-flow test-dice test-integration test-i18n test-ruleset test-5x5 test-timer-refresh

test-flow: | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(COMMON_SRCS) $(STUB_SRC) test_flow.c \
		-o $(BIN_DIR)/test_flow -lm
	@echo ">>> Running: test_flow"
	@./$(BIN_DIR)/test_flow

test-timer-refresh: | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(COMMON_SRCS) $(STUB_SRC) test_timer_refresh.c \
		-o $(BIN_DIR)/test_timer_refresh -lm
	@echo ">>> Running: test_timer_refresh"
	@./$(BIN_DIR)/test_timer_refresh

test-dice: | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(DICE_SRCS) $(STUB_SRC) test_dice.c \
		-o $(BIN_DIR)/test_dice -lm
	@echo ">>> Running: test_dice"
	@./$(BIN_DIR)/test_dice

test-integration: | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(INTEGRATION_SRCS) $(STUB_SRC) test_game_start_dice.c \
		-o $(BIN_DIR)/test_game_start_dice -lm
	@echo ">>> Running: test_game_start_dice"
	@./$(BIN_DIR)/test_game_start_dice

test-i18n: | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(I18N_TEST_SRCS) $(STUB_SRC) test_i18n.c \
		-o $(BIN_DIR)/test_i18n -lm
	@echo ">>> Running: test_i18n"
	@./$(BIN_DIR)/test_i18n

test-ruleset: | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(RULESET_TEST_SRCS) $(STUB_SRC) test_ruleset.c \
		-o $(BIN_DIR)/test_ruleset -lm
	@echo ">>> Running: test_ruleset"
	@./$(BIN_DIR)/test_ruleset

test-5x5: | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(INTEGRATION_SRCS) $(STUB_SRC) test_flow_5x5.c \
		-o $(BIN_DIR)/test_flow_5x5 -lm
	@echo ">>> Running: test_flow_5x5"
	@./$(BIN_DIR)/test_flow_5x5

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
