MIX = mix
CFLAGS += -g -O3 -ansi -pedantic -Wall -Wextra -Wno-unused-parameter

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH)

CFLAGS += -I/usr/local/include -I/usr/include -L/usr/local/lib -L/usr/lib
CFLAGS += -DWEBVIEW_GTK=1
CFLAGS += -std=gnu99 -Wno-unused-function `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`

ifneq ($(OS),Windows_NT)
	CFLAGS += -fPIC

	ifeq ($(shell uname),Darwin)
		LDFLAGS += -dynamiclib -undefined dynamic_lookup
	endif
endif

.PHONY: all webview clean

all: priv/webview.so

priv/webview.so: src/webview.c
	mkdir -p priv
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ src/webview.c

clean:
	$(MIX) clean
	$(RM) priv/*
