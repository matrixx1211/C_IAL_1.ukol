
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error()
{
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init(DLList *list)
{
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList *list)
{
    if (list != NULL)                              //pokud není list prázdný
    {                                              //
        DLLElementPtr tmp;                         //vytvoření dočasných proměnných
        while (list->firstElement != NULL)         //zatímco je něco v listu
        {                                          //
            tmp = list->firstElement->nextElement; //do pom. prom. si uložím následující prvek
            free(list->firstElement);              //uvolním první prvek
            list->firstElement = tmp;              //do prvního prvku uložím následující prvek z pom. prom
        }                                          //
        list->activeElement = NULL;                //
        list->firstElement = NULL;                 //v podstatě funkce DLL_Init
        list->lastElement = NULL;                  //
    }                                              //
    else                                           //když je list prázdný
        return;                                    //ukončení funkce
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList *list, int data)
{
    DLLElementPtr tmp = malloc(sizeof(struct DLLElement)); //přidelení paměti pro nový prvek
    if (tmp == NULL)                                       //pokud se přidělení nepovede
    {                                                      //
        DLL_Error();                                       //zavolá se fuknce pro error
        return;                                            //ukončí se funkce
    }                                                      //
                                                           //
    tmp->data = data;                                      //nastavím si do pom. prom. data
    tmp->nextElement = list->firstElement;                 //nový první následující se musí nastavit na původní první
    tmp->previousElement = NULL;                           //nastavení předchozího u prvního na NULL
                                                           //
    if (list->firstElement != NULL)                        //pokud první prvek není null
        list->firstElement->previousElement = tmp;         //původní první musí nastavit předchozí na nový první
    else                                                   //když je null
        list->lastElement = tmp;                           //a zároveň i poslední
                                                           //
    list->firstElement = tmp;                              //na první položku se nastaví nový první
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList *list, int data)
{
    DLLElementPtr tmp = malloc(sizeof(struct DLLElement)); //přidelení paměti pro nový prvek
    if (tmp == NULL)                                       //pokud se přidělení nepovede
    {                                                      //
        DLL_Error();                                       //zavolá se fuknce pro error
        return;                                            //ukončí se funkce
    }                                                      //
                                                           //
    tmp->data = data;                                      //nastavím si do pom. prom. data
    tmp->previousElement = list->lastElement;              //u nového posledního nastavím předchozí na původní poslední
    tmp->nextElement = NULL;                               //nadcházející u posledního nastaven na NULL
                                                           //
    if (list->lastElement != NULL)                         //pokud poslední prvek není null
        list->lastElement->nextElement = tmp;              //u starého posledního nastavím nadcházející na nový poslední
    else                                                   //když je null
        list->firstElement = tmp;                          //nastavím tmp na první prvek
                                                           //
    list->lastElement = tmp;                               //nastavím starý poslední na nový poslední
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList *list)
{
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList *list)
{
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList *list, int *dataPtr)
{
    if (list == NULL || list->firstElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList *list, int *dataPtr)
{
    if (list == NULL || list->lastElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList *list)
{
    DLLElementPtr tmp;
    if (list->firstElement != NULL)            //pokud je nějaký první prvek
    {                                          //
        tmp = list->firstElement->nextElement; //do tmp nastavím nový první
        tmp->previousElement = NULL;           //do předchozího nastavím NULL (před 1. nic není)
        free(list->firstElement);              //uvolním paměť starého prvního prvku
        list->firstElement = tmp;              //do prvního prvku nastavím nový první
    }                                          //
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList *list)
{
    DLLElementPtr tmp;
    if (list->lastElement != NULL)                //pokud je nějaký poslední prvek
    {                                             //
        tmp = list->lastElement->previousElement; //do tmp nastavím nový poslední
        tmp->nextElement = NULL;                  //do následujícího nastavím NULL (po n. nic není)
        free(list->lastElement);                  //uvolním paměť starého posledního prvku
        list->lastElement = tmp;                  //do posledního prvku nastavím nový poslední
    }                                             //
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList *list)
{
    if (list->activeElement && list->activeElement != list->lastElement) //pokud je aktivní prvek a není posledním prvkem
    {                                                                    //
        DLLElementPtr tmp = list->activeElement->nextElement;            //pomocná proměnná pro prvek, který budu mazat
        list->activeElement->nextElement = tmp->nextElement;             //předělám závislost na nový předchozí
        if (tmp == list->lastElement)                                    //pokud je mazaný posledním prvkem
            list->lastElement = list->activeElement;                     //nastavím poslední prvek na aktivní
        else                                                             //
            tmp->nextElement->previousElement = list->activeElement;     //předělám závislost nadcházejícího na aktivní
        free(tmp);                                                       //uvolním paměť mazaného prvku
    }                                                                    //
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList *list)
{
    if (list->activeElement && list->activeElement != list->firstElement) //pokud je aktivní prvek a není prvním prvkem
    {                                                                     //
        DLLElementPtr tmp = list->activeElement->previousElement;         //pomocná proměnná pro prvek, který budu mazat
        list->activeElement->previousElement = tmp->previousElement;      //předělám závislost na nový předchozí
        if (tmp == list->firstElement)                                    //pokud je mazaný prvním prvkem
            list->firstElement = list->activeElement;                     //nastavím první prvek na aktivní
        else                                                              //
            tmp->previousElement->nextElement = list->activeElement;      //předělám závislost nadcházejícího na aktivní
        free(tmp);                                                        //uvolním paměť mazaného prvku
    }                                                                     //
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter(DLList *list, int data)
{

    if (list->activeElement)                                         //pokud je aktivní prvek
    {                                                                //
        DLLElementPtr tmp = malloc(sizeof(struct DLLElement));       //pomocná proměnná pro prvek, který budu přidávat
        if (tmp == NULL)                                             //pokud se nepovede malloc
        {                                                            //
            DLL_Error();                                             //výpis chyby
            return;                                                  //ukončí funkci
        }                                                            //
        tmp->nextElement = list->activeElement->nextElement;         //předělání závisloti
        tmp->previousElement = list->activeElement;                  //předělání závisloti
        tmp->data = data;                                            //nastavení dat
        if (list->activeElement == list->lastElement)                //pokud je vkládaný element poslední
            list->lastElement = tmp;                                 //do posledního vložím nový
        else                                                         //
            list->activeElement->nextElement->previousElement = tmp; //v následujícím předchozí bude nový
        list->activeElement->nextElement = tmp;                      //vložení nového prvku za aktivní
    }                                                                //
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore(DLList *list, int data)
{
    if (list->activeElement)                                         //pokud je aktivní prvek
    {                                                                //
        DLLElementPtr tmp = malloc(sizeof(struct DLLElement));       //pomocná proměnná pro prvek, který budu přidávat
        if (tmp == NULL)                                             //pokud se nepovede malloc
        {                                                            //
            DLL_Error();                                             //výpis chyby
            return;                                                  //ukončí funkci
        }                                                            //
        tmp->nextElement = list->activeElement;                      //předělání závisloti
        tmp->previousElement = list->activeElement->previousElement; //předělání závisloti
        tmp->data = data;                                            //nastavení dat
        if (list->activeElement == list->firstElement)               //pokud je vkládaný element první
            list->firstElement = tmp;                                //do prvního vložím nový
        else                                                         //
            list->activeElement->previousElement->nextElement = tmp; //v předchozím následující bude nový
        list->activeElement->previousElement = tmp;                  //vložení nového prvku před aktivní
    }                                                                //
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList *list, int *dataPtr)
{
    if (list->activeElement == NULL)          //pokud není žádný aktivní prvek
        DLL_Error();                          //vyhodí error
    else                                      //když je vybraný aktivní prvek
        *dataPtr = list->activeElement->data; //uloží se hodnota data do dataPtr
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList *list, int data)
{
    if (list->activeElement != NULL)      //pokud je nějaký aktivní prvek
        list->activeElement->data = data; //přepíšu jeho data
    else                                  //jinak
        return;                           //ukončím funkci
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList *list)
{
    list->activeElement = list->activeElement->nextElement;
}

/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList *list)
{
    list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive(DLList *list)
{
    return list->activeElement == NULL ? 0 : 1;
}

/* Konec c206.c */
