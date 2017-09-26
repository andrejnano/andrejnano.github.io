
/* c201.c *********************************************************************}
{* T�ma: Jednosm�rn� line�rn� seznam
**
**                     N�vrh a referen�n� implementace: Petr P�ikryl, ��jen 1994
**                                          �pravy: Andrea N�mcov� listopad 1996
**                                                   Petr P�ikryl, listopad 1997
**                                P�epracovan� zad�n�: Petr P�ikryl, b�ezen 1998
**                                  P�epis do jazyka C: Martin Tu�ek, ��jen 2004
**	                                      �pravy: Bohuslav K�ena, ��jen 2014
**
** Implementujte abstraktn� datov� typ jednosm�rn� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je cel� ��slo typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou typu tList.
** Definici konstant a typ� naleznete v hlavi�kov�m souboru c201.h.
** 
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu s v��e
** uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ tList:
**
**      xxxInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      xxDisposeList ... zru�en� v�ech prvk� seznamu,
**      xxxInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      xxxFirst ......... nastaven� aktivity na prvn� prvek,
**      xxxCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      xxDeleteFirst ... zru�� prvn� prvek seznamu,
**      xxPostDelete .... ru�� prvek za aktivn�m prvkem,
**      xxPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      xxxCopy .......... vrac� hodnotu aktivn�ho prvku,
**      xxxActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      xxxSucc .......... posune aktivitu na dal�� prvek seznamu,
**      xxxActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci funkc� nevolejte ��dnou z funkc� implementovan�ch v r�mci
** tohoto p��kladu, nen�-li u dan� funkce explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* glob�ln� prom�nn� -- p��znak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/	
	L->Act = NULL;
	L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam L do stavu, v jak�m se nach�zel
** po inicializaci. Ve�ker� pam� pou��van� prvky seznamu L bude korektn�
** uvoln�na vol�n�m operace free.
***/
	while(L->First != NULL) //postupne rusenie vsetkych prvkov porade
	{
		if(L->First == L->Act) //ak narazim na aktualny zrusim aj ten
		{
			L->Act = NULL;
		}
		tElemPtr pomocny = L->First; //dame prvy prvok do pomocnej aby sme ho mohli uvolnit
		L->First = L->First->ptr; //druhy prvek sa stane prvym
		free(pomocny);
	}
}

void InsertFirst (tList *L, int val) {
/*
** Vlo�� prvek s hodnotou val na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci Error().
**/
	tElemPtr pomocny = malloc(sizeof(struct tElem));
	if(pomocny == NULL) // ak je chyba alokacie
	{
		Error();
	}
	else //inak vloz� prvek
	{
		pomocny->data = val; //nahrame data
		pomocny->ptr = L->First; //prvy prvok sa stava druhym
		L->First = pomocny; //a pomocny sa stane prvy
	}
}

void First (tList *L) {
/*
** Nastav� aktivitu seznamu L na jeho prvn� prvek.
** Funkci implementujte jako jedin� p��kaz, ani� byste testovali,
** zda je seznam L pr�zdn�.
**/	
 	L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci Error().
**/
	if(L->First != NULL) //ak nieco obsahuje
	{
		*val = L->First->data; //ulozim do val hodnotu
	}
	else //ak je prazdny nastane chyba
	{
		Error();	
	}
	
}

void DeleteFirst (tList *L) {
/*
** Zru�� prvn� prvek seznamu L a uvoln� j�m pou��vanou pam�.
** Pokud byl ru�en� prvek aktivn�, aktivita seznamu se ztr�c�.
** Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	if(L->First != NULL) //zoznam nieje prazdny
	{
		if(L->First == L->Act) //ak je aktivny tak aktivitu zrusi
		{
			L->Act = NULL;
		}
		tElemPtr pomocny = L->First; //dame prvy prvok do pomocnej aby sme ho mohli uvolnit
		L->First = L->First->ptr; //druhy prvek sa stane prvym
		free(pomocny);	
	}
}	

void PostDelete (tList *L) {
/* 
** Zru�� prvek seznamu L za aktivn�m prvkem a uvoln� j�m pou��vanou pam�.
** Pokud nen� seznam L aktivn� nebo pokud je aktivn� posledn� prvek seznamu L,
** nic se ned�je.
**/
	if(L->Act != NULL && L->Act->ptr != NULL) //zoznam je aktivny a aktivny neni posledny
	{
		tElemPtr pomocny = L->Act->ptr; //do pomocneho dam prvok za aktivnym aby som ho mohol uvolnit
		L->Act->ptr = L->Act->ptr->ptr; //2prvok za aktivnym bude 1 prvok za aktivnym 
		free(pomocny);	
	}
}

void PostInsert (tList *L, int val) {
/*
** Vlo�� prvek s hodnotou val za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je!
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** zavol� funkci Error().
**/
	if(L->Act != NULL) //zoznam je aktivny
	{
		tElemPtr pomocny = malloc(sizeof(struct tElem));
		if(pomocny == NULL) //nastala chyba pri alokacii
		{
			Error();
		}
		else //nenastala chyba pri alokacii
		{
			pomocny->data = val; //ulozim data
			pomocny->ptr = L->Act->ptr; //nasledujuci prvok aktivneho sa stane nasledujuci prvok pomocneho
			L->Act->ptr = pomocny; //pomocny sa stane nasledujuci aktivneho
		}
	}	
}

void Copy (tList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam nen� aktivn�, zavol� funkci Error().
**/
	if(L->Act != NULL) //ak je aktivny
	{
		*val = L->Act->data; //do val ulozime hodnotu aktualneho prvku
	}
	else //ak neni aktivny vola chybovu hlasku
	{
		Error();
	}	
}

void Actualize (tList *L, int val) {
/*
** P�ep�e data aktivn�ho prvku seznamu L hodnotou val.
** Pokud seznam L nen� aktivn�, ned�l� nic!
**/
	if(L->Act != NULL) //zoznam je aktivny
	{
		L->Act->data = val; //prepise data
	}	
}

void Succ (tList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** V�imn�te si, �e touto operac� se m��e aktivn� seznam st�t neaktivn�m.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if(L->Act != NULL) //zoznam je aktivny
	{
		if(L->Act->ptr != NULL)
		{
			L->Act = L->Act->ptr; //posunitie aktivity
		}
		else
		{
			L->Act = NULL; //ak bol aktivny posledny, aktivita sa straca
		}
	}	
}

int Active (tList *L) {		
/*
** Je-li seznam L aktivn�, vrac� TRUE. V opa�n�m p��pad� vrac� FALSE.
** Tuto funkci implementujte jako jedin� p��kaz return. 
**/
 	return(L->Act != NULL);	
}

/* Konec c201.c */
