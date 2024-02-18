#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "movie.h"
#include "user.h"


void displayMenu(){
    printf("\n***** Movie Recommendation System ****\n");
    printf("1. Register User\n");
    printf("2. Display Movies\n");
    printf("3. Rate a Movie\n");
    printf("4. Get Movie Recommendations\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

int main(){
    User users[100];
    int userCount =0;
    int choice;
    int userReturn;
    char userName[100];
    int numOfUsers=7;

    while(1){
        displayMenu();
        scanf("%d",&choice);

        switch(choice){
            case 1: 
                while(1){
                printf("\nEnter username for registration: \n");
                char registerName[100];
                scanf("%99s",registerName);
                userReturn = checkUser(registerName);
                    if (userReturn == 0){
                        addUserAndUpdateRatingsFile(registerName, ++numOfUsers);
                        break;
                    } else {
                        printf("User already exists. Please choose a different name.");
                        
                    }
                }
                break;
            case 2:
                displayMovies();
                break;
            case 3:
                
                printf("\nPlease enter your username: ");
                scanf("%99s", userName);
                if(!checkUser(userName)){
                    printf("User not found. Please register first.\n");
                    break;
                } else {
                    displayMovies();
                    int movieIndex;
                    int rating;
                    int validate = 1;
                    while(validate){
                        printf("Enter the number of the movie you want to rate: ");
                        scanf("%d", &movieIndex);
                        validate = indexValidation(movieIndex);
                    }
                    int validRating = 0;
                    while (!validRating) {
                        printf("Enter your rating (1-5): ");
                        scanf("%d", &rating);
                        if (rating >= 1 && rating <= 5) {
                            int userIndex = getUserIndex(userName);
                            userRating_ByIndex(userIndex, movieIndex, rating);
                            validRating = 1;
                            break;
                        } else {
                            printf("Invalid input. Please enter a rating between 1 and 5.\n");
                        }
                    }
                }
                break;
            case 4:
                printf("\nPlease enter your username: ");
                scanf("%99s", userName);
                if(!checkUser(userName)){
                    printf("User not found. Please register first.\n");
                    break;
                } else {
                    float** ratingsMatrix = loadRatingsMatrix(numOfUsers);
                    if (!ratingsMatrix) {
                        printf("Failed to load ratings matrix.\n");
                        return 1; // Exit if matrix loading fails
                    }
                    for (int i=0;i<numOfUsers;i++){
                        for(int j=0;j<10;j++){
                            printf("%.1f", ratingsMatrix[i][j]);
                        }
                        printf("\n");
                    }
                    int userID = getUserIndex(userName);
                    if (userID == -1) {
                        printf("User not found.\n");
                    } else {
                        char** movieArray = store_movie_in_array();
                        calculateRecommendations(ratingsMatrix, numOfUsers, userID, movieArray);
                    }

                    for (int i = 0; i < numOfUsers; i++) {
                        free(ratingsMatrix[i]);
                    }
                    free(ratingsMatrix);
                }
                
                break;
            case 0:
                printf("Bye\n");
                return 0;
            default:
                printf("Invalid choice. Please try again!\n");
        }


    }





    return 0;
}