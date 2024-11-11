//
// Created by Luis Finale on 10/13/2024.
//

#include "GeneticAlgorithmWOC.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "algorithm"
#include "random"
#include "cstdlib"
#include "ctime"

std::vector<std::vector<int>> WOCPopulation;
std::vector<std::vector<int>> previousCommonCities;
std::vector<std::vector<int>> nextCommonCities;
std::vector<int> WOCfinalRoute;

// Function to generate the starting population
std::vector<std::vector<int>> startingPopulation() {
    // THIS IS POPULATION SIZE NOT TOTAL CITIES
    int populationSize = 2 * cities.size();
    int maxPopulation = cities.size();
    std::vector<std::vector<int>> startingPopulation;


    for (int i = 0; i < populationSize; ++i) {
        // Use randomRoute() to generate a random route and add it to the population
        std::vector<int> route = randomRoute(maxPopulation);
        startingPopulation.push_back(route);
    }
    return startingPopulation;
}

vector<int> strongerChild(vector<int> &childA, vector<int> &childB) {
    double childASize = routeDistance(childA);
    double childBSize = routeDistance(childB);

    if (childASize <= childBSize) {
        if (childASize < shortestPath) {
            minPath = childA;
            shortestPath = childASize;
        }
        return childA;
    } else {
        if (childBSize < shortestPath) {
            minPath = childB;
            shortestPath = childBSize;
        }

        return childB;
    }
}

// Function for Order Crossover (OX)
vector<int> orderCrossover(const vector<int> &parent1, const vector<int> &parent2) {
    int size = parent1.size() - 1; // Exclude the last city (duplicate of first city)
    vector<int> child1(size, -1);
    vector<int> child2(size, -1);

    // Randomly select two crossover points
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, size - 1);

    int cutPoint1 = dist(gen);
    int cutPoint2 = dist(gen);

    while (cutPoint2 == cutPoint1) {
        cutPoint2 = dist(gen);
    }

    if (cutPoint1 > cutPoint2) {
        swap(cutPoint1, cutPoint2);
    }

    // Copy the subsequence from parent1 to child1
    for (int i = cutPoint1; i <= cutPoint2; ++i) {
        child1[i] = parent1[i];
    }

    // Copy the subsequence from parent2 to child2
    for (int i = cutPoint1; i <= cutPoint2; ++i) {
        child2[i] = parent2[i];
    }

    // Fill the remaining positions in child1 from parent2
    int currentPos = (cutPoint2 + 1) % size;
    for (int i = 0; i < size; ++i) {
        int idx = (cutPoint2 + 1 + i) % size;
        int city = parent2[idx];
        if (find(child1.begin(), child1.end(), city) == child1.end()) {
            child1[currentPos] = city;
            currentPos = (currentPos + 1) % size;
        }
    }

    // Similarly fill child2 from parent1
    currentPos = (cutPoint2 + 1) % size;
    for (int i = 0; i < size; ++i) {
        int idx = (cutPoint2 + 1 + i) % size;
        int city = parent1[idx];
        if (find(child2.begin(), child2.end(), city) == child2.end()) {
            child2[currentPos] = city;
            currentPos = (currentPos + 1) % size;
        }
    }

    // Add the starting city at the end to complete the cycle
    child1.push_back(child1[0]);
    child2.push_back(child2[0]);

    return strongerChild(child1, child2);
}

