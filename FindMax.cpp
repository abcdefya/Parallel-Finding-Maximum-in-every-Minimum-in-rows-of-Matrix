#include <iostream>
#include <omp.h>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>

/*-----------------------GLOBAL VARIABLES DECLARATION --------------------------*/
int SAMPLES = 1;
int matrixSize = 1;
int** matrix;
int THREADS = 32;

/* ------------------------- FUNCTION PROTOTYPES ----------------------------- */
void deallocate_matrix(int row, int** mat);  // Function to deallocate memory for a matrix
void generate_matrix(int n, int iteration); // Function to generate a matrix with random values
int sequential_min(int* array, int size); // Function to find the minimum value in an array sequentially
void print_matrix_to_file(int n); // Function to print a matrix to a file
std::pair<double, int> serial_find_max(int n); // Function to find the maximum of the minimum values in a matrix (serial version)
std::pair<double, int> parallel_find_max(int n); // Function to find the maximum of the minimum values in a matrix (parallel version)
void serial_Selection(); // Function to perform operations in the serial selection mode
void parallel_Selection(); // Function to perform operations in the parallel selection mode


/* ------------------------- CODE FOR EACH FUNCTION----------------------------- */

/*
 Function to deallocate memory for a matrix
 Parameters:
   - row: Number of rows in the matrix
   - mat: Pointer to the matrix to be deallocated
*/
void deallocate_matrix(int row, int** mat) {
    // Deallocate memory for each row (array of integers)
    for (int i = 0; i < row; ++i) {
        delete[] mat[i];
    }

    // Deallocate memory for the array of row pointers
    delete[] mat;
} // End function


/*
------------- Function to generate a matrix with random values--------------
 Parameters:
   - n: Size of the square matrix (number of rows and columns)
   - iteration: A value used to seed the random number generator for each iteration
*/
void generate_matrix(int n, int iteration) {
    // Seed the random number generator using the current time and iteration value
    srand(time(NULL) + iteration);

    // Allocate memory for the matrix (2D array)
    matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }

    // generate the matrix with random values between 0 and 14999
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 30000;
        }
    }
} // End function


/*
-------------- Function to find the minimum value in an array sequentially--------------
 Parameters:
   - array: Pointer to the array of integers
   - size: Number of elements in the array
 Returns:
   - The minimum value in the array
*/
int sequential_min(int* array, int size) {
    // Initialize the minimum value to the maximum possible integer value
    int min_value = INT_MAX;

    // Iterate through each element in the array
    for (int i = 0; i < size; ++i) {
        // Check if the current element is smaller than the current minimum value
        if (array[i] < min_value) {
            // Update the minimum value if the current element is smaller
            min_value = array[i];
        }
    }

    // Return the final minimum value found in the array
    return min_value;
} // End function


/*
---------- Function to print a matrix to a file ------
Parameters:
- outputFile: Open file stream to which the matrix will be written
- n: Size of the square matrix (number of rows and columns)
*/
void print_matrix_to_file(std::ofstream& outputMatFile, int n) {
    // Check if the file stream is in a valid state
    if (outputMatFile.is_open()) {
        // Iterate through each row of the matrix
        for (int i = 0; i < n; ++i) {
            // Iterate through each column of the matrix
            for (int j = 0; j < n; ++j) {
                // Write each matrix element followed by a space to the file
                outputMatFile << matrix[i][j] << " ";
            }
            // Move to the next line after writing each row
            outputMatFile << "\n";
        }
        // Add a blank line after writing the entire matrix
        outputMatFile << "\n";

        // Print a success message to the console
        std::cout << "Matrix written to the file stream successfully.\n";
    } else {
        // Print an error message to the standard error stream if the file stream is not valid
        std::cerr << "Invalid file stream. Unable to write the matrix.\n";
    }
} // End function


/*
----------Function to find the maximum of the minimum values in a matrix (serial version)--------
Parameters:
   - n: Size of the square matrix (number of rows and columns)
Returns:
   - A std::pair containing the execution time and the maximum of the minimum values
*/
std::pair<double, int> serial_find_max(int n) {
    // Variables to measure execution time and store the maximum of the minimum values
    double startTime, endTime;
    int max_min_value_sequential = INT_MIN;

    // Record the start time of the function execution
    startTime = omp_get_wtime();

    // Iterate through each row of the matrix
    for (int i = 0; i < n; ++i) {
        // Find the minimum value in the current row using the sequential_min function
        int min_value = sequential_min(matrix[i], n);

        // Update the maximum of the minimum values if a larger minimum value is found
        if (min_value > max_min_value_sequential) {
            max_min_value_sequential = min_value;
        }
    }

    // Record the end time of the function execution
    endTime = omp_get_wtime();

   // printf("Maximum of serial: %d\n", max_min_value_sequential);

    return std::make_pair(endTime - startTime, max_min_value_sequential);
} // End Function


