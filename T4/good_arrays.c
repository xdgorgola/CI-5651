#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE  (1==1)
#define FALSE (!TRUE)

int count_good_arrays(int* A, int n);

int main(int argc, char const *argv[])
{
    printf("%d\n", count_good_arrays((int []) {2,2,1,22,15}, 5));
    printf("%d\n", count_good_arrays((int []) {1,2,3,4}, 4));
}

/* 
    Cuenta la cantidad de buenos subarreglos de un arreglo
    A: Arreglo a contar buenos subarreglos
    n: Tamano del arreglo
*/
int count_good_arrays(int* A, int n)
{
    int** p = calloc(2, sizeof(int*)); /* Cantidad de arreglos de tamano i - 1 que terminan en los elementos menores a j*/
    p[0]    = calloc(n, sizeof(int));
    p[1]    = calloc(n, sizeof(int));
    int i,j,c = n;
    
    
    /* Arreglos de tamano i que terminan en el j-esimo elemento */
    for (i = 1; i <= n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            /* Caso base. Todos los arreglos de tamaño i que terminen un elemento son posibles por divisivilidad entre 1  */
            if (i == 1)
            {
                p[i&1][j] = j;
                continue;
            }

            /* No puedes terminar en el j-esimo elemento si este elemento es menor que el tamaño */
            if (j+1 < i) 
            {
                p[i&1][j] = j > 0 ? p[(i+1)&1][j-1] : 0;
                continue;
            }

            /* 
                Si el elemento en el que acaba el arreglo es divisible entre su largo...
                Podemos tomar todos los arreglos de tamano i-1 que terminan en los elementos
                menores a j y concatenarle j :) Bottom-Up dado a que construimos los nuevos
                resultados con la fila anterior!

                Aumentamos nuestro contador!
            */
            if (A[j] % i == 0)
                c += p[(i+1)&1][j];
            else if (j > 0)
                p[i&1][j] = p[(i+1)&1][j-1];
        }
    }
    free(p[0]);
    free(p[1]);
    free(p);
    return c;
}