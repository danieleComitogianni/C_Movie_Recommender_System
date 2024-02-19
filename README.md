# C_Movie_Recommender_System
Small Project to work on and understand the key fundamental aspects of C

Movie Recommendation System

Introduction

This project is an implementation of a basic movie recommendation system in C. It aims to provide a hands-on experience with the C programming language, covering the fundamental concepts discussed in class. The application is designed to recommend movies to users based on their preferences and viewing history, similar to systems used by Netflix, Amazon Prime, Hulu, and YouTube. However, this implementation is simplified for educational purposes.

Features

User Registration: Allows new users to register with their first name. The application checks for existing users in a case-insensitive manner and prompts for a different name if the entered name already exists.
Display Movies: Shows a list of available movies stored in a database file in an indexed manner.
Rate a Movie: Enables registered users to rate movies. The application checks if the user exists and validates the entered movie index and rating before updating the database.
Get Movie Recommendations: Recommends movies to users based on a simple collaborative filtering algorithm, excluding movies the user has already rated and averaging ratings from other users.
Exit: Gracefully exits the application and releases all resources.
Implementation Specifications

The application displays a welcome message and a main menu.
It supports user registration, movie display, rating, and recommendations, as well as an exit option.
User and movie data are managed through text files (user_data.txt, movie_database.txt, and user_ratings.txt).