// Function for Cycle Crossover (CX)
vector<int> cycleCrossover(const vector<int> &parent1, const vector<int> &parent2) {
    int size = parent1.size() - 1; // Exclude the duplicate city at the end
    vector<int> child1(size, -1);
    vector<int> child2(size, -1);

    // Precompute city-to-index mappings
    vector<int> parent1CityIndex(size + 1); // Indexed by city number (1-based indexing)
    vector<int> parent2CityIndex(size + 1);

    for (int i = 0; i < size; ++i) {
        parent1CityIndex[parent1[i]] = i;
        parent2CityIndex[parent2[i]] = i;
    }

    // Cycle mapping for child1
    vector<bool> visited(size, false);
    int index = 0;

    while (!visited[index]) {
        child1[index] = parent1[index];
        visited[index] = true;
        int city = parent2[index];
        index = parent1CityIndex[city];
    }

    // Fill remaining positions from parent2
    for (int i = 0; i < size; ++i) {
        if (child1[i] == -1) {
            child1[i] = parent2[i];
        }
    }

    // Cycle mapping for child2
    fill(visited.begin(), visited.end(), false);
    index = 0;

    while (!visited[index]) {
        child2[index] = parent2[index];
        visited[index] = true;
        int city = parent1[index];
        index = parent2CityIndex[city];
    }

    // Fill remaining positions from parent1
    for (int i = 0; i < size; ++i) {
        if (child2[i] == -1) {
            child2[i] = parent1[i];
        }
    }

    // Add the starting city at the end to complete the cycle
    child1.push_back(child1[0]);
    child2.push_back(child2[0]);

    return strongerChild(child1, child2);
}


// Function to mutate the population by swapping two cities in each route
vector<vector<int>> mutatePopulation(const vector<vector<int>> &population, double mutationRate) {
    std::vector<std::vector<int>> mutatedPopulation = population; // Create a copy to mutate

    // Initialize random number generators
    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto &route: mutatedPopulation) {
        int size = route.size();
        if (size <= 3) continue; // Not enough cities to swap (start and end fixed)

        // Define the range for indices to swap (excluding first and last city)
        std::uniform_int_distribution<> dist(1, size - 2); // 0-based indexing

        int index1 = dist(gen);
        int index2 = dist(gen);

        // Ensure index2 is different from index1
        while (index2 == index1) {
            index2 = dist(gen);
        }

        // Swap the two cities
        std::swap(route[index1], route[index2]);
    }

    return mutatedPopulation;
}

// Function to select top parents based on fitness
std::vector<std::vector<int>> selectTopParents(
        const std::vector<std::pair<std::vector<int>, double>> &fitnessPopulation,
        double selectionPercentage,
        int elitismCount) {

    int selectionSize = static_cast<int>(fitnessPopulation.size() * selectionPercentage);
    selectionSize = std::max(selectionSize, elitismCount); // Ensure at least elitismCount parents

    std::vector<std::vector<int>> selectedParents;
    for (int i = 0; i < selectionSize; ++i) {
        selectedParents.push_back(fitnessPopulation[i].first);
    }

    return selectedParents;
}


std::vector<std::vector<int>>
tournamentSelection(const vector<std::pair<std::vector<int>, double>> &fitnessPopulation, int tournamentSize,
                    double selectionPercentage) {

    int populationSize = fitnessPopulation.size();
    int numberOfParents = static_cast<int>(populationSize * selectionPercentage);
    numberOfParents = std::max(numberOfParents, 2); // Ensure at least 2 parents are selected

    std::vector<std::vector<int>> selectedParents;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, populationSize - 1);

    for (int i = 0; i < numberOfParents; ++i) {
        double bestFitness = std::numeric_limits<double>::max();
        std::vector<int> bestIndividual;

        for (int j = 0; j < tournamentSize; ++j) {
            int idx = dist(gen);
            const auto &individual = fitnessPopulation[idx];
            if (individual.second < bestFitness) {
                bestFitness = individual.second;
                bestIndividual = individual.first;
            }
        }
        selectedParents.push_back(bestIndividual);
    }

    return selectedParents;
}

// Function to apply elitism and carry over top individuals unchanged
void applyElitism(const std::vector<std::vector<int>> &selectedParents, int elitismCount,
                  std::vector<std::vector<int>> &childPopulation) {
    for (int i = 0; i < elitismCount && i < selectedParents.size(); ++i) {
        childPopulation.push_back(selectedParents[i]);
        WOCPopulation.push_back(selectedParents[i]);
    }
}

void randomizeStartingCity(vector<int> &route) {
    int n = route.size();
    if (n <= 2) {
        return;
    }
    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, route.size() - 2); // Avoid first and last city

    // Get a random city index
    int randomCityIndex = dist(gen);
    int randomCity = route[randomCityIndex];
    int startingCity = route[0];

    // Swap the random city with the starting city
    route[randomCityIndex] = startingCity;
    route[0] = randomCity;
    route[route.size() - 1] = randomCity; // Update the last city to match the new starting city

}

