#include <stdio.h>
// #include <sys/socket.h>
#include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#define PORT 8080

struct User
{
    char name[35];
    char email[30];
    char password[20];
    int sleep;
};

struct User users[10];
pthread_mutex_t lock;

void *updateName(void *args)
{
    struct User user = *((struct User *)args);
    //pthread_mutex_lock(&lock);
    printf("Started thread %ld\n", pthread_self());
    sleep(user.sleep);
    for (int i = 0; i < 2; i++)
    {
        if (strcmp(users[i].email, user.email) == 0)
        {
            printf("%s\n", users[i].name);
            strcpy(users[i].name, user.name);
            printf("Updated to %s\n", users[i].name);
            printf("Ended thread %ld\n", pthread_self());
            //pthread_mutex_unlock(&lock);
            return (struct User *)&users[i];
        }
    }
}

int main()
{
    //pthread_mutex_init(&lock, NULL);
    struct User user1;
    strcpy(user1.email, "mail@mail.com");
    strcpy(user1.name, "name");
    struct User user3;
    strcpy(user3.email, "mail@mail.com");
    strcpy(user3.name, "name");
    struct User user2;
    strcpy(user2.email, "mail2@mail.com");
    strcpy(user2.name, "name2");
    users[0] = user1;
    users[1] = user2;

    pthread_t newThread1, newThread2;

    strcpy(user1.name, "new name1");
    user1.sleep = 1;
    pthread_create(&newThread1, NULL, &updateName, (void *)&user1);

    user3.sleep = 2;
    strcpy(user3.name, "new name2");
    pthread_create(&newThread2, NULL, &updateName, (void *)&user3);
    pthread_join(newThread1, NULL);
    pthread_join(newThread2, NULL);
    //pthread_mutex_destroy(&lock);
}