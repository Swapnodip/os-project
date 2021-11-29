#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "os_proj_types.c"
#define PORT 8000
#define MAX_USERS 10

struct User users[MAX_USERS];
pthread_mutex_t lock;
int pointer = -1;

struct User createUser(struct User user)
{
    if (pointer == MAX_USERS - 1)
    {
        printf("Max limit reached, cannot create new user\n");
        user.id = -1;
        return user;
    }
    pthread_mutex_lock(&lock);
    user.id = ++pointer;
    users[pointer] = user;
    pthread_mutex_unlock(&lock);
    return user;
}

//Only user.id is relevant for delete and read,
//the remaining fields of user do not need to be initialised for these operations

struct User deleteUser(struct User user)
{
    if(user.id>pointer||user.id<0)
    {
        printf("Entry does not exist\n");
        return user;
    }
    pthread_mutex_lock(&lock);
    struct User ret=users[user.id];
    for(int i=user.id;i<pointer;i++)
    {
        users[i]=users[i+1];
    }
    pointer--;
    pthread_mutex_unlock(&lock);
    return ret;
}

//Mutex lock is not needed for read operation because db is not being updated

struct User readUser(struct User user)
{
    if(user.id>pointer||user.id<0)
    {
        printf("Entry does not exist\n");
        return user;
    }
    return users[user.id];
}

void *serveThread(void *args)
{
    struct data received_data;
    int new_socket = (*(int *)args);
    read(new_socket, (struct data *)&received_data, sizeof(struct data));
    struct data send_data;
    if (received_data.create)
    {
        // Create new User
        send_data.user = createUser(received_data.user);
    }
    else if(received_data.delete)
    {
        send_data.user = deleteUser(received_data.user);
    }
    else if(received_data.read)
    {
        send_data.user = readUser(received_data.user);
    }
    write(new_socket, (struct data *)&send_data, sizeof(send_data));
    close(new_socket);
}

void serve(int new_socket)
{
    pthread_t newThread;
    pthread_create(&newThread, NULL, &serveThread, (void *)&new_socket);
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_mutex_init(&lock, NULL);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        serve(new_socket);
    }
    close(server_fd);
    pthread_mutex_destroy(&lock);
    return 0;
}