// Genetic Algorithm Implementation with Elitist Selection
std::vector<int> geneticAlgorithm(int generations, std::vector<double> &avgDistances) {
    // Parameters
    const double selectionPercentage = 0.2; // Top 20% selected as parents
    srand(static_cast<unsigned >(time(0))); // random generator for the starting city randomizer


    // Initialize the population
    std::vector<std::vector<int>> parentPopulation = startingPopulation();
    std::vector<std::vector<int>> childPopulation;

    const int elitismCount = std::max(static_cast<int>(parentPopulation.size() * 0.02),
                                      1);             // Number of top individuals to carry over unchanged



    // For storing the best route found
    std::vector<int> bestRoute;
    double bestDistance = std::numeric_limits<double>::max();

    // Initialize random number generators
    std::random_device rd;
    std::mt19937 gen(rd());


    // Initialize mutation rate
    double mutationRate = 0.05;

    for (int genNum = 0; genNum < generations; ++genNum) {

        // Dynamic Mutation Rate
        if (genNum % 50 == 0 && genNum != 0) {
            mutationRate += 0.01; // Increase mutation rate by 1%
            if (mutationRate > 0.2) { // Cap mutation rate at 20%
                mutationRate = 0.2;
            }
        }

        // Evaluate fitness of current population
        std::vector<std::pair<std::vector<int>, double>> fitnessPopulation;
        for (const auto &route: parentPopulation) {
            double distance = routeDistance(route);
            fitnessPopulation.emplace_back(route, distance);

            // Track the best route
            if (distance < bestDistance) {
                bestDistance = distance;
                bestRoute = route;
            }
        }

        // Sort the population based on fitness (ascending order of distance)
        std::sort(fitnessPopulation.begin(), fitnessPopulation.end(),
                  [](const std::pair<std::vector<int>, double> &a,
                     const std::pair<std::vector<int>, double> &b) -> bool {
                      return a.second < b.second;
                  });

        // Finding the AVG distance
        double avgDistance = computeAverageDistance(fitnessPopulation);
        avgDistances.push_back(avgDistance);

        // Selecting top parents
        std::vector<std::vector<int>> selectedParents = selectTopParents(fitnessPopulation, selectionPercentage,
                                                                         elitismCount);

        childPopulation.clear();
        applyElitism(selectedParents, elitismCount, childPopulation);



        // Generate children until reaching the desired population size
        while (childPopulation.size() < parentPopulation.size()) {
            // Select two random parents from the selectedParents pool
            std::uniform_int_distribution<> parentDist(0, selectedParents.size() - 1);
            int parent1Idx = parentDist(gen);
            int parent2Idx = parentDist(gen);

            // Ensure parents are different
            while (parent2Idx == parent1Idx) {
                parent2Idx = parentDist(gen);
            }

            const std::vector<int> &parent1 = selectedParents[parent1Idx];
            const std::vector<int> &parent2 = selectedParents[parent2Idx];
            std::vector<int> child;



            // Use Cycle Crossover
            child = cycleCrossover(parent1, parent2);


            randomizeStartingCity(child);


            // Add child to childPopulation
            childPopulation.push_back(child);
        }


        childPopulation = mutatePopulation(childPopulation, mutationRate);


        // Update parentPopulation for the next generation
        parentPopulation = childPopulation;
    }

//    for(const auto & route : WOCPopulation) {
//        for(int i : route) {
//            cout << i << ", ";
//        }
//        cout << endl;
//    }

    return bestRoute;
}


