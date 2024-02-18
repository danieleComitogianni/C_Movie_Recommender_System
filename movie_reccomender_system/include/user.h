#ifndef USER_H
#define USER_H

typedef struct {
    char userName[100];
} User;

void loadUsers(User users[], int* userCount);

int checkUser(char* name);

void addUserAndUpdateRatingsFile(const char* name, int userCount);

int getUserIndex(char* name);

void userRating_ByIndex(int userIndex, int movieIndex, float rating);

#endif