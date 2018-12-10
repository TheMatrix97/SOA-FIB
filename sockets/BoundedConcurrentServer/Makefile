all: clientSocket launchClient serverSocketBounded

launchClient: launchClient.c
	gcc -o launchClient launchClient.c


serverSocketBounded: serverSocketBounded.c socketMng.o
	gcc -o serverSocketBounded -g serverSocketBounded.c socketMng.o

clientSocket: clientSocket.c socketMng.o
	gcc -o clientSocket -g clientSocket.c socketMng.o

socketMng.o: socketMng.c
	gcc -c -g socketMng.c

clean: 
	rm -f  clientSocket  serverSocketBounded socketMng.o launchClient
ultraclean:
	rm -f  clientSocket  serverSocketBounded socketMng.o launchClient client_* launch_info
