#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket != -1)
    {
        struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(8000)
        };

        if (!bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)))
        {
            while (!listen(server_socket, 5))
            {
                struct sockaddr_in client_address = {};
                socklen_t socket_length = sizeof(client_address);
                int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &socket_length);

                if (client_socket > 0)
                {
                    char buffer[1024] = {};
                    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

                    if (bytes_read > 0)
                    {
                        printf("%s\n", buffer);

                        const char* response = 
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 13\r\n"
                            "\r\n"
                            "Hello, world!";

                        if (write(client_socket, response, strlen(response)) > -1)
                        {
                            close(client_socket);
                            printf("Responsed successfully!\n");
                        }
                        else
                        {
                            perror("Failed to write reponse!\n");
                        }
                    }
                    else
                    {
                        perror("Failed to read from client!\n");
                        close(client_socket);
                        continue;
                    }
                }
                else
                {
                    perror("Failed to accept client socket!\n");
                    continue;
                }
            }
        }

        if (!shutdown(server_socket, SHUT_RDWR))
        {
            if (!close(server_socket))
            {
                printf("Server shutdown successfully!\n");
            }
            else
            {
                perror("Failed to close socket!");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            perror("Failed to shutdown read and write!");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("Failed to create a socket!");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
