#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "movie.h"
#include <stdlib.h>

typedef struct {
    int count; 
    float sum; 
} MovieRatingInfo;



void displayMovies(){
    FILE* loadFile = fopen("../data/movie_database.txt","r");
    if (loadFile==NULL){
        perror("Failed to open User Data");
        return;
    }

    char line[200];
    printf("\n***** Movie Database *****\n");
    int count=1;
    while(fgets(line, sizeof(line),loadFile) != NULL){
        printf("%d. %s", count,line);
        count++;
    }
    fclose(loadFile);   
}

int indexValidation(int index){
    if(index<1||index>10){
        printf("Input is not valid. Choose between movies 1-10 please.\n");
        return 1;
    } else {
        return 0;
    }
}


float** loadRatingsMatrix(int numRows) {
    int numCols=10;
    FILE* file = fopen("../data/user_ratings.txt", "r");
    if (!file) {
        perror("Failed to open ratings file");
        return NULL;
    }


    // Allocate memory for the ratings matrix
    float** ratingsMatrix = (float**)malloc(numRows * sizeof(float*));
    if (!ratingsMatrix) {
        perror("Failed to allocate memory for ratings matrix");
        fclose(file);
        return NULL;
    }

    // Skip the dimensions line since we already know numRows and numCols
    fscanf(file, "%*d %*d"); // Correctly skip the dimensions line
    
    int c;
    while ((c = fgetc(file)) != '\n' && c != EOF); // Consume until end of line or EOF


    for (int i = 0; i < numRows; i++) {
        ratingsMatrix[i] = (float*)malloc(numCols * sizeof(float));
        if (!ratingsMatrix[i]) {
            perror("Failed to allocate memory for row");
            // Clean up previously allocated memory
            for (int j = 0; j < i; j++) {
                free(ratingsMatrix[j]);
            }
            free(ratingsMatrix);
            fclose(file);
            return NULL;
        }
        for (int j = 0; j < numCols; j++) {
            if (fscanf(file, "%f", &ratingsMatrix[i][j]) != 1) {
                fprintf(stderr, "Error reading rating at row %d, col %d\n", i + 1, j + 1);
            }
        }
    }

    fclose(file);
    return ratingsMatrix;
}

char** store_movie_in_array(){
    FILE* loadFile = fopen("../data/movie_database.txt","r");
    if (loadFile==NULL){
        perror("Failed to open Movie Data");
        return NULL;
    }

    char** movieTitlesAndGenres = (char**)malloc(10 * sizeof(char*));
    char line[250];
    int movieIndex = 0;

    while (fgets(line, sizeof(line), loadFile) && movieIndex < 10) {
        line[strcspn(line, "\n")] = '\0'; // Remove the newline character at the end

        // Replace underscores with spaces for better readability before isolating the title and genre
        for (char* p = line; *p; ++p) {
            if (*p == '_') *p = ' ';
        }

        // Find the last space in the string, which is right before the rating number
        char* lastSpacePos = strrchr(line, ' ');
        if (lastSpacePos != NULL) {
            *lastSpacePos = '\0'; // Terminate the string before the rating number
        }

        // Allocate memory for the movie title and genre string and copy it
        movieTitlesAndGenres[movieIndex] = (char*)malloc((strlen(line) + 1) * sizeof(char));
        strcpy(movieTitlesAndGenres[movieIndex], line);

        movieIndex++;
    }

    fclose(loadFile);
    return movieTitlesAndGenres;
}


void calculateRecommendations(float** ratingsMatrix,int numRows,int userIndex, char** movieTitles) {
    int numCols=10;
    userIndex=userIndex-1;
    // Allocate an array to hold rating info for each movie
    MovieRatingInfo* movieRatingsInfo = (MovieRatingInfo*)calloc(numCols, sizeof(MovieRatingInfo));
    if (movieRatingsInfo == NULL) {
        perror("Memory allocation failed");
        return;
    }

    // Initialize array to track if the movie has been watched by the user
    int* watched = (int*)calloc(numCols, sizeof(int));
    if (watched == NULL) {
        perror("Memory allocation failed");
        free(movieRatingsInfo); // Free previously allocated memory
        return;
    }

    // Mark watched movies by the user
    for (int j = 0; j < numCols; ++j) {
        if (ratingsMatrix[userIndex][j] > 0.0) {
            watched[j] = 1;
            printf("Movie %d watched by user %d: %d\n", j + 1, userIndex + 1, watched[j]);
        }
    }

    // Aggregate ratings for unwatched movies
    for (int j = 0; j < numCols; ++j) {
        if (!watched[j]) { // If movie is not watched by the user
            for (int i = 0; i < numRows; ++i) {
                if (i != userIndex && ratingsMatrix[i][j] > 0.0) {
                    movieRatingsInfo[j].count++;
                    movieRatingsInfo[j].sum += ratingsMatrix[i][j];
                }
            }
        }
    }

    // Display recommendations
    printf("***** Recommended Movies *****\n");
    int count=1;
    for (int j = 0; j < numCols; ++j) {
        if (!watched[j] && movieRatingsInfo[j].count > 0) {
            float averageRating = movieRatingsInfo[j].sum / movieRatingsInfo[j].count;
            printf("%d. %s - Predicted Rating: %.2f\n", count++, movieTitles[j], averageRating);
        }
    }

    // Cleanup
    free(movieRatingsInfo);
    free(watched);
}
