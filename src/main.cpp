//
//
// Created by LuisF on 10/12/2024.
//
// main.cpp
#include <iostream>
#include <vector>
#include "TravelPlanner.h"
#include "GeneticAlgorithmWOC.h"
#include "GuiHandaler.h"


using namespace std;


int main() {

    cities = GetCitiesWithLocation();
    precomputeDistances();
    // A print statement to confirm that it's correct
    for (const auto &coord: cities) {
        cout << "City: " << coord[0] << ", x-coordinate: " << coord[1] << ", y-coordinate: " << coord[2] << endl;
    }


    int generations = 0;
    std::cout << "How many generations:";
    std::cin >> generations;
    std::vector<double> avgDistances;
    std::vector<int> localBestRoute;
    vector<int> bestRoute;
    int bestRouteDistance = INT16_MAX;
    vector<int> bestWOCRoute;
    double bestWOCRouteDistance = INT16_MAX;


    // Vector to store average distances

    cout << "WORKING..." << std::endl;
    cout << endl;

    Timer timer;
    timer.start();

    // Run the genetic algorithm
    localBestRoute = geneticAlgorithm(generations, avgDistances);

    timer.stop();
    double elapsedTime = timer.getElapsedMilliseconds();
    std::cout << "Execution time: " << elapsedTime << " milliseconds" << std::endl;

    // Calculate the distance of the best route
    double bestDistance = routeDistance(localBestRoute);
    std::cout << "Best route distance by GA: " << bestDistance << std::endl;

    // Initialize WOC data
    initilizeData();

    // Calculate the distance of the WOC route
    double wocDistance = routeDistance(WOCfinalRoute);

    // Update bestRoute if necessary
    if (bestDistance < bestRouteDistance) {
        bestRouteDistance = bestDistance;
        bestRoute = localBestRoute;
    }

    // Update bestWOCRoute if necessary
    if (wocDistance < bestWOCRouteDistance) {
        bestWOCRouteDistance = wocDistance;
        bestWOCRoute = WOCfinalRoute;
    }




    //



//     Gui Display for the table

    displayPlot(avgDistances, cities, bestRoute, bestWOCRoute);


    return 0;
}