#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COMPONENTES "Componentes.bin"
#define PAQUETES "Paquetes.bin"
#define CANTMAX 1000

//paquetes.bin
typedef struct
{
    int cantMateria; //gramos
    char calidad; // "a", "b", "c"
    float costo; //cada bloque posee distintos costos en base a los gramos

}PaqueteMateriaPrima;

//componentes.bin
typedef struct
{
    char nombre[20]; //"Biblia", "Copa de vino", "Doll"
    int falla; //puede tener 0...2 fallas
    float costo;
    char calidad; //según la calidad de la materia, será la calidad del producto

}Componente;

//PROTOTIPADO

void despersistencia (PaqueteMateriaPrima arr[], int* val);
void muestraarreglo (PaqueteMateriaPrima arr[], int val);
void persistencia (Componente* dato);
void carga (PaqueteMateriaPrima arr[], int val);
int cargaporcomponente(PaqueteMateriaPrima arr[], int pos, char nombre[], Componente* p, int materia, float* costoxgramo);
void costocomponente (Componente* a, float costo_g);
void muestracomponentes ();
void mostrarcomponente (Componente a);

int main()
{
    PaqueteMateriaPrima paquetes[CANTMAX];
    int val = 0;

    despersistencia (paquetes, &val);
    printf ("ARCHIVO PAQUETES:\n");
    muestraarreglo (paquetes, val);
    printf ("--------------------\n");
    carga (paquetes, val);
    printf ("ARCHIVO COMPONENTES:\n\n");
    muestracomponentes();

    return 0;
}

void despersistencia (PaqueteMateriaPrima arr[], int* val)
{
    FILE* fp = fopen(PAQUETES, "rb");
    PaqueteMateriaPrima p;
    if (fp != NULL)
    {
        while(fread(&p, sizeof(PaqueteMateriaPrima), 1, fp) > 0 && *val < CANTMAX)
        {
            arr[*val] = p;
            (*val)++;
        }
        fclose(fp);
    }
}

void muestraarreglo (PaqueteMateriaPrima arr[], int val)
{
    int i = 0;
    while (i < val)
    {
        printf("Cantidad de materia: %i \n", arr[i].cantMateria);
        printf("Calidad: %c \n", arr[i].calidad);
        printf("Costo: %.2f \n", arr[i].costo);
        printf("\n");
        i++;
    }
}

void persistencia (Componente* dato)
{
    FILE* fp = fopen(COMPONENTES, "ab");
    if (fp != NULL)
    {
        fwrite(dato, sizeof(Componente), 1, fp);
        fclose(fp);
    }
}

void carga (PaqueteMateriaPrima arr[], int val)
{
    Componente p;
    int i = 0;
    int flag = 0;
    srand(time(NULL));
    int materiaDoll = 25, materiaCopa = 3, materiaBiblia = 2;
    char DOLL[] = "Doll";
    char COPA[] = "Copa de vino";
    char BIBLIA[] = "Biblia";

    float costoxGramo = arr[i].costo / arr[i].cantMateria;

    while (i < val)
    {
        while (i < val && flag == 0)
        {
            if (flag == 0 && arr[i].cantMateria >= materiaDoll)
            {
                flag = cargaporcomponente (arr, i, DOLL, &p, materiaDoll, &costoxGramo);
            }
            else
            {
                i++;
                costoxGramo = arr[i].costo / arr[i].cantMateria;
            }
        }
        flag = 0;


        while (i < val && flag == 0)
        {
            if (flag == 0 && arr[i].cantMateria >= materiaCopa)
            {
                flag = cargaporcomponente (arr, i, COPA, &p, materiaCopa, &costoxGramo);
            }
            else
            {
                i++;
                costoxGramo = arr[i].costo / arr[i].cantMateria;
            }
        }
        flag = 0;

        while (i < val && flag == 0)
        {
            if (flag == 0 && arr[i].cantMateria >= materiaBiblia)
            {
                flag = cargaporcomponente (arr, i, BIBLIA, &p, materiaBiblia, &costoxGramo);
            }
            else
            {
                i++;
                costoxGramo = arr[i].costo / arr[i].cantMateria;
            }
        }
        flag = 0;
    }
}

int cargaporcomponente (PaqueteMateriaPrima arr[], int pos, char nombre[], Componente* p, int materia, float* costoxgramo)
{
    int flag;
    arr[pos].cantMateria = arr[pos].cantMateria - materia;

    strcpy((*p).nombre, nombre);

    (*p).falla = (rand() % 3);

    costocomponente (&*p, *costoxgramo);

    (*p).calidad = arr[pos].calidad;

    persistencia (&*p);

    flag = 1;

    return flag;
}

void costocomponente (Componente* a, float costo_g)
{

    if (strcmp((*a).nombre, "Doll") == 0)
    {
        float resultado = ((25 * costo_g) + 6);
        (*a).costo = resultado;
    }

    if (strcmp((*a).nombre, "Copa de vino") == 0)
    {
        float resultado = ((3 * costo_g) + 0.75);
        (*a).costo = resultado;
    }

    if (strcmp((*a).nombre, "Biblia") == 0)
    {
        float resultado = ((2 * costo_g) + 0.50);
        (*a).costo = resultado;
    }
}

void muestracomponentes ()
{
    Componente p;
    int i = 0;
    FILE* fp = fopen(COMPONENTES, "rb");
    if (fp != NULL)
    {
        while (fread(&p, sizeof(Componente), 1, fp) > 0)
        {
            mostrarcomponente(p);
            i++;
        }
        fclose(fp);
    }
    printf("CANTIDAD DE COMPONENTES %i \n", i);

}

void mostrarcomponente (Componente a)
{

    printf("Nombre -> %s \n", a.nombre);
    printf("Falla: %i \n", a.falla);
    printf("Costo: %.2f \n", a.costo);
    printf("Calidad: %c \n", a.calidad);
    printf("\n");
}
