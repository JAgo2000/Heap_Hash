#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MEM_SIZE 100
#define ITEM_WIDTH 4
#define EPS 1e-10
#define HASH_SIZE 3
///////////////////////////////////////////////////////
typedef struct entry_s
{
    char songtitle[256], interpreter[256];
} entry_t;
///////////////////////////////////////////////////////
typedef struct heap_s
{
    entry_t *entries;
    long size;
    long next_free;
} heap_t;
/////////////////////////////////////////////////
typedef struct hasharray_s
{
    entry_t *entries;
    long num_entries;
} hasharray_t;
///////////////////////////////////////////////////////////
typedef struct hash_s
{
    hasharray_t *hasharrays;
    long hashsize;
} hash_t;
//////////////////////////////////////////////////////////////

heap_t *heap_create(long size)
{
    heap_t *new_heap;
    if((new_heap=calloc(1,sizeof(heap_t)))==0)//heap allozieren
    {
        printf("Speicher allozieren fehlgeschlagen,heap_create\n");
        return(0);
    }
    else
    {
        if((new_heap->entries=calloc(size,sizeof(entry_t)))==0)//entries allozieren
        {
            printf("Speicher allozieren fehlgeschlagen,heap_create\n");
            return(0);
        }
        else
        {
            new_heap->size=size;
            new_heap->next_free=1;
            return(new_heap);
        }

    }
}
/////////////////////////////////////////////////////////////////////
void up_heap(heap_t *heap, long index)
{
    char songtitle_temp[255];
    char interpreter_temp[255];
    strcpy(songtitle_temp,heap->entries[index].songtitle);
    strcpy(interpreter_temp,heap->entries[index].interpreter);

    while ((index >= 2) && (strcmp(heap->entries[index/2].songtitle,songtitle_temp)<=0))
    {
        heap->entries[index] = heap->entries[index/2];
        index = index/2; /* fortfahren mit dem Vorgänger */
    }
    strcpy(heap->entries[index].songtitle,songtitle_temp);
    strcpy(heap->entries[index].interpreter,interpreter_temp);
}
///////////////////////////////////////////////////
long heap_insert(heap_t *heap,char songtitle[],char interpreter[])
{
    long status;
    if(heap)
    {

        if (heap->next_free < MEM_SIZE)//nicht über die max größe schreiben
        {
            strcpy(heap->entries[(heap->next_free)].interpreter,interpreter);
            strcpy(heap->entries[(heap->next_free)].songtitle,songtitle);
            up_heap(heap, heap->next_free);
            heap->next_free = heap->next_free + 1;
            status=1;
        }
        else
        {
            printf("Speicher zu klein,heap_insert\n");
            status = 0;
        }

    }
    else
    {
        printf("Heap leer,heap_insert\n");
        status=0;
    }
    return status;

}
///////////////////////////////////////////////////////////////////////////////////
void heap_print(heap_t *heap, long index, long level)
{
     long i;
    if(heap)
    {
        if(heap->next_free<=index)
        {
            return;
        }
        heap_print(heap,index*2+1,level+1);

        for (i = 0; i < level; ++i) fprintf(stdout, "  ");

        printf("%s,%s\n", heap->entries[index].songtitle,heap->entries[index].interpreter);

        heap_print(heap,index*2,level+1);
    }
    else
    {
        printf("ungueltiger Zeiger,print_tree\n");
    }



}
/////////////////////////////////////////////////////////////////////////////
void heap_free(heap_t *heap)
{

    if((heap!=0)&&(heap->entries)!=0)
    {
        free(heap->entries);
        free(heap);
        printf("Gelöscht\n");
    }
    else
    {
        printf("Heap leer,heap_free\n");
    }

}
//////////////////////////////////////////////////////////////////
void read_from_keyboard(char songtitle[],char interpreter[])
{
    printf("Geben Sie den Songtitel an!\n");
    char zeichen;
    long n=0;

    while((zeichen=getchar())!='\n')
    {

        songtitle[n]=zeichen;
        n++;
        if(n>255)
        {
            break;
        }
    }
    songtitle[n]='\0';
    n=0;
    printf("Geben Sie den Interpreten an!\n");
    while((zeichen=getchar())!='\n')
    {

        interpreter[n]=zeichen;
        n++;
        if(n>255)
        {
            break;
        }
    }
    interpreter[n]='\0';
}
//////////////////////////////////////////////////////////////////////////////
long heap_search(heap_t *heap,char songtitle[],char interpreter[])
{
    long index=1;
    long ende=0;
    long breite=1;
    long weiter=1;
    int n=0;
    if((heap)&&((heap->next_free)!=1))
    {
        while((index<heap->next_free)&&(ende==0)&&(weiter!=0))
        {

            if(breite*2==index)//immer wenn die Zeile gewechselt wird(vertikal)
            {
                breite=index;//dann ist der index genau der spaltenanzahl(breite)
                weiter=breite;//
            }


            if(strcmp(songtitle,heap->entries[index].songtitle)>0)//wenn alle songtitle in dieser reihe kleiner sind als der gesuchte wird nicht mehr weitergesucht
            {
                weiter=weiter-1;
            }
            else
            {

                if((strcmp(songtitle,heap->entries[index].songtitle)==0)&&(strcmp(interpreter,heap->entries[index].interpreter))==0)
                {

                    ende=1;
                }

            }

            if(ende==0)
            {
                index=index+1;
            }
            n++;
        }

    }
    else
    {
        printf("Heap leer,heap_search\n");
        return(0);
    }
    printf("Durchläufe=%d\n",n);
    if(weiter==0||index==heap->next_free)//index auf null setzen wenn ende erreicht oder in der reihe kein größerer songtitle gefunden wurde
    {
        index=0;
    }

    return(index);
}
///////////////////////////////////////////////////////////////////////////////
void down_heap(heap_t *heap, long index)
{
    entry_t *mem = heap->entries;
    entry_t tmp = mem[index];
    long last = heap->next_free - 1;
    while (index <= last/2)
    {
        long next = index * 2;
        if ((next < last) && (strcmp(mem[next].songtitle,mem[next+1].songtitle)<0))
            next++; /* rechter Nachfolger oder linker */
        if ((strcmp(tmp.songtitle,mem[next].songtitle)>=0))
            break; /* gefunden! */
        mem[index] = mem[next];
        index = next;
    }
    mem[index] = tmp;
}
//////////////////////////////////////////////////////////////////////////////
long heap_delete_first(heap_t *heap)
{
    long status;

    if ((heap!=0)&&(heap->next_free!=1))
    {
        printf("ZU löschender Eintrag: %s, ",heap->entries[1].songtitle);
        printf("%s\n",heap->entries[1].interpreter);
        heap->next_free = heap->next_free - 1;
        heap->entries[1] = heap->entries[heap->next_free];
        down_heap(heap, 1);
        if(heap->next_free==1)
        {
            status=0;
        }
        else
        {
            status=1;
        }


    }
    else
    {
        printf("Heap leer,heap_delete_first\n");
        status=0;
    }

    return status;

}
/////////////////////////////////////////////////////////////////////////////////
long hash_key(char songtitle[],char interpreter[],long hashsize)
{
    unsigned long index = 0, i;
    for(i = 0; i < strlen(songtitle); ++i)
        index = 64 * index + (long)(songtitle[i]);
    for(i = 0; i < strlen(interpreter); ++i)
        index = 64 * index + (long)(interpreter[i]);
    return index % hashsize;
}
////////////////////////////////////////////////////////////////
hash_t *hash_create(long hashsize)
{
    hash_t *hash_new;
    if ((hash_new= calloc(1, sizeof(hash_t))) == 0)
    {
        printf("Speicher konnte nicht alloziert werden, hash_create\n");
        return (0);
    }


        if ((hash_new->hasharrays= calloc(hashsize, sizeof(hasharray_t))) == 0)
        {
            printf("Speicher konnte nicht alloziert werden, hash_create\n");
            return (0);
        }

    hash_new->hashsize = hashsize;
    return(hash_new);
}

