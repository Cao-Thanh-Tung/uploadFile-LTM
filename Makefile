client: TCP_Client/client.c TCP_Client/helper.h TCP_Client/helper.c TCP_Client/socketX.h TCP_Client/socketX.c  server 
	gcc -o client TCP_Client/client.c TCP_Client/helper.h TCP_Client/socketX.c TCP_Client/socketX.h TCP_Client/helper.c
server: TCP_Server/server.c TCP_Server/extension.h TCP_Server/extension.c TCP_Server/socketServer.h TCP_Server/socketServer.c
	gcc -o server TCP_Server/server.c TCP_Server/extension.h TCP_Server/extension.c TCP_Server/socketServer.h TCP_Server/socketServer.c
