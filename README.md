Hello everyone, this is ivaip!
That`s Webserver complitely written in c++

By the way, thats my first expirience for coding something like that
so dont be that quite serious, its a pet-proj

to start the server you need just to download whole repos
1. download it
2. change ip, port and maximum amount of clients in server.cpp file
3. download all libs (full list down there) OR work in Visual Studio with regular c++ stuff
4. change html code to yours OR replace index.html your file (if it has different name dont forget to change the name in server.cpp)
5. pay your attention to the absence of css and js files, so you`ll need to connect it via index.html 

so, this whole thing working on Windows only
it using :
-WinSock(version 2) header file
-Ws2tcpip header file
-iphlpapi header file
-Windows header file
-iostream (really :(?)
-string 
-sstream (stringstream type)
-fstream (to import html code file)
