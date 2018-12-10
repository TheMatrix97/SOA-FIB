all: clientSocket launchClient serverSocketThreads

launchClient: launchClient.c
	gcc -o launchClient launchClient.c

serverSocketThreads: serverSocketThreads.c socketMng.o
	gcc -o serverSocketThreads -g serverSocketThreads.c socketMng.o -pthread

clientSocket: clientSocket.c socketMng.o
	gcc -o clientSocket -g clientSocket.c socketMng.o

socketMng.o: socketMng.c
	gcc -c -g socketMng.c

clean: 
	rm -f  clientSocket  serverSocketThreads socketMng.o launchClient
ultraclean:
	rm -f  clientSocket  serverSocketThreads socketMng.o launchClient client_* launch_info
