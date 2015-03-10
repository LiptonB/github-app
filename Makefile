all: server client deadlock deadlock_spin

server: server.c
	gcc -o server server.c

client: client.c
	gcc -o client client.c

deadlock: deadlock.c
	gcc -g -o deadlock deadlock.c -lpthread

deadlock_spin: deadlock_spin.c
	gcc -g -o deadlock_spin deadlock_spin.c -lpthread
