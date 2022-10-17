# DeterminantCalc
Simple determinant calculator for systems of linear equations using Cramer's method  

Command line arguments: input file, output file. Ex: main.c in.txt out.txt  
Input file should contain size of the matrix, then the matrix itself: (size) strings and (size+1) columns  
Ex:  
4  
1 1 1 4 5  
0 0 1 2 3  
1 2 0 2 4  
1 2 3 4 90   

Your SLE was:  
x + y + z + 4w = 5  
z + 2w = 3  
x + 2y + 2w = 4  
x + 2y + 3z + 4w = 90  
