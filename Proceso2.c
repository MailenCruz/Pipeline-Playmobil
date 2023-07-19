#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nomarch "Componentes.bin"
#define perdidas "Perdidas.bin"
#define prodfinal "ProductosFinales.bin"
#define cantmax 10000
#define maxfallas 2

typedef struct
{
    char nombre[20]; //"Biblia", "Copa de vino", "Doll"
    int falla; //puede tener 0...2 fallas
    float costo;
    char calidad; //según la calidad de la materia, será la calidad del producto
} Componente;

typedef struct
{
    float costo;
    char calidad;
} PlayMobil;

///Prototipado:
void cargar_arreglos(Componente doll[], Componente wine[], Componente bible[], int*d, int*w, int*b);
void mostrar_arreglo (Componente arr[], int validos);
void dos_fallas (Componente total[], Componente definitivo[], int* validos);
void borrar_del_array (Componente arr[], int* validos, int pos);
void mandar_a_perdidas(Componente arr[], int pos);
void mostrar_archivo ();
float costo_final (Componente doll[], int posd, Componente bible[], int posb, Componente wine[], int posw);
float costo_pm (Componente doll[], int posd, Componente bible[], int posb, Componente wine[], int posw);
char calidad (Componente doll[], int pos1, Componente bible[], int* pos2, Componente wine[], int* pos3, int b, int w);
void cargar_PlayMobil (PlayMobil arr[], int* val, Componente doll[], Componente bible[], Componente wine[], int d, int b, int w);
void mostrar_playmobil(PlayMobil arr[], int val);
void persistir_PlayMobil (PlayMobil arr[], int validos);
void mostrar_arch_de_playmobil();



int main()
{
    int dol = 0;
    Componente doll_total[cantmax];
    int c = 0;
    Componente copas_total[cantmax];
    int bi = 0;
    Componente bible_total[cantmax];

    cargar_arreglos(doll_total,copas_total,bible_total,&dol,&c,&bi);


    Componente dolls[cantmax];
    Componente bible[cantmax];   //definitivos
    Componente copas[cantmax];

    dos_fallas(doll_total,dolls,&dol);
    dos_fallas(copas_total,copas,&c);
    dos_fallas(bible_total,bible,&bi);
    mostrar_archivo();


    PlayMobil prod_finales[cantmax];
    int validos = 0;

    cargar_PlayMobil(prod_finales,&validos,dolls,bible,copas,dol,bi,c);

    persistir_PlayMobil(prod_finales,validos);

    mostrar_playmobil(prod_finales,validos);


    //mostrar_arch_de_playmobil();

    return 0;
}

void cargar_arreglos(Componente doll[], Componente wine[], Componente bible[], int*d, int*w, int*b)
{
    FILE* fp = fopen(nomarch, "rb"); //abro el arch de componentes en modo lectura
    Componente aux;
    if(fp != NULL)
    {
        while(!feof(fp)) //imprimo los componentes que me trae el arch
        {
            fread(&aux,sizeof(Componente),1,fp);

            if(strcmp(aux.nombre, "Doll") == 0) //si el componente es un muñeco, va al arr de muñecos
            {
                doll[*d] = aux;
                (*d)++;
            }
            else
            {
                if(strcmp(aux.nombre, "Biblia") == 0) //si es una biblia va al arr de biblias
                {
                    bible[*b] = aux;
                    (*b)++;
                }
                else
                {
                    wine[*w] = aux; //si es una copa de vino, va al arreglo de copas
                    (*w)++;
                }
            }
        }

        fclose(fp);

    }
}

void mostrar_arreglo (Componente arr[], int validos)
{
    for(int i=0; i<validos; i++)
    {
        puts(arr[i].nombre);
        printf("costo: %.2f ",arr[i].costo);
        printf("fallas: %i ",arr[i].falla);
        printf("calidad: %c ",arr[i].calidad);
        printf("\n\n");
    }
    printf("\n");
}

void dos_fallas (Componente total[], Componente definitivo[], int* validos)
{
    int i;
    int posdef = 0;
    int fallass=0;
    float acumulador=0;


    for(i=0; i<(*validos); i++)
    {
        if(total[i].falla == 2) //si las fallas del componente son dos, lo manda a pérdidas y lo elimina del arr
        {
            mandar_a_perdidas(total,i);
        }
        else
        {
            definitivo[posdef] = total[i]; //si las fallas son cero o uno, los pone en el arr de definitivos
            posdef++;
        }
    }
    (*validos) = posdef;
}


