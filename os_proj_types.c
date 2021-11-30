#include <stdlib.h>

struct User
{
    uint id;
    char name[30];
    char email[30];
    char password[30];
};

struct data
{
    int create;
    int read;
    int update;
    int delete;
    struct User user;
};