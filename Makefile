MKDIR 			:= mkdir -p --
RMDIR   		:= rm -Rf --
RM				:= rm --
PRINTF			:= printf --
CP				:= cp --

EXTRA_CFLAGS	:=
WARNINGS		:= -Wall -Wextra -Wfloat-equal -Wundef
ERRORS			:= -Werror=implicit-int -Werror=implicit-function-declaration
CFLAGS 		    := $(WARNINGS) $(ERRORS) -std=c99 $(EXTRA_CFLAGS) -D_POSIX_C_SOURCE
LLVM_FLAGS		:= $(shell llvm-config --cflags --ldflags --system-libs --libs all)
CURRENT_CFLAGS	:=

BIN   		    := ./bin
INCLUDE		    := ./include
SRC 		    := ./src
SRCS  		    := $(wildcard $(SRC)/*.c)
OUT				:= $(BIN)/ansllc
INCLUDEFLAGS	:= -I$(INCLUDE)
TESTSRC			:= ./tests
TESTSRCS		:= $(wildcard $(TESTSRC)/*.c)
TESTBIN			:= ./testbin
TESTOUT		    := $(TESTBIN)/test

CHECK_TEST_FLAGS:= -pthread -lcheck -lrt -lm

DESTDIR			:= /usr/local/bin

.PHONY: release debug tests install clean

release: CURRENT_CFLAGS += -O3 -DNDEBUG
release: $(OUT)

debug: CURRENT_CFLAGS += -g3 -O0 -DDEBUG
debug: $(OUT) tests

$(OUT): $(SRCS)
	$(MKDIR) $(BIN)
	$(CC) $(CFLAGS) $(CURRENT_CFLAGS) $(INCLUDEFLAGS) $(LLVM_FLAGS) $^ -o $(OUT)

tests: $(TESTOUT)
	$(RM) $^

$(TESTOUT): $(TESTSRC)/main.c
	$(PRINTF) 'NEEDED: check\n'
	$(MKDIR) $(TESTBIN)
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(CHECK_TEST_FLAGS) -o $@ $^
	./$@

install:
	$(CP) $(OUT) $(DESTDIR)/ansllc

clean:
	$(RMDIR) $(TESTBIN) $(BIN)
