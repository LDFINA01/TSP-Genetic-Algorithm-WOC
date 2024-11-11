# Solving the Traveling Salesman Problem using Genetic Algorithms and Wisdom of Crowds

### Author: Luis D. Finale
Computer Science & Engineering\
J.B Speed School of Engineering\
University of Louisville, USA\
ldfina01@louisville.edu

---

## Introduction

This project addresses the classic **Traveling Salesman Problem (TSP)**, which involves finding the shortest possible route that visits each city exactly once and returns to the origin city. The TSP is known for its computational complexity and is a well-known problem in combinatorial optimization.

Building upon previous work with Genetic Algorithms (GA), this project introduces an innovative approach by incorporating the **Wisdom of Crowds (WOC)** methodology. The combination of GA and WOC aims to enhance the efficiency and accuracy of finding optimal or near-optimal solutions to the TSP.

Additionally, the project utilizes **OpenGL** with the **GLFW**, **ImGui**, and **ImPlot** libraries to provide a graphical user interface (GUI) that visualizes the GA in action and showcases the final paths created by the algorithm.

---

## Approach

### Genetic Algorithm (GA)

The Genetic Algorithm is an evolutionary algorithm inspired by natural selection. Key components include:

- **Population Initialization**: A diverse population of possible routes is generated randomly.
- **Selection**: The top-performing routes (individuals) are selected based on their fitness (shorter distances).
- **Crossover and Mutation**: Selected individuals undergo crossover and mutation to produce new offspring routes.
- **Dynamic Mutation Rate**: The mutation rate starts at 5% and increases dynamically after 50% of the total generations have passed.

### Wisdom of Crowds (WOC)

The Wisdom of Crowds method leverages the collective intelligence of a group to make better decisions. In this project:

- **Elite Population**: The top 4% of the population from each generation are considered "experts" and stored in a `WOCPopulation` data structure.
- **Aggregation of Routes**: Analyzes city-to-city transitions across elite routes to identify the most common paths.
- **Construction of WOC Route**: Builds a new route by following the most frequent transitions, ensuring each city is visited exactly once and preventing cycles or repeats.

### Visualization with GLFW, ImGui, and ImPlot

To enhance the understanding of the GA and WOC methods, the project includes a graphical user interface built using:

- **GLFW**: A library for creating windows, contexts, and handling input and events.
- **ImGui**: A bloat-free graphical user interface library for C++.
- **ImPlot**: An immediate mode plotting library for ImGui.

The GUI provides:

- **Real-Time Visualization**: Displays the GA in action, showing how the algorithm evolves over generations.
- **Path Visualization**: Shows the final paths created by both the GA and WOC methods.
- **Interactive Interface**: Allows users to interact with the algorithm, adjust parameters, and observe changes.

### Integration of GA, WOC, and Visualization

By integrating GA with WOC and providing visualization tools, the project aims to:

- **Enhance Understanding**: Visual representations help in understanding how the algorithms work.
- **Provide Insight**: Users can see how the routes improve over time and how the WOC method aggregates the best solutions.
- **Improve User Experience**: An interactive GUI makes the project more accessible and engaging.

---

## Results

The combined GA and WOC approach was tested on various datasets with different numbers of cities, including:

- `Random11.tsp`
- `Random22.tsp`
- `Random30.tsp`
- `Random40.tsp`
- `Random44.tsp`
- `Random77.tsp`
- `Random97.tsp`
- `Random100.tsp`
- `Random222.tsp`

### Performance Highlights

- **Improved Solutions**: The WOC method often produced shorter routes compared to the best routes found by GA alone, especially with larger datasets and higher numbers of generations.
- **Efficiency**: The dynamic mutation rate and optimization of genetic operators enhanced the efficiency of the GA.
- **Scalability**: The benefits of the WOC approach became more pronounced with larger numbers of cities and increased generations.
- **Visualization**: The GUI provided clear visual evidence of the improvements, making it easier to compare GA and WOC routes.

### Data Analysis

**Execution Time and Path Cost Comparisons:**

| Cities | Generations | Time (ms)      | GA Distance   | WOC Distance  |
|--------|-------------|----------------|---------------|---------------|
| 44     | 500         | Avg: 652.2     | Avg: 763.72   | Avg: 645.12   |
|        | 2500        | Avg: 4,117.80  | Avg: 713.13   | Avg: 605.44   |
| 222    | 500         | Avg: 5,571.00  | Avg: 3,506.20 | Avg: 3,285.70 |
|        | 2500        | Avg: 27,334.60 | Avg: 2,868.51 | Avg: 2,304.42 |

