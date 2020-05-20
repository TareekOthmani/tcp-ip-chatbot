all: serveur client

serveur: src/serveur.o src/chatbot.o
	gcc -o serveur src/serveur.o src/chatbot.o 

client: src/client.o
	gcc -o client src/client.o

serveur.o: src/serveur.c
	gcc -o serveur.o -c serveur.c

client.o: src/client.c
	gcc -o src/client.o -c src/client.c


chatbot.o: src/chatbot.c src/chatbot.h
	gcc -o src/chatbot.o -c src/chatbot.c 

serv: 
	./serveur
clie:
	./client


clean:
	rm -rf ./src/*.o
	rm -rf serveur client