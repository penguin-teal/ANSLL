MKDIR 			:= mkdir -p --
RMDIR   		:= rm -Rf --
RM				:= rm --
PRINTF			:= printf --
CP				:= cp --
GITCLONETAG		:= git clone --single-branch -c advice.detachedHead=false --branch

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
TESTSRC			:= ./tests
TESTSRCS		:= $(wildcard $(TESTSRC)/*.c)
TESTBIN			:= ./testbin
TESTOUT		    := $(TESTBIN)/test

LIBS			:= ./libs
HBLIB			:= $(LIBS)/libhashedbrown.a
HBSRC			:= ./.hashedbrown
HBVERSION		:= 'v1.0.0'
UTF8LIB			:= $(LIBS)/libutf8encoder.a
UTF8SRC			:= ./.utf8encoder
UTF8VERSION		:= 'v1.1.0'

INCLUDEFLAGS	:= -I$(INCLUDE) -I$(LIBS)

DESTDIR			:= /usr/local/bin

.PHONY: release debug tests install clean

release: CURRENT_CFLAGS += -O3 -DNDEBUG
release: $(OUT)

debug: CURRENT_CFLAGS += -g3 -O0 -DDEBUG
debug: $(OUT) tests

$(OUT): $(SRCS) $(HBLIB) $(UTF8LIB)
	$(MKDIR) $(BIN)
	$(CC) $(CFLAGS) $(CURRENT_CFLAGS) $(INCLUDEFLAGS) $(LLVM_FLAGS) -L$(LIBS) -lhashedbrown $^ -o $@

$(HBLIB):
ifeq (,$(wildcard $(HBLIB)))
	$(MKDIR) $(LIBS)
	$(GITCLONETAG) $(HBVERSION) https://github.com/penguin-teal/hashedbrown $(HBSRC)
	make -C$(HBSRC) release CC=$(CC)
	cp $(HBSRC)/bin/libhashedbrown.a $(HBLIB)
	cp $(HBSRC)/include/hashedbrown.h $(LIBS)/hashedbrown.h
	$(RMDIR) $(HBSRC)
endif

$(UTF8LIB):
ifeq (,$(wilcard $(UTF8LIB)))
	$(MKDIR) $(LIBS)
	$(GITCLONETAG) $(UTF8VERSION) https://github.com/penguin-teal/utf8encoder $(UTF8SRC)
	make -C$(UTF8SRC) static CC=$(CC)
	cp $(UTF8SRC)/bin/libutf8encoder.a $(UTF8LIB)
	cp $(UTF8SRC)/include/utf8encoder.h $(LIBS)/utf8encoder.h
	$(RMDIR) $(UTF8SRC)
endif

tests: $(TESTOUT)
	$(RM) $^

$(TESTOUT): $(TESTSRC)/main.c
	$(PRINTF) 'Tests not implemented yet.\n'
	$(MKDIR) $(TESTBIN)
	$(CC) $(CFLAGS) -g3 -O0 -DDEBUG $^ -o $@

install:
	$(CP) $(OUT) $(DESTDIR)/ansllc

clean:
	$(RMDIR) $(TESTBIN) $(BIN) $(LIBS) $(HBSRC) $(UTF8SRC)
