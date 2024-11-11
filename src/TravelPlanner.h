// TravelPlanner.h
//
// Created by LuisF on 10/12/2024.
//
#ifndef TRAVELPLANNER_H
#define TRAVELPLANNER_H

#include "iostream"
#include "fstream"
#include "sstream"
#include "vector"
#include "string"
#include "math.h"
#include "chrono"
#include "algorithm"

using namespace std;



// Declare external variables without initializing them
extern vector<vector<double>> cities;
extern vector<int> minPath;
extern double shortestPath;
extern vector<vector<double>> distance;
extern vector<vector<double>> distanceMatrix;

class Timer {
public:
    Timer();
    void start();
    void stop();
    double getElapsedMilliseconds() const;

private:
    chrono::time_point<chrono::high_resolution_clock> startTime;
    chrono::time_point<chrono::high_resolution_clock> endTime;
    bool running;
};

// Function to calculate the distance between two cities
double getDistance(int cityA, int cityB);
// Function to find the total distance of a route
double routeDistance(vector<int> route);

void precomputeDistances();

// Function to calculate the distance between a point and a line
double pointToLineDistanceCalculator(const vector<double>& city, const vector<double>city1, const vector<double>& city2);

// Function to get cities from the TSP file
vector<vector<double>> GetCitiesWithLocation();

vector<int> randomRoute (int maxCities);

// Function to compute average distance
double computeAverageDistance(const std::vector<std::pair<std::vector<int>, double>>& fitnessPopulation);

#endif // TRAVELPLANNER_H
