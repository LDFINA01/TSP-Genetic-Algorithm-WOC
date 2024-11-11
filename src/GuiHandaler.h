//
// Created by LuisF on 10/17/2024.
//

#ifndef PROJECT4_GUIHANDALER_H
#define PROJECT4_GUIHANDALER_H


#include "vector"

// Function to display plots
void displayPlot(
        const std::vector<double>& avgDistances,
        const std::vector<std::vector<double>>& cities,
        const std::vector<int>& bestRoute,
        const std::vector<int>& WOCRoute
);


#endif //PROJECT4_GUIHANDALER_H
