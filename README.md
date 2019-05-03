# Genetic-Programming-code-from-University-Dissertation
All C++ and MATLAB code written during Jack Hutton's final year university dissertation for BEng Automatic Control and Systems Engineering at the University of Sheffield. 

Individual.cpp  - A class to store the syntax trees for the five equations being modified and their metrics.
Generation.cpp  - A class to store an array of individuals, extract their syntax trees as C++ code and read/write metrics.
GP.cpp          - A file to provide the required methods to perform a genetic program run.
FileEdit.cpp    - A class to find and edit lines in text/cpp files, and read csv files.
main.cpp        - The main method, a genetic program (this file also contains several test methods).

Overall_Pareto_fronts_script  - A MATLAB script to create a figure of the first Pareto front of the overall population.
Overall_convergence_script.m  - A MATLAB script to load the output of the previous output script and construct convergence figures. 