- **Observation**: The WOC approach consistently outperformed the GA alone in terms of finding shorter routes, with the most significant improvements observed in larger datasets and higher generation counts.

---

## Conclusion

The integration of Genetic Algorithms with the Wisdom of Crowds methodology provides a robust approach to solving the TSP. By leveraging the strengths of both methods and utilizing visualization tools, the project demonstrates that collective insights can lead to more efficient solutions than individual algorithms alone. The dynamic mutation rate, focus on elite populations, and graphical interface contribute to the overall effectiveness and accessibility of the solution.

---

## How to Run the Project

### Prerequisites

- **Compiler**: A C++ compiler supporting C++11 or higher.
- **Build System**: **CMake** version **3.26** is used to manage the build process.
- **Libraries**:
    - **GLFW**: For creating windows and handling input.
    - **ImGui**: For the graphical user interface.
    - **ImPlot**: For plotting data within the GUI.
- **OpenGL**: Required for rendering the GUI.

### Installation of Dependencies

#### GLFW

- Download from the [official website](https://www.glfw.org/).
- Follow the installation instructions for your platform.

#### ImGui

- Clone the repository from [GitHub](https://github.com/ocornut/imgui).
- Include the necessary files in your project.

#### ImPlot

- Clone the repository from [GitHub](https://github.com/epezent/implot).
- Include the necessary files in your project.
- Ensure that ImPlot is integrated with ImGui.

### Files

- `main.cpp`: The main source code file containing the implementation.
- `CMakeLists.txt`: Configuration file for CMake to manage the build process.
- `RandomXX.tsp`: Dataset files where `XX` represents the number of cities (e.g., `Random44.tsp`).

### Compilation

The project uses **CMake** version **3.26** to manage the build process, handle dependencies, and generate appropriate build files for different platforms.

---

## Project Structure

- **Data Structures**:
    - `WOCPopulation`: A 2D vector storing elite routes from each generation.
    - `nextCommonCity`: A 2D array used to store and analyze the frequency of city-to-city transitions.

- **Algorithms**:
    - **Selection**: Roulette wheel or tournament selection methods.
    - **Crossover**: Cycle crossover or order crossover techniques.
    - **Mutation**: Swap mutation with a dynamic mutation rate.

- **Visualization Components**:
    - **GLFW**: Handles window creation and input events.
    - **ImGui**: Provides the GUI framework.
    - **ImPlot**: Used for plotting the routes and other data.

- **Build System**:
    - **CMakeLists.txt**: Configuration file for CMake to manage the build process, handle dependencies, and generate build files for various platforms.

---

## References

- **GLFW**:
    - “GLFW - An Open Source Multi-platform Library for OpenGL.” [https://www.glfw.org/](https://www.glfw.org/).

- **ImGui**:
    - Ocornut, Omar. “Dear ImGui.” *GitHub*, [https://github.com/ocornut/imgui](https://github.com/ocornut/imgui).

- **ImPlot**:
    - Pezent, Evan. “ImPlot.” *GitHub*, [https://github.com/epezent/implot](https://github.com/epezent/implot).

- **File Handling in C++**:
    - “How to Read from a File in C++?” *GeeksforGeeks*, [https://www.geeksforgeeks.org/how-to-read-from-a-file-in-cpp/](https://www.geeksforgeeks.org/how-to-read-from-a-file-in-cpp/).

- **Genetic Algorithms Introduction**:
    - “Genetic Algorithms.” *GeeksforGeeks*, [https://www.geeksforgeeks.org/genetic-algorithms/](https://www.geeksforgeeks.org/genetic-algorithms/).

- **Crossover Operators Explanation**:
    - Garg, Apar. “Crossover Operators in Genetic Algorithm.” *Medium*, Geek Culture, [https://medium.com/geekculture/crossover-operators-in-ga-cffa77cdd0c8](https://medium.com/geekculture/crossover-operators-in-ga-cffa77cdd0c8).

---

## Contact Information

For questions or further information, please contact:

**Luis D. Finale**\
Email: ldfina01@louisville.edu

---

## License

This project is licensed under the MIT License - see the LICENSE.md file for details.

---

## Acknowledgments

- Special thanks to Dr. Yampolski for his invaluable guidance in Artificial Intelligence, which has been instrumental in the development of this project.
- Thanks to the University of Louisville and the Speed School of Engineering for supporting this project.
- Appreciation to the online communities and resources that provided valuable insights into Genetic Algorithms, optimization techniques, and graphical interfaces.

---