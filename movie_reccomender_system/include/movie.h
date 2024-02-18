#ifndef MOVIE_H
#define MOVIE_H

typedef struct {
    int movieID;
    char title[100];
} Movie;

void displayMovies();

int indexValidation(int index);

float** loadRatingsMatrix(int numRows);

char** store_movie_in_array();

void calculateRecommendations(float** ratingsMatrix,int numRows,int userIndex, char** movieTitles);


#endif