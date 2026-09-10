CC = gcc
CDEBUGFLAGS = -ggdb3 -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -Wno-format
CRELFLAGS = -O3

ci: ci_main.c ./vendor/sv.h ./vendor/darr.h
	@$(CC) ci_main.c -o $@ $(CRELFLAGS) -I./vendor/ -std=c99 -D_DEFAULT_SOURCE

ci_debug: ci_main.c ./vendor/sv.h ./vendor/darr.h
	@$(CC) ci_main.c -o $@ $(CDEBUGFLAGS) -I./vendor/ -std=c99 -D_DEFAULT_SOURCE

.PHONY: run drun clean install

install: ci
	@cp ./ci /usr/bin/

uninstall: ci
	@rm /usr/bin/ci

run: ci
	./ci

debug: ci_debug
	./ci_debug

clean:
	@rm -rf ci
	rm -rf ci_debug
