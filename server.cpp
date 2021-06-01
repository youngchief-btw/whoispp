#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // setsockopt(sockfd, 0, O_NONBLOCK, 0, 0);
    if (sockfd < 0)
    {
        perror("ERROR Opening socket.");
        exit(1);
    }
    else
    {
        cout << "Opened socket!\n";
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = 1234; // Port number goes here.

    // Configure server details.
    serv_addr.sin_family = AF_INET;         // More details on types: man socket
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Allow any interface.
    serv_addr.sin_port = htons(portno);

    cout << "Port: " << portno << "\n"; // Output the port number, you could also ask for a custom port and use that.

    // Start binding the socket.
    if (
        bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR binding.");
        exit(1);
    }
    else
    {
        cout << "Binded socket!\n";
    }

    // Nap until it gets an incomming connection.
    if (listen(sockfd, 5) < 0)
    {
        perror("ERROR Listening to the socket");
    }
    else
    {
        // change 5 to how ever many connections you want in queue before refusing to connect
        cout << "Now listening to the socket!\n";
        clilen
 = sizeof(cli_addr); // 16 is default, change this to how ever many (or little) you want!
        cout << "Number of connections allowed in queue before refusing to connect: " << clilen
 << "\n";
    }

    while(1) 
    {
    // Accept the connection from client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR Socket accept");
        exit(1);
    }
    else
    {
        cout << "Accepted connection!\n"; // << getpeername(newsockfd, (struct sockaddr *)&cli_addr, &addrlen) << "\n";
    }

    // Connected established, let's start communicating
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("ERROR Reading from socket!\n");
        exit(1);
    }
    else
    {
        cout << "Connection estabilished!\n";
    }

    printf("Recieved message: \n%s\n", buffer);

    // Store what the client asked for in a variable
    const char* res = buffer;
    // if (res == "-q version") {
    //     n = write(newsockfd, "whoispp - https://github.com/youngchief-btw/whoispp", strlen("whoispp - https://github.com/youngchief-btw/whoispp"));
    // if (n < 0)
    // {
    //     perror("ERROR writing to socket failed.");
    //     exit(1);
    // }
    // else
    // {
    //     cout << "Sent reply message!\n";
    // }
    // }

    // Send something back to client!
    fstream file;
    string fout;
    string reply;
   file.open(res, ios::in); //open a file to perform read operation using file object
   if (file.is_open()){   //checking whether the file is open
      // Keep reading the file
        while(getline(file, fout))
        {
            // print the line on the standard output
            // cout << fout << "\n";
            reply == fout + "\r\n"; 
        }
      file.close();   //close the file object.
   } else {
        cout << "Unable to open the file!\nSending error as the reply!\n";
        // if (reply == "\r\n") {
            reply.assign("% Not sure what happened.\r\n% Try another WHOIS server.\r\n");
            // cout << reply << "\n"; //debug.
        // };
    }
    n = write(newsockfd, reply.c_str(), strlen(reply.c_str()));
    if (n < 0)
    {
        perror("ERROR writing to socket failed.");
        exit(1);
    }
    else
    {
        cout << "Sent reply message!\nReply:\n" << reply << "\n";
    }
    close(newsockfd); // Close the connection to indicate we finished with the query.
    }


   // Close socket
    close(sockfd);
    
    return 0;
};