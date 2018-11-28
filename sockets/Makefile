all: serverSocket clientSocket launchClient serverSocketUnbounded serverSocketBounded
unbounded: serverSocketUnbounded clientSocket launchClient
bounded: serverSocketBounded clientSocket launchClient

launchClient: launchClient.c
	gcc -o launchClient launchClient.c

serverSocket: serverSocket.c socketMng.o
	gcc -o serverSocket -g serverSocket.c socketMng.o

serverSocketUnbounded: serverSocketUnbounded.c socketMng.o
	gcc -o serverSocketUnbounded -g serverSocketUnbounded.c socketMng.o

serverSocketBounded: serverSocketBounded.c socketMng.o
	gcc -o serverSocketBounded -g serverSocketBounded.c socketMng.o

clientSocket: clientSocket.c socketMng.o
	gcc -o clientSocket -g clientSocket.c socketMng.o

socketMng.o: socketMng.c
	gcc -c -g socketMng.c

clean: 
	rm -f serverSocket clientSocket serverSocketUnbounded serverSocketBounded socketMng.o launchClient
ultraclean:
	rm -f serverSocket clientSocket serverSocketUnbounded serverSocketBounded socketMng.o launchClient client_* launch_info
 