#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRODUCTOS "ProductosFinales.bin"
#define PERDIDAS "Perdidas.bin"
#define CONTADURIA "Contaduria.bin"
#define DETALLE "DetalleLote.bin"
#define CANTMAX 1000
#define porcentajeA 1.25
#define porcentajeB 1.23
#define porcentajeC 1.2

typedef struct
{
    float gananciaBruta;
    float costoLote;
    float costoPerdidas;
    int cantTotal;
}Contaduria;

typedef struct
{
    float precioFinal;
    int cant;
    char calidad;
}DetalleLote;

typedef struct
{
    float costo;
    char calidad;
}PlayMobil;

//PROTOTIPADO

void despersistenciaPM (PlayMobil arr[], int* val);
void despersistenciaP (float arr[CANTMAX], int* val);
void cargaDetalleLote (PlayMobil arrP[], int valP, DetalleLote arr[], Contaduria* c);
void precioF (DetalleLote arr[], int val, Contaduria* c);
void perdidasFun (Contaduria* c, float arr[], int val);
void gananciaB (DetalleLote arr[], int val, Contaduria* c);
void persistenciaDL (DetalleLote arr[], int val);
void persistenciaC (Contaduria c);
void mostrarLote (DetalleLote arr[], int val);
void Lote (DetalleLote dato);
void muestraCont (Contaduria dato);

int main()
{
    PlayMobil playM [CANTMAX];
    int validosPM = 0;

    float perdidas[CANTMAX];
    int validosP = 0;

    DetalleLote lote[CANTMAX];
    int validosL = 3;

    Contaduria cont;

    //-------------------------------------//

    despersistenciaPM (playM, &validosPM);

    despersistenciaP (perdidas, &validosP);

    cargaDetalleLote (playM, validosPM, lote, &cont);

    precioF (lote, validosL, &cont);

    perdidasFun (&cont, perdidas, validosP);

    mostrarLote (lote, validosL);

    gananciaB (lote, validosL, &cont);

    muestraCont (cont);

    persistenciaDL (lote, validosL);

    persistenciaC (cont);

    return 0;
}

void despersistenciaPM (PlayMobil arr[], int* val)
{
    FILE* fp = fopen(PRODUCTOS, "rb");
    PlayMobil p;
    if (fp != NULL)
    {
        while (fread(&p, sizeof(PlayMobil), 1, fp) > 0 && *val < CANTMAX)
        {
            arr[*val] = p;
            (*val)++;
        }
        fclose(fp);
    }
}

void despersistenciaP (float arr[CANTMAX], int* val)
{
    FILE* fp = fopen(PERDIDAS, "rb");
    float p;
    if (fp != NULL)
    {
        while (fread(&p, sizeof(float), 1, fp) > 0 && *val < CANTMAX)
        {
            arr[*val] = p;
            (*val)++;
        }
        fclose(fp);
    }
}

void cargaDetalleLote (PlayMobil arrP[], int valP, DetalleLote arr[], Contaduria* c)
{
    int i = 0;
    int cant = 0;

    (*c).costoLote = 0;
    (*c).cantTotal = 0;

    arr[0].calidad = 'a';
    arr[0].cant = 0;

    arr[1].calidad = 'b';
    arr[1].cant = 0;

    arr[2].calidad = 'c';
    arr[2].cant = 0;

    while (i < valP)
    {
        if (arrP[i].calidad == 'a')
        {
            arr[0].cant = arr[0].cant + 1;
            (*c).costoLote = (*c).costoLote + arrP[i].costo;
            (*c).cantTotal = (*c).cantTotal + 1;
        }

        if (arrP[i].calidad == 'b')
        {
            arr[1].cant = arr[1].cant + 1;
            (*c).costoLote = (*c).costoLote + arrP[i].costo;
            (*c).cantTotal = (*c).cantTotal + 1;
        }

        if (arrP[i].calidad == 'c')
        {
            arr[2].cant = arr[2].cant + 1;
            (*c).costoLote = (*c).costoLote + arrP[i].costo;
            (*c).cantTotal = (*c).cantTotal + 1;
        }

        i++;
    }
}

void precioF (DetalleLote arr[], int val, Contaduria* c)
{
    int i = 0;
    float promedio = (*c).costoLote / (*c).cantTotal;

    while(i < val)
    {
        if(arr[i].calidad == 'a')
        {
            float precio = promedio + (promedio * porcentajeA);
            arr[i].precioFinal = precio;
        }
        if(arr[i].calidad == 'b')
        {
            float precio = promedio + (promedio * porcentajeB);
            arr[i].precioFinal = precio;
        }
        if(arr[i].calidad == 'c')
        {
            float precio = promedio + (promedio * porcentajeC);
            arr[i].precioFinal = precio;
        }

        i++;
    }
}

void perdidasFun (Contaduria* c, float arr[], int val)
{
    int i = 0;
    float p = 0;

    (*c).costoPerdidas = 0;

    while (i < val)
    {
        p = p + arr[i];
        i++;
    }

    (*c).costoPerdidas = p;
}

void gananciaB (DetalleLote arr[], int val, Contaduria* c)
{
    float ganB = 0, cos;
    int i = 0;

    while (i < val)
    {
        ganB = ganB + (arr[i].cant * arr[i].precioFinal);
        i++;
    }

    (*c).gananciaBruta = ganB;

    cos = (*c).costoLote + (*c).costoPerdidas;

    if (cos < ganB)
    {
        printf("Hay ganancia. \n");
        printf("\n");
    }
    else
    {
        printf("No hay ganancia. \n");
        printf("\n");
    }
}

void persistenciaDL (DetalleLote arr[], int val)
{
    int i = 0;
    FILE* fp = fopen(DETALLE, "wb");
    if (fp != NULL)
    {
        while (i < val)
        {
            fwrite(&arr[i], sizeof(DetalleLote), 1, fp);
            i++;
        }
        fclose(fp);
    }
}

void persistenciaC (Contaduria c)
{
    FILE* fp = fopen(CONTADURIA, "wb");
    if (fp != NULL)
    {
        fwrite(&c, sizeof(Contaduria), 1, fp);
        fclose(fp);
    }
}

void mostrarLote (DetalleLote arr[], int val)
{
    printf("LOTES \n");
    int i = 0;
    while (i < val)
    {
        DetalleLote p = arr[i];
        Lote (p);
        i++;
    }
}

void Lote (DetalleLote dato)
{
    printf("Precio Final: %.2f \n", dato.precioFinal);
    printf("Cantidad: %i \n", dato.cant);
    printf("Calidad: %c \n", dato.calidad);
    printf("\n");
}

void muestraCont (Contaduria dato)
{
    printf("CONTADURIA \n");
    printf("Ganancia bruta: %.2f \n", dato.gananciaBruta);
    printf("Costo lote: %.2f \n", dato.costoLote);
    printf("Costo perdidas: %.2f \n", dato.costoPerdidas);
    printf("Cantidad total: %i \n", dato.cantTotal);
    printf("\n");
}





