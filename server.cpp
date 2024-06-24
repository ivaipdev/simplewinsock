#include "server.h"

int main() {
	WSADATA wsaData;    //creating a WSADATA structure
	int obvEr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (obvEr != 0) {
		std::cerr << "Failed to initialize Winsock: " << obvEr << std::endl;
		WSACleanup();
		return 1;
	}
	puts("Winsock initialized!\n");		//successfully

	SOCKET listener;	//creating a listening (server) socket
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == INVALID_SOCKET) {
		std::cerr << "Socket creation fault: " << WSAGetLastError() << std::endl;
		closesocket(listener);
		WSACleanup();
		return 1;
	}
	puts("Server socket created!\n");	//successfully

	in_addr iptonum;	//a structure for translate ip to numbers
	obvEr = inet_pton(AF_INET, "127.0.0.1", &iptonum);	// DON`T FORGET TO CHANGE IP AND PORT
	puts("IP translated!\n");	//successfully

	sockaddr_in servinfo;		//a structure that will keep server information
	ZeroMemory(&servinfo, sizeof(servinfo));	//getting itself to know how big is it :)
	servinfo.sin_family = AF_INET;		//setting all the details
	servinfo.sin_addr = iptonum;
	servinfo.sin_port = htons(8080);			//PORT TO CHANGE ITSELF
	puts("All details setted!\n");		//successfully

	obvEr = bind(listener, (sockaddr*)&servinfo, sizeof(servinfo));		//binding
	if (obvEr == SOCKET_ERROR) {
		std::cerr << "Bind operation fault: " << WSAGetLastError() << std::endl;
		closesocket(listener);
		WSACleanup();
		return 1;
	}
	puts("Binded successfully!\n");		//successfully
	
	sockaddr_in clientinfo;		//a structure that will keep all client information
	ZeroMemory(&clientinfo, sizeof(clientinfo));		//also getting itself to know how big is it :)

	SOCKET clsocket = socket(clientinfo.sin_family, NULL, NULL);	//creating client socket
	obvEr = listen(listener, 2);	//setting it for listening process
	if (obvEr == SOCKET_ERROR) {
		std::cerr << "Listening fault: " << WSAGetLastError() << std::endl;
		closesocket(listener);
		closesocket(clsocket);
		WSACleanup();
		return 1;
	}
	puts("Listening...");		//successfully
	
	int sizec = sizeof(clientinfo);
	clsocket = accept(listener, (sockaddr*)&clientinfo, &sizec);		//accepting every connection
	if (clsocket == INVALID_SOCKET) {
		std::cerr << "Failed to accept connection : " << WSAGetLastError() << std::endl;
		closesocket(listener);
		closesocket(clsocket);
		WSACleanup();
		return 1;
	}
	puts("Accepted!\n");		//successfully accepted

	std::ifstream file("index.html", std::ios::binary | std::ios::ate);		//opening your website index.html file
	if (!file.is_open()) {
		std::cerr << "Unable to open index.html" << std::endl;
		closesocket(clsocket);
		closesocket(listener);
		WSACleanup();
		return 1;
	}		//successfully

	std::streampos fileSize = file.tellg();		//counting its length
	char* htmlbuff = new char[fileSize];		//creating a buffer to keep html code
	file.seekg(0, std::ios::beg);				//starting point	
	file.read(htmlbuff, fileSize);				//reading a html code from breggining point to the last length point ^_^
	int sizebuff = (int)fileSize;				//its length
	file.close();								//of course need to close it
	puts("Html imported!\n");					//successfully
	
	std::stringstream response;					//creating a stringstream type to keep http POST message
	response << "HTTP/1.1 200 OK\r\n"			//HTTP POST message 
		<< "Version: HTTP/1.1\r\n"
		<< "Content-Type: text/html; charset=utf-8\r\n"
		<< "Content-Length: " << sizebuff		
		<< "\r\n\r\n"
		<< htmlbuff;							//adding a html file buffer to the ending of HTTP message 
	
	char* rbuff = new char[200];				//buffer for HTTP GET message from client browser
	obvEr = recv(clsocket, rbuff, sizeof(rbuff), 0);		//recieving a GET HTTP 
	
	puts("GET recieved!\n");					// no comment :)
	
	obvEr = send(clsocket, response.str().c_str(), response.str().length(), 0);		//finally sending our HTTP POST message with our http file buffer into it
	if (obvEr == SOCKET_ERROR) {
		std::cerr << "Failed to send..." << std::endl;
	}
	puts("HTML sent!\n");								//successfully
	
  obvEr = shutdown(clsocket, SD_SEND);					//shutting down our server, because there`s nothing else to do
	if (obvEr == SOCKET_ERROR) {
		std::cerr << "Failed to shutdown>> " << WSAGetLastError() << std::endl;
	}
			
	puts("end of program...\n server is closing");
	delete[] htmlbuff;								//of course we need to delete temporary replacements (pointers to some parts of memory) of char arrays
	delete[] rbuff;
	closesocket(listener);							//closing connection to the both of our sockets
	closesocket(clsocket);
	WSACleanup();									//forsing Winsock to Clear Winsock dll library 
	return 0;
}
