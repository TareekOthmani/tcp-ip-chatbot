all: serveur client

serveur: serveur.c
	gcc -o serveur serveur.c 

client: client.c
	gcc -o client client.c

run: 
	./serveur
run1:
	./client

clean:
	rm client
	rm serveur


all:
	gcc -std=c11 -Wall -Wextra -pedantic src/chatbot.c -o chat

clean:
	rm -rf *o chat
