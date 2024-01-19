To run the program, make sure you already installed the OpenMP library In your direct file, open Command Prompt and build the program by this line of code:

g++ -fopenmp [name_of_CPP_file].cpp -o [name_of_CPP_file]
for example: g++ -fopenmp FindMax.cpp -o FindMax

After running this code, a new [name_of_CPP_file].exe file will be generated run: name.exe
==> FindMax.exe

![image](https://github.com/abcdefya/Parallel-Finding-Maximum-in-every-Minimum-in-rows-of-Matrix/assets/148352681/16bfd07a-a1f0-4ec6-92ea-33134d7262de)

### Problem
Given a square matrix 

<img src="https://latex.codecogs.com/svg.image?&space;A=\begin{bmatrix}a_{11}&a_{12}&\cdots&a_{1n}\\a_{21}&a_{22}&\cdots&a_{2n}\\\vdots&\vdots&\ddots&\vdots\\a_{n1}&a_{n2}&\cdots&a_{nn}\end{bmatrix}" title=" A=\begin{bmatrix}a_{11}&a_{12}&\cdots&a_{1n}\\a_{21}&a_{22}&\cdots&a_{2n}\\\vdots&\vdots&\ddots&\vdots\\a_{n1}&a_{n2}&\cdots&a_{nn}\end{bmatrix}" />

On each row i, we find the smallest value in the row $a_{ij}$, we get a set of minimum values of rows ${a_{1j_{1}}, a_{2j_{2}} , ..., a_{nj_{n}}}$, then we choose the largest value in that set.
