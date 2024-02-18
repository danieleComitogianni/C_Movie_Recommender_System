#include "user.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


static int user_count=7;
int totalMovies;

void loadUsers(User users[], int* userCount){
    char data[100][1000];
    FILE* loadFile = fopen("../data/user_data.txt","r");
    if (loadFile==NULL){
        perror("Failed to open User Data");
        return;
    }

    int line=0;
    while (!feof(loadFile)&& !ferror(loadFile)) {
       if (fgets(data[line],1000,loadFile)!=NULL){
        line++;
       }
    }

    fclose(loadFile);

    for(int i=0;i<line;i++){
        printf("%s", data[i]);
    }
    
}

int checkUser(char* name){
    int num;
    FILE* loadFile = fopen("../data/user_data.txt","r");
    if (loadFile==NULL){
        perror("Failed to open User Data");
        return -1;
    }
    char buf[100];
    int nameExists=0;
    while(fgets(buf, sizeof(buf), loadFile)!=NULL){
        
        buf[strcspn(buf, "\n")]='\0';
        
        char* space = strchr(buf, ' ');
        if(space != NULL){
            *space = '\0';
        }

        #ifdef _WIN32
        if(_stricmp(buf, name) == 0){
        #else
        if(strcasecmp(buf, name) == 0){
        #endif
            nameExists=1;
            break;
        }
    }

    fclose(loadFile);
    return nameExists;
}

float** readCurrentRatings(FILE* file, int* numRows, int* numCols, float** ratings) {
    fscanf(file, "%d %d", numRows, numCols);

    // Allocate memory for the ratings matrix
    ratings = (float**)malloc(*numRows * sizeof(float*));
    for (int i = 0; i < *numRows; i++) {
        ratings[i] = (float*)malloc(*numCols * sizeof(float));
        for (int j = 0; j < *numCols; j++) {
            fscanf(file, "%f", &ratings[i][j]);
        }
    }

    return ratings;
}

// Function to update the user_ratings.txt file
void addUserAndUpdateRatingsFile(const char* name, int userCount) {
    // Open user_ratings.txt for reading
    FILE* file = fopen("../data/user_ratings.txt", "r");
    if (!file) {
        perror("Failed to open ratings file for reading");
        return;
    }

    int numRows, numCols;
    float** ratings = NULL;
    ratings = readCurrentRatings(file, &numRows, &numCols, ratings);
    fclose(file); // Close the file after reading

    // Increment the number of users (rows)
    numRows += 1;

    // Open user_ratings.txt for writing to update the entire file
    file = fopen("../data/user_ratings.txt", "w");
    if (!file) {
        perror("Failed to open ratings file for writing");
        // Free allocated memory for ratings
        for (int i = 0; i < numRows - 1; i++) {
            free(ratings[i]);
        }
        free(ratings);
        return;
    }

    // Write the updated header
    fprintf(file, "%d %d\n", numRows, numCols);

    // Write back all the old ratings
    for (int i = 0; i < numRows - 1; i++) {
        for (int j = 0; j < numCols; j++) {
            fprintf(file, "%.1f ", ratings[i][j]);
        }
        fprintf(file, "\n");
    }

    // Append the new user's ratings (all 0.0s)
    for (int i = 0; i < numCols; i++) {
        fprintf(file, "0.0 ");
    }
    fprintf(file, "\n");

    fclose(file);

    // Free allocated memory for ratings
    for (int i = 0; i < numRows - 1; i++) {
        free(ratings[i]);
    }
    free(ratings);

    // Append the new user in user_data.txt
    file = fopen("../data/user_data.txt", "a");
    if (!file) {
        perror("Failed to open user data file for appending");
        return;
    }
    fprintf(file, "\n%s %d", name, userCount++);
    printf("\nUser %s is successfully registered.\n", name);
    fclose(file);
}

/**void addUser(char* name){
    FILE* loadFile = fopen("../data/user_data.txt","a");
    if (loadFile==NULL){
        perror("Failed to open User Data");
        return;
    }

    fprintf(loadFile, "%s %d\n", name, ++user_count);
    fclose(loadFile);
    printf("\nUser %s is successfully registered.\n", name);

    FILE* ratingsFile = fopen("../data/user_ratings.txt", "r");
    if (!ratingsFile){
        perror("Failed to open ratings file");
        return;
    }

    int numberOfRows, numberOfCol;
    fscanf(ratingsFile, "%d %d", &numberOfRows, &numberOfCol);
    totalMovies=numberOfCol;

    fseek(ratingsFile, 0, SEEK_END);
    for(int i=0;i<totalMovies;i++){
        fprintf(ratingsFile, "0.0 ");
    }
    fprintf(ratingsFile, "\n");

    rewind(ratingsFile);
    fprintf(ratingsFile, "%d %d", numberOfRows + 1, numberOfCol);

    fclose(ratingsFile);
}
**/
int getUserIndex(char* name) { //Basically the same method as the check user but I return the index number
    FILE* loadFile = fopen("../data/user_data.txt", "r");
    if (!loadFile) {
        perror("Failed to open User Data");
        return -1;
    }
    char buf[100];
    int index = 0, found = 0;
    while (fgets(buf, sizeof(buf), loadFile) != NULL) {
        char* space = strchr(buf, ' ');
        if (space != NULL) {
            *space = '\0';  
            if (strcmp(buf, name) == 0) {
                found = 1;
                sscanf(space + 1, "%d", &index);  
                break;
            }
        }
    }
    fclose(loadFile);
    return found ? index : -1;
}

void userRating_ByIndex(int userIndex, int movieIndex, float rating){
    int numRows, numCols;
    
    FILE* loadFile = fopen("../data/user_ratings.txt","r");
    if (loadFile==NULL){
        perror("Failed to open User Data");
        return;
    }

    if (fscanf(loadFile, "%d %d", &numRows, &numCols) != 2) {
        printf("Error reading matrix dimensions.\n");
        fclose(loadFile);
        return;
    }
    
    if (userIndex < 1 || userIndex > numRows || movieIndex < 1 || movieIndex > numCols) {
        printf("Invalid user or movie index.\n");
        fclose(loadFile);
        return;
    }

    float** ratings = malloc(numRows * sizeof(float*));
    for (int i = 0; i < numRows; i++) {
        ratings[i] = malloc(numCols * sizeof(float));
        for (int j = 0; j < numCols; j++) {
            if (fscanf(loadFile, "%f", &ratings[i][j]) != 1) {
                // Handle potential error in reading ratings
            }
        }
    }
    fclose(loadFile);

    // Update the specific rating
    ratings[userIndex - 1][movieIndex - 1] = rating;

    // Write the updated matrix back to the file
    loadFile = fopen("../data/user_ratings.txt", "w");
    if (!loadFile) {
        perror("Failed to open ratings file for writing");
        // Free allocated memory before returning
        for (int i = 0; i < numRows; i++) {
            free(ratings[i]);
        }
        free(ratings);
        return;
    }

    fprintf(loadFile, "%d %d\n", numRows, numCols);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            fprintf(loadFile, "%.1f ", ratings[i][j]);
        }
        fprintf(loadFile, "\n");
    }

    // Cleanup
    for (int i = 0; i < numRows; i++) {
        free(ratings[i]);
    }
    free(ratings);
    fclose(loadFile);
    printf("Rating recorded successfully.\n");
}
