server_debug:
	gcc -o build/server.out -g src/main.c src/server_utils.c
	gcc -o build/tests.out tests/unit_tests.c src/server_utils.c
	build/tests.out

server:
	gcc -o build/server.out src/main.c src/server_utils.c