/////////////////////////////////////////////////////////////////////////
void hash_insert(hash_t *hash,char songtitle[],char interpreter[])
{
    if(hash!=0)
    {


        long hashValue=hash_key(songtitle,interpreter,HASH_SIZE);
        if(hash->hasharrays[hashValue].entries==0)
        {
            if((hash->hasharrays[hashValue].entries=calloc(1,sizeof(entry_t)))==0)
            {
                printf("Allozieren der entries fehlgeschlagen,hash_insert\n");
                return;
            }

        }
        else
        {
            if((hash->hasharrays[hashValue].entries=realloc(hash->hasharrays[hashValue].entries,(hash->hasharrays[hashValue].num_entries+1)*sizeof(entry_t)))==0)
            {
                printf("Realozieren der entries fehlgeschlagen,hash_insert\n");
                return;
            }

        }

        hash->hasharrays[hashValue].num_entries++;
        long free_entry = hash->hasharrays[hashValue].num_entries-1;
        strcpy(hash->hasharrays[hashValue].entries[free_entry].songtitle,songtitle);
        strcpy(hash->hasharrays[hashValue].entries[free_entry].interpreter,interpreter);
    }
    else
    {
        printf("hash leer,hash_insert\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////
void hash_print(hash_t *hash)
{
    long n=0;
    long u=0;
    if(hash)
    {

        while(n<HASH_SIZE)
        {
            if(hash->hasharrays[n].num_entries!=0)
            {
                printf("%ld. %s, %s\n",n,hash->hasharrays[n].entries[u].songtitle,hash->hasharrays[n].entries[u].interpreter);
            }

            if(u<(hash->hasharrays[n].num_entries-1))
            {
                u++;
            }
            else
            {
                n++;
                u=0;
            }

        }
    }
    else
    {
        printf("hash leer,hash_print\n");
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void hash_free(hash_t *hash)
{
    long n=0;

    if(hash)
    {
        while(n<HASH_SIZE)
        {
            if(hash->hasharrays[n].num_entries!=0)
            {
                free(hash->hasharrays[n].entries);
            }
            n++;
        }
        free(hash);
    }
    else
    {
        printf("Hash leer, hash_free\n");
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
hash_t *convert_heap_to_hash(heap_t *heap)
{

    long n=1;
    hash_t* hash=hash_create(HASH_SIZE);

    while(n<(heap->next_free))
    {
        hash_insert(hash,heap->entries[n].songtitle,heap->entries[n].interpreter);
        n++;
    }
    return(hash);
}
/////////////////////////////////////////////////////////////////////////////////////////////////

entry_t *hash_search(hash_t *hash, char songtitle[], char interpreter[])
{
    entry_t *entry_found;
    int found=0;
    long n=0;
    if(hash)
    {

        long hash_value=hash_key(songtitle,interpreter,HASH_SIZE);
        while(n<(hash->hasharrays[hash_value].num_entries)&&(found==0))
        {
            if((strcmp((hash->hasharrays[hash_value].entries[n].songtitle),songtitle)==0)&&(strcmp((hash->hasharrays[hash_value].entries[n].interpreter),interpreter)==0))
            {
                found=1;
            }
            else
            {
                printf("n=%ld",n);
                n++;
            }
        }
        entry_found=&(hash->hasharrays[hash_value].entries[n]);
        if(found)
        {
            printf("found\n");
            return(entry_found);
        }
        else
        {
            printf("not found\n");
            return(0);
        }
    }
    else
    {
        printf("Hash leer, hash_search\n");
        return(0);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////

long hash_delete_entry(hash_t *hash,char songtitle[],char interpreter[])
{
    entry_t *new_entry;
    entry_t *entry_found=0;
    long u=0;
    long n=0;
    if(hash)
    {
        long hash_value=hash_key(songtitle,interpreter,HASH_SIZE);
        entry_found=hash_search(hash,songtitle,interpreter);
        if(entry_found)
        {

        if((new_entry=calloc((hash->hasharrays[hash_value].num_entries-1),sizeof(entry_t)))==0)
        {
            printf("Allozieren des neuen Feldes fehlgeschlagen,hash_delete_entry\n");
            return(0);
        }
        else
        {

            while(u<(hash->hasharrays[hash_value].num_entries))
            {
                if(&(hash->hasharrays[hash_value].entries[u])!=entry_found)
                {
                  strcpy(new_entry[n].songtitle,hash->hasharrays[hash_value].entries[u].songtitle);
                  strcpy(new_entry[n].interpreter,hash->hasharrays[hash_value].entries[u].interpreter);
                  n++;
                }

                u++;
            }
            hash->hasharrays[hash_value].num_entries--;
            free(hash->hasharrays[hash_value].entries);
        hash->hasharrays[hash_value].entries=new_entry;
         return(1);
        }
        }
        else
        {
            printf("not found\n");
            return(0);
        }

    }
    else
    {
        printf("Hash leer, hash_search\n");
        return(0);
    }

}




//////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    long auswahl=-1;
    long status=0;
    long index;
    heap_t *heap=heap_create(MEM_SIZE);
    char songtitle[255];
    char interpreter[255];

    hash_t *hash=hash_create(HASH_SIZE);
    entry_t *found_entry=0;

    while(auswahl!=0)
    {

        printf("0. Programm beenden\n");
        printf("1. Gesamten Heap loeschen\n");
        printf("2. Neuen Eintrag im Heap ablegen\n");
        printf("3. Heap ausgeben\n");
        printf("4. Eintrag im Heap suchen\n");
        printf("5. Ersten Eintrag aus dem Heap löschen\n");
        printf("6. Hash aus Heap erzeugen\n");
        printf("7. Gesamten Hash löschen\n");
        printf("8. Neuen Eintrag im Hash ablegen\n");
        printf("9. Hash ausgeben\n");
        printf("10. Eintrag im Hash suchen\n");
        printf("11. Eintrag aus dem Hash löschen\n");


        printf("Treffen Sie eine Wahl!\n");
        scanf("%ld",&auswahl);

        printf("heap_zeiger=%p\n",heap);
        getchar();

        switch(auswahl)
        {
        case(0):

            printf("Beenden.............\n");
            break;

        case(1):
            heap_free(heap);
            heap=0;
            break;

        case(2):
            read_from_keyboard(songtitle,interpreter);
            status=heap_insert(heap,songtitle,interpreter);
            printf("status=%ld\n",status);
            break;

        case(3):
            heap_print(heap,1,0);
            break;

        case(4):
            read_from_keyboard(songtitle,interpreter);
            index=heap_search(heap,songtitle,interpreter);
            printf("index=%ld\n",index);
            break;

        case(5):
            status=heap_delete_first(heap);
            printf("status=%ld\n",status);
            break;

        case(6):
            if(heap==0)
            {
                printf("heap zeiger null, case(6)");
                break;
            }

            if(hash)
            {
                hash_free(hash);
            }
            hash=convert_heap_to_hash(heap);
            break;

        case(7):
            hash_free(hash);
            hash=0;
            break;

        case(8):
            read_from_keyboard(songtitle,interpreter);
            hash_insert(hash,songtitle,interpreter);
            break;

        case(9):
            hash_print(hash);
            break;

        case(10):
            read_from_keyboard(songtitle,interpreter);
            found_entry=hash_search(hash,songtitle,interpreter);
            if(found_entry)
            {
                printf("\nKey:%ld, %s, %s\n\n",hash_key(songtitle,interpreter,HASH_SIZE),found_entry->songtitle,found_entry->interpreter);
                found_entry=0;
            }
            break;

        case(11):
            read_from_keyboard(songtitle,interpreter);
            status=hash_delete_entry(hash,songtitle,interpreter);
            printf("status=%ld\n",status);
            break;


        default:
            printf("Falsche Eingabe\n");
            break;
        }
        status=0;
    }
    return 0;
}
