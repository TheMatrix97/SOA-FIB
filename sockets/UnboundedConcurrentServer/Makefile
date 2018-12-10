all: clientSocket launchClient serverSocketUnbounded

launchClient: launchClient.c
	gcc -o launchClient launchClient.c

serverSocketUnbounded: serverSocketUnbounded.c socketMng.o
	gcc -o serverSocketUnbounded -g serverSocketUnbounded.c socketMng.o

clientSocket: clientSocket.c socketMng.o
	gcc -o clientSocket -g clientSocket.c socketMng.o

socketMng.o: socketMng.c
	gcc -c -g socketMng.c

clean: 
	rm -f clientSocket serverSocketUnbounded  socketMng.o launchClient
ultraclean:
	rm -f clientSocket serverSocketUnbounded  socketMng.o launchClient client_* launch_info
