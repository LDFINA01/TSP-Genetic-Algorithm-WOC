//
// Created by LuisF on 10/17/2024.
//

#include "GeneticAlgorithmWOC.h"
#include "TravelPlanner.h"
#include "GuiHandaler.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot/implot.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>

// Function to display plots
void displayPlot(
        const std::vector<double>& avgDistances,
        const std::vector<std::vector<double>>& cities,
        const std::vector<int>& bestRoute,
        const std::vector<int>& WOCRoute
) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        return;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Genetic Algorithm Visualization", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // Initialize ImGui and ImPlot
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Prepare data for plotting average distances
    std::vector<double> x_data(avgDistances.size());
    for (size_t i = 0; i < avgDistances.size(); ++i) {
        x_data[i] = static_cast<double>(i + 1);
    }

    // Prepare data for plotting cities and best route
    // Extract x and y coordinates of cities
    std::vector<double> city_x;
    std::vector<double> city_y;
    for (const auto& coord : cities) {
        // coord[0] is the city index, coord[1] is x, coord[2] is y
        city_x.push_back(coord[1]);
        city_y.push_back(coord[2]);
    }

    // Extract route coordinates based on bestRoute
    std::vector<double> route_x;
    std::vector<double> route_y;
    for (const auto& city_idx : bestRoute) {
        // Adjusting index if cities are 1-based
        int zero_based_idx = city_idx - 1;
        if (zero_based_idx < 0 || zero_based_idx >= static_cast<int>(cities.size())) {
            std::cerr << "Invalid city index in bestRoute: " << city_idx << std::endl;
            continue;
        }
        route_x.push_back(cities[zero_based_idx][1]);
        route_y.push_back(cities[zero_based_idx][2]);
    }

    // Validate bestRoute
    if (bestRoute.size() > 1) { // Ensure there's more than one city
        std::vector<int> uniqueCities(bestRoute.begin(), bestRoute.end() - 1); // Exclude the last city (duplicate)
        std::sort(uniqueCities.begin(), uniqueCities.end());
        uniqueCities.erase(std::unique(uniqueCities.begin(), uniqueCities.end()), uniqueCities.end());

        if (uniqueCities.size() != cities.size()) {
            std::cerr << "Best route does not include all cities exactly once.\n";
        }
    }
    double bestDistance = routeDistance(bestRoute);

    // Prepare data for plotting WOCRoute
    std::vector<double> woc_route_x;
    std::vector<double> woc_route_y;
    for (const auto& city_idx : WOCRoute) {
        // Adjusting index if cities are 1-based
        int zero_based_idx = city_idx - 1;
        if (zero_based_idx < 0 || zero_based_idx >= static_cast<int>(cities.size())) {
            std::cerr << "Invalid city index in wocRoute: " << city_idx << std::endl;
            continue;
        }
        woc_route_x.push_back(cities[zero_based_idx][1]);
        woc_route_y.push_back(cities[zero_based_idx][2]);
    }
    double WOCDistance = routeDistance(WOCRoute);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll events
        glfwPollEvents();

        // Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create ImGui window with tabs
        ImGui::Begin("Genetic Algorithm Visualization");

        if (ImGui::BeginTabBar("TabBar")) {
            //-----------------------------------------------------------------------------
            // TAB 1
            //-----------------------------------------------------------------------------
            if (ImGui::BeginTabItem("Average Distance")) {
                if (ImPlot::BeginPlot("Average Distance per Generation", ImVec2(-1, -1), 0)) {
                    // Compute min and max of x_data and avgDistances
                    double min_x = *std::min_element(x_data.begin(), x_data.end());
                    double max_x = *std::max_element(x_data.begin(), x_data.end());
                    double min_y = *std::min_element(avgDistances.begin(), avgDistances.end());
                    double max_y = *std::max_element(avgDistances.begin(), avgDistances.end());

                    // Compute x-axis limits with 5% padding
                    double x_range = max_x - min_x;
                    if (x_range == 0.0) {
                        x_range = 1.0; // Prevent division by zero
                    }
                    double x_min_limit = min_x - x_range * 0.05;
                    double x_max_limit = max_x + x_range * 0.05;

                    // Compute y-axis limits: from 0 to 5% larger than max_y
                    double y_max_limit = max_y * 1.05;

                    // Set up axis labels and limits
                    ImPlot::SetupAxes("Generation", "Average Distance", 0, 0);
                    ImPlot::SetupAxisLimits(ImAxis_X1, x_min_limit, x_max_limit, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, y_max_limit, ImGuiCond_Always);

                    // Add legend
                    ImPlot::SetupLegend(ImPlotLocation_NorthEast);

                    // Plot the data
                    ImPlot::PlotLine("Average Distance", x_data.data(), avgDistances.data(), avgDistances.size());

                    ImPlot::EndPlot();
                }
                ImGui::EndTabItem();
            }
            //-----------------------------------------------------------------------------
            // TAB 2
            //-----------------------------------------------------------------------------
            if (ImGui::BeginTabItem("Best Route")) {
                ImGui::Text("Best Route Distance: %.2f", bestDistance);
                if (ImPlot::BeginPlot("Cities and Best Route", ImVec2(-1, -1), 0)) {
                    // Compute dynamic axis limits
                    double min_x = *std::min_element(city_x.begin(), city_x.end());
                    double max_x = *std::max_element(city_x.begin(), city_x.end());
                    double min_y = *std::min_element(city_y.begin(), city_y.end());
                    double max_y = *std::max_element(city_y.begin(), city_y.end());

                    // Add padding
                    double padding_x = (max_x - min_x) * 0.05; // 5% padding
                    double padding_y = (max_y - min_y) * 0.05; // 5% padding

                    // Set up axes with padding
                    ImPlot::SetupAxes("X Coordinate", "Y Coordinate", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                    ImPlot::SetupAxisLimits(ImAxis_X1, min_x - padding_x, max_x + padding_x, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, min_y - padding_y, max_y + padding_y, ImGuiCond_Always);

                    // Add legend
                    ImPlot::SetupLegend(ImPlotLocation_NorthEast);

                    // Plot cities as scatter points

                    ImPlot::PlotScatter("Cities", city_x.data(), city_y.data(), city_x.size(), 3.0f); // Marker size 3.0f


                    // Plot best route as a connected line
                    ImPlot::PlotLine("Best Route", route_x.data(), route_y.data(), route_x.size());


                    ImPlot::EndPlot();
                }
                ImGui::EndTabItem();
            }
            //-----------------------------------------------------------------------------
            // TAB 3 - WOC Route
            //-----------------------------------------------------------------------------
            if (ImGui::BeginTabItem("WOC Route")) {
                ImGui::Text("WOC Route Distance: %.2f", WOCDistance);
                if (ImPlot::BeginPlot("Cities and WOC Route", ImVec2(-1, -1), 0)) {
                    // Compute dynamic axis limits
                    double min_x = *std::min_element(city_x.begin(), city_x.end());
                    double max_x = *std::max_element(city_x.begin(), city_x.end());
                    double min_y = *std::min_element(city_y.begin(), city_y.end());
                    double max_y = *std::max_element(city_y.begin(), city_y.end());

                    // Add padding
                    double padding_x = (max_x - min_x) * 0.05; // 5% padding
                    double padding_y = (max_y - min_y) * 0.05; // 5% padding

                    // Set up axes with padding
                    ImPlot::SetupAxes("X Coordinate", "Y Coordinate", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                    ImPlot::SetupAxisLimits(ImAxis_X1, min_x - padding_x, max_x + padding_x, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, min_y - padding_y, max_y + padding_y, ImGuiCond_Always);

                    // Add legend
                    ImPlot::SetupLegend(ImPlotLocation_NorthEast);

                    // Plot cities as scatter points
                    ImPlot::PlotScatter("Cities", city_x.data(), city_y.data(), city_x.size(), 3.0f); // Marker size 3.0f

                    // Plot WOC route as a connected line
                    ImPlot::PlotLine("WOC Route", woc_route_x.data(), woc_route_y.data(), woc_route_x.size());

                    ImPlot::EndPlot();
                }
                ImGui::EndTabItem();
            }


            ImGui::EndTabBar();
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
#ifdef __APPLE__
        glViewport(0, 0, display_w, display_h);
#else
        glViewport(0, 0, display_w, display_h);
#endif
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render ImGui draw data
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}