/*
-----------Function to find the maximum of the minimum values in a matrix (parallel version)----------
Parameters:
- n: Size of the square matrix (number of rows and columns)
Returns:
- A std::pair containing the execution time and the maximum of the minimum values
*/
std::pair<double, int> parallel_find_max(int n) {
    // Variables to measure execution time and store the maximum of the minimum values in parallel
    double startTime, endTime;
    int max_min_value_parallel = INT_MIN;

    // Set the number of threads
    omp_set_num_threads(THREADS);

    // Record the start time of the function execution
    startTime = omp_get_wtime();

    // OpenMP parallel for loop to iterate through each row of the matrix in parallel
#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        // Find the minimum value in the current row using the sequential_min function
        int min_value = sequential_min(matrix[i], n);

        // Use a critical section to update the maximum of the minimum values safely
#pragma omp critical
        {
            if (min_value > max_min_value_parallel) {
                max_min_value_parallel = min_value;
            }
        }
    }

    // Record the end time of the function execution
    endTime = omp_get_wtime();

    return std::make_pair(endTime - startTime, max_min_value_parallel);
} // End function


/*----------Function to perform operations in the serial selection mode-----------*/ 
void serial_Selection() {
    // Variables to store matrix size, total elapsed time, and maximum value in serial mode
    double totalElapsedTime = 0;
    int max_serial_value;

    // Open a file named "serial_max.txt" for writing in append mode
    std::ofstream outputFile("serial_max.txt", std::ios::trunc);

    // Iterate through the specified number of samples
    for (int i = 0; i < SAMPLES; i++) {
        // generate the matrix with random values and print it to a file
        generate_matrix(matrixSize, i);
        
        // Find the maximum of the minimum values in the matrix using the serial approach
        std::pair<double, int> result = serial_find_max(matrixSize);

        // Update total elapsed time and maximum value in serial mode
        totalElapsedTime += result.first;
        max_serial_value = result.second;

        // Write the maximum value to the file and deallocate the matrix
         outputFile << max_serial_value << "\n";
        // print_matrix_to_file(outputFile, matrixSize);

        deallocate_matrix(matrixSize, matrix);
    }

    outputFile.close();

    // Calculate and print the average time for finding the maximum of every minimum
    double average = totalElapsedTime / SAMPLES;
    printf("Serial: Average time for finding the maximum of every minimum on %d x %d Matrix is %f\n", matrixSize, matrixSize, average);

   // printf("Max values written to serial_max.txt.\n");
    printf("\n---------------------------------------------\n");

} // End function


/*--------- Function to perform operations in the parallel selection mode -----------*/ 
void parallel_Selection() {
    // Variables to store matrix size, total elapsed time, and maximum value in serial mode
    double totalElapsedTime = 0;
    int max_parallel_value;

    // Open a file named "parallel_max.txt" for writing in append mode
   // std::ofstream outputFile("parallel_max.txt", std::ios::trunc);
    
        
        // Iterate through the specified number of samples
        for (int i = 0; i < SAMPLES; i++) {
        // generate the matrix with random values and print it to a file
        generate_matrix(matrixSize, i);
        
        // Find the maximum of the minimum values in the matrix using the parallel approach
        std::pair<double, int> result = parallel_find_max(matrixSize);

        // Update total elapsed time and maximum value in parallel mode
        totalElapsedTime += result.first;
        max_parallel_value = result.second;

        // Write the maximum value to the file and deallocate the matrix
       // outputFile << max_parallel_value << "\n";
        //print_matrix_to_file(outputFile, matrixSize);
        deallocate_matrix(matrixSize, matrix);
    
        }
   // outputFile.close();
    
    // Calculate and print the average time for finding the maximum of every minimum
    double average = totalElapsedTime / SAMPLES;
    printf("Parallel: Average time for finding the maximum of every minimum on %d x %d Matrix is %f\n", matrixSize, matrixSize, average);

  //  printf("Max values written to parallel_max.txt.\n");
    printf("\n---------------------------------------------\n");
    
} // End function


/*-------- Function to control the main menu for program selection ---------*/ 
void control_menu() {
    int selection;

    // Infinite loop to continuously prompt the user for program selection
    while (true) {
        printf("Enter one of the options given below\n \t1 - For serial program \n\t2 - For parallel program\n \t3 - Exit\n");
        scanf("%d", &selection);

        // Exit the loop and program if the user selects option 3
        if (selection == 3) return;

        // Prompt the user to enter the sample size
        printf("Enter sample size: ");
        scanf("%d", &SAMPLES);

        // Matrix Size input
        printf("Enter matrixSize: ");
        if (scanf("%d", &matrixSize) != 1) {
            printf("Invalid input for matrixSize. Exiting...\n");
            return;
        }

        // Prompt the user to input the number of threads
        printf("Enter the number of threads (default is %d): ", THREADS);
        scanf("%d", &THREADS);

        // Perform actions based on user's selection
        switch (selection) {
            case 1:
                // Call the function to perform operations in serial mode
                serial_Selection();
                break;

            case 2:
                // Call the function to perform operations in parallel mode
                parallel_Selection();
                break;
        }
    }
} // End function

/* ------------------------- MAIN FUNCTION ----------------------------- */
int main() {
    
    control_menu();
    return 0;

} 

