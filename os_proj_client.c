#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "os_proj_types.c"
#define PORT 8000

void initRequest(struct data *request)
{
    request->create = 0;
    request->delete = 0;
    request->read = 0;
    request->update = 0;
}

void displayUser(struct User user)
{
    printf("ID: %d, Name: %s\n Email: %s\n", user.id, user.name, user.email);
}

int main()
{
    int sd, n;
    struct sockaddr_in seraddress;
    struct data req;
    int requestType;
    struct data request, response;
    struct User user;
    char buf[20];
    printf("Enter 1 to create, 2 to read, 3 to update, 4 to delete\n");
    initRequest(&request);
    scanf("%d", &requestType);
    if (requestType == 1)
    {
        printf("Enter name, email and password of new user\n");
        scanf("%s", buf);
        strcpy(user.name, buf);
        scanf("%s", buf);
        strcpy(user.email, buf);
        scanf("%s", buf);
        strcpy(user.password, buf);
        request.create = 1;
    }
    request.user = user;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    seraddress.sin_family = AF_INET;
    seraddress.sin_addr.s_addr = INADDR_ANY;
    seraddress.sin_port = htons(PORT);
    connect(sd, (struct sockaddr *)&seraddress, sizeof(seraddress));
    n = write(sd, (struct data *)&request, sizeof(request));
    if (n < 0)
    {
        printf("Write error\n");
    }
    n = read(sd, (struct data *)&response, sizeof(response));
    if (n < 0)
    {
        printf("Read error\n");
    }
    printf("---------- RESPONSE ----------\n");
    displayUser(response.user);
    close(sd);
}
