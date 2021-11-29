#include <stdlib.h>

struct User
{
    uint id;
    char name[20];
    char email[20];
    char password[20];
};

struct data
{
    int create;
    int read;
    int update;
    int delete;
    struct User user;
};