void mandar_a_perdidas(Componente arr[], int pos)
{

    FILE* fp = fopen(perdidas, "ab"); //abro el arch en modo escritura
    if(fp != NULL)
    {
        fwrite(&arr[pos].costo,sizeof(float),1,fp);//inserto en el arch los costos de los componentes con dos fallas
        fclose(fp);
    }
}

void mostrar_archivo () //muestra perdidas
{
    FILE* fp = fopen(perdidas, "rb");
    if(fp != NULL)
    {
        float aux;
        printf("\n\n      PERDIDAS: \n\n");
        while(fread(&aux,sizeof(float),1,fp) > 0)
        {
            printf("%.2f  ",aux);
        }
        fclose(fp);
    }
}

void cargar_PlayMobil (PlayMobil arr[], int* val, Componente doll[], Componente bible[], Componente wine[], int d, int b, int w)
{
    int posd=0, posb=0, posw=0; //una pos para cada array

    char calidadPM;
    float costoFINAL;

    for(posd=0; posd<d; posd++)
    {
        calidadPM = calidad(doll,posd,bible,&posb,wine,&posw,b,w);
        if(calidadPM != 'n') //si calidad es 'a', 'b' o 'c'
        {
            arr[*val].calidad = calidadPM; //le asigna la calidad al playmobil
            costoFINAL = costo_final(doll,posd,bible,posb,wine,posw);
            arr[*val].costo = costoFINAL; //le asigno el costo al playmobil
            (*val)++;

            borrar_del_array(bible,&b,posb); //borra la biblia y la copa que usamos para el playmobil
            borrar_del_array(wine,&w,posw);
        }
    }

}

char calidad (Componente doll[], int pos1, Componente bible[], int* pos2, Componente wine[], int* pos3, int b, int w)
{
    char calidad;
    int flag;

    calidad = doll[pos1].calidad;
    flag = 0;

    while(*pos2<b && flag == 0)
    {
        if(bible[*pos2].calidad == calidad)
        {
            while(*pos3<w && flag == 0)
            {
                if(wine[*pos3].calidad == calidad)
                {
                    flag = 1;
                }
                else
                {
                    (*pos3)++;
                }
            }

        }
        else
        {
            (*pos2)++;
        }
    }



    if(flag) //encontró un muñeco, una biblia y una copa de vino de la misma calidad
    {
        return calidad;
    }
    else
    {
        return 'n';
    }


}

float costo_pm (Componente doll[], int posd, Componente bible[], int posb, Componente wine[], int posw)
{
    float costo = 0;

    costo = doll[posd].costo + bible[posb].costo + wine[posw].costo;

    return costo;
}



float costo_final (Componente doll[], int posd, Componente bible[], int posb, Componente wine[], int posw)
{
    float costofinal = costo_pm(doll,posd,bible,posb,wine,posw);

    if(doll[posd].calidad == 'a')
    {
        costofinal = costofinal + 14;
    }
    else
    {
        if(doll[posd].calidad == 'b')
        {
            costofinal = costofinal + 12;
        }
        else
        {
            costofinal = costofinal + 10;
        }
    }

    return costofinal;
}


void borrar_del_array (Componente arr[], int* validos, int pos)
{
    for(pos; pos+1 < (*validos); pos++)
    {
        arr[pos] = arr[pos+1];
    }
    (*validos)--;
}

void mostrar_playmobil(PlayMobil arr[], int val)
{
    printf("\n\n\n      PLAYMOBILs:");
    for(int i=0; i<val; i++)
    {
        printf("\n\n pm calidad ::: %c \n",arr[i].calidad);
        printf("\n pm costo :::: %.2f\n",arr[i].costo);
    }
}

void persistir_PlayMobil (PlayMobil arr[], int validos)
{
    FILE* fp = fopen(prodfinal, "ab");
    if(fp != NULL)
    {
        for(int i=0; i<validos; i++)
        {
            fwrite(&arr[i],sizeof(PlayMobil),1,fp);
        }
        fclose(fp);
    }
}

void mostrar_arch_de_playmobil()
{
    FILE* fp = fopen(prodfinal, "rb");
    PlayMobil arr[cantmax];
    int val =0;
    if(fp != NULL)
    {
        while(!feof(fp))
        {
            fread(&arr[val],sizeof(PlayMobil),1,fp);
            printf("\n\n\n %.2f   %c",arr[val].costo, arr[val].calidad);
            val++;
        }
        fclose(fp);
    }
}
