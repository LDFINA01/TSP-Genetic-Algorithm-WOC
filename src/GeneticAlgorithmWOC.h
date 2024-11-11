//
// Created by Luis Finale on 10/13/2024.
//

#ifndef PROJECT4_GENETICALGORITHM_H
#define PROJECT4_GENETICALGORITHM_H

#include "vector"
#include "TravelPlanner.h"



// 2D-array to hold all top cities from all generations
extern vector<vector<int>> WOCPopulation;
extern  vector<vector<int>> previousCommonCities ;
extern  vector<vector<int>> nextCommonCities;
extern vector<int> WOCfinalRoute;


// Function to generate the starting population
vector<vector<int>> startingPopulation();

vector<vector<int>> generation(int maxCities);

// Function for Order Crossover
vector<int> orderCrossover(const vector<int>& parent1, const vector<int>& parent2);

// Function for Cycle Crossover
vector<int> cycleCrossover(const vector<int>& parent1, const vector<int>& parent2);

// Get the shorter child
vector<int> strongerChild (vector<int> &childA, vector<int> &childB);

// Function to mutate the population by swapping two cities in each route
vector<vector<int>> mutatePopulation(const vector<vector<int>> &population, double mutationRate);

std::vector<int> geneticAlgorithm(int generations, std::vector<double> &avgDistances);

// Function to select top parents based on fitness
std::vector<std::vector<int>> selectTopParents(const std::vector<std::pair<std::vector<int>, double>>& fitnessPopulation,double selectionPercentage,int elitismCount);

std::vector<std::vector<int>> tournamentSelection(const std::vector<std::pair<std::vector<int>, double>>& fitnessPopulation,int tournamentSize,double selectionPercentage);
// Function to apply elitism and carry over top individuals unchanged
void applyElitism(const std::vector<std::vector<int>> &selectedParents, int elitismCount, std::vector<std::vector<int>> &childPopulation);

void initilizeData ();

// Function to randomize the starting city
void randomizeStartingCity  (std::vector<int>& route);
#endif //PROJECT4_GENETICALGORITHM_H