void initilizeData() {
    int n = WOCPopulation[0].size();
    previousCommonCities.resize(n, std::vector<int>(n));
    nextCommonCities.resize(n, std::vector<int>(n));
    vector<vector<int>> WOCRoutes;


    for (const auto &route: WOCPopulation) {
        int routeSize = route.size();
        for (int i = 1; i < routeSize - 1; i++) {
            int previousCity = route[i - 1];
            int nextCity = route[i + 1];
            int currentCity = route[i];

            previousCommonCities[currentCity - 1][previousCity - 1]++;
            nextCommonCities[currentCity - 1][nextCity - 1]++;


        }
    }

    int startingCity = 0;
    for (int i = 1; i < n; i++) {
        startingCity = i;
        // Initialize the final route
        WOCfinalRoute.reserve(n + 1); // Reserve space for n cities plus the return to start
        WOCfinalRoute.push_back(startingCity);

        // Set to keep track of visited cities
        std::unordered_set<int> visitedCities;
        visitedCities.insert(startingCity);

        // Build the route
        int currentCity = startingCity;
        for (int i = 0; i < n - 1; i++) {
            int nextCity = -1;
            int maxFrequency = -1;

            // Iterate over possible next cities
            for (int j = 0; j < n; j++) {
                int potentialNextCity = j + 1;
                if (visitedCities.find(potentialNextCity) == visitedCities.end()) {
                    int frequency = nextCommonCities[currentCity - 1][j];
                    if (frequency > maxFrequency) {
                        maxFrequency = frequency;
                        nextCity = potentialNextCity;
                    }
                }
            }

            // If no unvisited next city is found (should not happen), select any unvisited city
            if (nextCity == -1) {
                cout << "NO UNVISITED CITY FOUND " << endl;
                for (int j = 1; j <= n; j++) {
                    if (visitedCities.find(j) == visitedCities.end()) {
                        nextCity = j;
                        break;
                    }
                }
            }

            // Add the next city to the route
            WOCfinalRoute.push_back(nextCity);
            visitedCities.insert(nextCity);
            currentCity = nextCity;
        }
        WOCfinalRoute.erase(WOCfinalRoute.begin() + WOCfinalRoute.size() - 1);

        // Return to the starting city to complete the cycle
        WOCfinalRoute.push_back(startingCity);
        WOCRoutes.push_back(WOCfinalRoute);
        WOCfinalRoute.clear();
    }
    int bestDistance = INT16_MAX;
    for(auto & route : WOCRoutes) {
        int currentDistance = routeDistance(route);
        if (currentDistance < bestDistance) {
            bestDistance = currentDistance;
            WOCfinalRoute = route;
        }
    }

//    R

    std::cout << "Route distance of WOC route: " << routeDistance(WOCfinalRoute) << std::endl;

    std::cout << endl;

//    for(const auto & route : previousCommonCities) {
//        for(int i : route)
//            cout << i << ", ";
//        }
//        cout << endl;
//    }
//    for(const auto & route : nextCommonCities) {
//        for(int i : route) {
//            cout << i << ", ";
//        }
//        cout << endl;
//    }

}




/*here is where the WOC part of the function or in other words
 * the wisdom of the crowds sections is going to be, here
 * NOTE: if im going to to a double value instead of a int i can do the int/WOCPopulation.size() since that will be the avg amount that the cites are
 *
 * 1. input go throught the WOCPopulation and wrtie all of the cities that come previous and after
 *  NOTE: here is the inteded strucuture of the 2D array, also not that we're doing this with previousCommonCities in mind
 *
 *      row = city, so if we are currently on city 'i' we would go to WOCPopulation[i] to find the most common ciites before
 *      colum = that city in the WOCPopulation, so example, we're iterating though 'route' in WOCPopulation and we're
 *          currently on city 45, i = 44, route[44] = 45 or WOCPopulation[route][44] = 45, and we know that i - 1 = 34
 *          meaning that we have city 34 before 44 we would add this to the previousCommonCities as
 *          previousCommonCities[i or in this case 44 since the city is stored at index city # - 1][33 also since the indx is store at city # - 1]++;
 *
 *  ex:
 *
 *      NOTE: also find another data structure to store and find the most common starting/ending city is
 *      for(const auto& route : WOCPopulation ) {
 *          for(int i = 1; i < route.size() - 1; ) {
 *              //Finding previous city and storing it
*
 *
 *
 *
 *
 * */





