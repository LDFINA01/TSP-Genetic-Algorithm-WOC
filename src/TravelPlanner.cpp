// TravelPlanner.cpp
//
// Created by LuisF on 10/12/2024.
//
#include "TravelPlanner.h"
#include "algorithm"
#include "random"
#include "iostream"

// Define the external variables
vector<vector<double>> cities;
vector<int> minPath;
double shortestPath = INT_MAX;
vector<vector<double>> distanceMatrix;


// Timer implementation
Timer::Timer() : running(false) {}

void Timer::start() {
    running = true;
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    running = false;
}

double Timer::getElapsedMilliseconds() const {
    std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;

    if (running) {
        endTimePoint = std::chrono::high_resolution_clock::now();
    } else {
        endTimePoint = endTime;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(endTimePoint - startTime).count();
}

// Distance calculator implementation
double getDistance(int cityA, int cityB) {
    //to get the indx and not the actual city number
    cityA--;
    cityB--;
    double x1 = cities[cityA][1];
    double y1 = cities[cityA][2];
    double x2 = cities[cityB][1];
    double y2 = cities[cityB][2];
    double xDistance = (x2 - x1) * (x2 - x1);
    double yDistance = (y2 - y1) * (y2 - y1);

    return sqrt(xDistance + yDistance);
}

double routeDistance (vector<int> route) {
    double distance = 0.0;
    for(int i = 0; i < route.size() - 1; i++) {
        distance += distanceMatrix[route[i] - 1][route[i + 1] - 1];
    }
    return distance;
}



void precomputeDistances() {
    int n = cities.size();
    distanceMatrix.resize(n , std::vector<double>(n , 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = getDistance(i + 1, j + 1); // Adjusting for 1-based indexing
            distanceMatrix[i][j] = dist;
            distanceMatrix[j][i] = dist;
        }
    }
}




// Function to calculate the distance between a point and a line
double pointToLineDistanceCalculator(const vector<double>& city, const vector<double>city1, const vector<double>& city2) {
    double x0 = city[1], y0 = city[2]; // Coordinates of the point (city)
    double x1 = city1[1], y1 = city1[2]; // Coordinates of the first point on the line (city1)
    double x2 = city2[1], y2 = city2[2]; // Coordinates of the second point on the line (city2)

    // Compute the distance from city to the line formed by city1 and city2
    double numerator = abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1);
    double denominator = sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
    return numerator / denominator;
}

// GetCitiesWithLocation implementation
vector<vector<double>> GetCitiesWithLocation() {
    // Prompt for file name
    string fileName;
    cout << "Choose a file and find its optimal route (write entire file name): ";
    cin >> fileName;
    cout << "You chose the file " << fileName << endl;

    // Open the file
    ifstream file(fileName);

    // Check if file was opened correctly
    if (!file.is_open()) {
        cout << "Could not open file " << fileName << endl;
        return {};
    }

    // 2-D vector to store city data
    vector<vector<double>> cities;

    // Read file line by line
    string line;
    bool readingCoordinates = false;

    while (getline(file, line)) {
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            readingCoordinates = true;
            continue;
        }

        if (readingCoordinates) {
            istringstream iss(line);
            double city, xCoord, yCoord;
            if (iss >> city >> xCoord >> yCoord) {
                cities.push_back({city, xCoord, yCoord});
            } else {
                cerr << "Error parsing line: " << line << endl;
            }
        }
    }

    if (cities.empty()) {
        cerr << "No cities were read from the file." << endl;
    }


    return cities;
}


// Function to generate a random list of cities for a Hamiltonian cycle
vector<int> randomRoute(int maxCities) {
    // Seed the random number generator with a random device for better randomness
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, maxCities);

    // Generate a random starting city
    int startingCity = dist(gen);

    // Create a list of all possible cities
    vector<int> allCities;
    for (int i = 1; i <= maxCities; i++) {
        allCities.push_back(i);
    }

    // Remove the starting city from the list since it is already chosen
    allCities.erase(std::remove(allCities.begin(), allCities.end(), startingCity), allCities.end());

    // Shuffle the remaining cities to create a random order
    shuffle(allCities.begin(), allCities.end(), gen);

    // Create the route and add the starting city first
    vector<int> route;
    route.push_back(startingCity);

    // Add all remaining cities in random order
    for (const auto& city : allCities) {
        route.push_back(city);
    }

    // Add the starting city again at the end to complete the Hamiltonian cycle
    route.push_back(startingCity);

    return route;
}

// Function to compute average distance
double computeAverageDistance(const std::vector<std::pair<std::vector<int>, double>>& fitnessPopulation) {
    double totalDistance = 0.0;
    for (const auto& individual : fitnessPopulation) {
        totalDistance += individual.second;
    }
    return totalDistance / fitnessPopulation.size();
}

