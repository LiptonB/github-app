all: server client deadlock

server: server.c
	gcc -o server server.c

client: client.c
	gcc -o client client.c

deadlock: deadlock.c
	gcc -o deadlock deadlock.c -lpthread
