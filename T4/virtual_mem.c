#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE  (1==1)
#define FALSE (!TRUE)

typedef int bool;
typedef struct s_virtual_array {
    int ctr;
    int* a;
    int* b;
    int* T;
} virtual_array;


void print_usage();
virtual_array initialize_virtual_arr(int n);
void assign_value(virtual_array* va, int i, int val);
bool consult_position(virtual_array* va, int i, int* out);
bool reset_virtual_array(virtual_array* va);
bool try_get_int(const char* str, int* out);

int main(int argc, char const *argv[])
{
    int i, n, pos, val;

    if (argc != 2) 
    {
        printf("Numero incorrecto de parametros de entrada\n");
        print_usage();
        return 1;
    }

    if (!try_get_int(argv[1], &n)) 
    {
        printf("El tamano del arreglo debe ser numerico\n");
        print_usage();
        return 1;
    }

    if (n <= 0)
    {
        printf("El tamano del arreglo debe ser al menos 0\n");
        print_usage();
        return 1;
    }

    virtual_array v_arr = initialize_virtual_arr(n);
    char input[256];
    while (TRUE)
    {
        printf(">");
        if (fgets(input, 255, stdin) == NULL)
        {
            printf("Problema con la entrada.\n");
            continue;
        }

    
        input[strcspn(input, "\n")] = 0;
        size_t inp_l = strlen(input);
        for (i = 0; i < inp_l; ++i) input[i] = toupper(input[i]);

        char* tok = strtok(input, " ");
        if (strcmp(tok, "ASIGNAR") == 0) 
        {
            tok = strtok(NULL, " ");
            if (tok == NULL)
            {
                printf("Falta POSICION\n");
                continue;
            }

            if (!try_get_int(tok, &pos))
            {
                printf("Asignar espera un valor numerico para POSICION\n");
                continue;
            }
            if (0 > pos || pos >= n)
            {
                printf("Posicion invalida\n");
                continue;
            }

            tok = strtok(NULL, " ");
            if (tok == NULL)
            {
                printf("Falta VALOR\n");
                continue;
            }

            if (!try_get_int(tok, &val))
            {
                printf("Asignar espera un valor numerico para VALOR\n");
                continue;
            }

            assign_value(&v_arr, pos, val);
        }
        else if (strcmp(tok, "CONSULTAR") == 0)
        {
            tok = strtok(NULL, " ");
            if (tok == NULL)
            {
                printf("Falta POSICION\n");
                continue;
            }

            if (!try_get_int(tok, &pos))
            {
                printf("Mal uso de comando CONSULTAR\n");
                continue;
            }

            if (0 > pos || pos >= n)
            {
                printf("Posicion invalida\n");
                continue;
            }

            if (!consult_position(&v_arr, pos, &val))
            {
                printf("Posicion no inicializada\n");
                continue;
            }

            printf("Valor obtenido: %d\n", val);
        }
        else if (strcmp(tok, "LIMPIAR") == 0)
            reset_virtual_array(&v_arr);
        else if (strcmp(tok, "SALIR") == 0)
            break;
        else
            printf("Comando desconocido.\n");
    }
    
    return 0;
}


void print_usage() 
{
    printf("USO: ejecutable <tamano arreglo>");
}


virtual_array initialize_virtual_arr(int n)
{
    if (n <= 0)
        return (virtual_array) {0, NULL, NULL, NULL};

    return (virtual_array) {
        0,
        calloc(n, sizeof(int)),
        calloc(n, sizeof(int)),
        calloc(n, sizeof(int))
    };
}


void assign_value(virtual_array* va, int i, int val)
{
    if (consult_position(va, i, NULL)) 
    {
        va->T[i] = val;
        return;
    }

    va->T[i] = val;
    va->a[va->ctr] = i;
    va->b[i] = va->ctr;
    va->ctr += 1;
}


bool consult_position(virtual_array* va, int i, int* out)
{
    if (0 <= va->b[i] && va->b[i] < va->ctr)
    {
        if (va->a[va->b[i]] != i)
            return FALSE;

        if (out != NULL)
            *out = va->T[i];

        return TRUE;
    }

    return FALSE;
}


bool reset_virtual_array(virtual_array* va)
{
    va->ctr = 0;
    return TRUE;
}


bool try_get_int(const char* str, int* out)
{
    int n = atoi(str);
    if (n == 0 && str[0] != '0')
        return FALSE;

    if (out != NULL)
        *out = n;
    return TRUE;
}