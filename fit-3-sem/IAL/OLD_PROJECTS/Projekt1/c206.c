	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2014
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      xxxDLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      xxxDLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      xxxDLInsertLast .... vlo�en� prvku na konec seznamu, 
**      xxxDLFirst ......... nastaven� aktivity na prvn� prvek,
**      xxxDLLast .......... nastaven� aktivity na posledn� prvek, 
**      xxxDLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      xxxDLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      xxxDLDeleteFirst ... zru�� prvn� prvek seznamu,
**      xxxDLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      xxxDLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      xxxDLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      xxxDLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      xxxDLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      xxxDLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      xxxDLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      xxxDLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
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

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/
	while(L->First != NULL) //prejde vsetky prvky zoznamu
	{
		if(L->First == L->Act) //ak je ruseny prvok aktivny, tak zrusi aktivitu 
		{
			L->Act = NULL;
		}
		if(L->First == L->Last) //ak je prvy zaroven poslednym tak zrusi ukazovanie na posledny
		{
			L->Last = NULL;
		}
		tDLElemPtr pomocny = L->First; //dame prvy do pomocnej aby sme ho mohli uvolnit
		L->First = L->First->rptr; //prvok napravo od prveho sa stava prvym
		free(pomocny);
	}
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	tDLElemPtr pomocny = malloc(sizeof(struct tDLElem));
	if(pomocny == NULL) //chyba alokacie
	{
		DLError();
	}
	else //alokacia prebehla spravne
	{
		pomocny->data = val; //nahrame data
		pomocny->lptr = NULL; //prvok nalavo od pomocneho bude ukazovat na null
		pomocny->rptr = L->First; //prvok napravo od pomocneho bude ukazovat na prvy prvok
		if(L->Last == NULL) //ak to je prve vlozenie
		{
			L->Last = pomocny; //posledny je nas vlozeny prvok
		}
		else //nieje prve vlozenie
		{
			L->First->lptr = pomocny; //prvok nalavo prveho bude ukazovat na pomocny
		}
		L->First = pomocny; //prvy je nas vlozeny prvok
	}	
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/ 	
	tDLElemPtr pomocny = malloc(sizeof(struct tDLElem));
	if(pomocny == NULL) //chyba alokacie
	{
		DLError();
	}
	else //bezchybna alokacia
	{
		pomocny->data = val; //ulozime data
		pomocny->rptr = NULL; //napravo od pomocneho nebude ukazovat nikam lebo pomocny bude posledny
		pomocny->lptr = L->Last; //nalavo pomocneho bude ukazovat na posledny prvok
		if(L->Last == NULL) //ak vkladame prvy prvok
		{
			L->First = pomocny; //posledny je zaroven prvy teda nas pomocny
		}
		else //nevkladame prvy prvok
		{
			L->Last->rptr = pomocny; //napravo od posledneho bude ukazovat na nas pomocny, pomocny sa stava posledny
		}
		L->Last = pomocny; //pomocny je posledny
	}
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->Last;	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if(L->First == NULL) //zoznam je prazdny
	{
		DLError();
	}
	else //zoznam neni prazdny
	{
		*val = L->First->data;
	}	
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if(L->First == NULL) //zoznam je prazdny
	{
		DLError();
	}
	else //zoznam neni prazdny
	{
		*val = L->Last->data;
	}	
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	if(L->First != NULL) //zoznam nebol prazdny
	{
		tDLElemPtr pomocny = L->First; //dame si prvy do pomocnej nech ho mozme uvolnit neskor
		if(L->First == L->Last) //ak bol prvy zaroven posledny tak nastavime aktivitu a posledny tiez na nul
		{
			L->First = NULL;
			L->Last = NULL;
			L->Act = NULL;
		}
		else
		{
			if(L->First == L->Act) //ak bol prvy aktivny, zrusime aktivitu tiez
			{
				L->Act = NULL;
			}
			L->First = pomocny->rptr; //prvym sa stava prvok napravo od prveho
			L->First->lptr = NULL; //novy prvy prvok bude ukzovat nalavo na null
		}
	free(pomocny);
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/ 
	if(L->First != NULL)
	{
		tDLElemPtr pomocny = L->Last;
		if(L->First == L->Last) //ak bol prvy zaroven posledny tak nastavime aktivitu a posledny tiez na nul
		{
			L->First = NULL;
			L->Last = NULL;
			L->Act = NULL;
		}
		else
		{
			if(L->Last == L->Act) //ak bol posledny aktivny, zrusime aktivitu tiez
			{
				L->Act = NULL;
			}
			L->Last = pomocny->lptr; //posledny sa stava prvok nalavo od posleneho 
			L->Last->rptr = NULL; //novy posledny bude napravo ukazovat na null
		}
	free(pomocny);
	}	
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/
	if(L->Act != NULL && L->Act != L->Last) //zoznam je aktivny a aktivny neni posledny
	{
		tDLElemPtr pomocny = L->Act->rptr;
		L->Act->rptr = pomocny->rptr; //preskocime ruseny prvok
		if(L->Last == pomocny) //ak je ruseny prvok posledny
		{
			L->Last = L->Act;
		}
		else //ak neni posledny
		{
			pomocny->rptr->lptr = L->Act; //preskocime ruseny prvok
		}
		free(pomocny);
	}	
}

void DLPreDelete (tDLList *L) {
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/
	if(L->Act != NULL && L->Act != L->First) ///zoznam je aktivny a aktivny neni posledny
	{
		tDLElemPtr pomocny = L->Act->lptr;
		L->Act->lptr = pomocny->lptr; //preskocime ruseny prvok
		if(L->First == pomocny) //ak je ruseny prvok prvy
		{
			L->First = L->Act;
		}
		else //ak neni prvy
		{
			pomocny->lptr->rptr = L->Act; //preskocime ruseny prvok
		}
		free(pomocny);
	}			
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if(L->Act != NULL) //zoznam je aktivny
	{	
		tDLElemPtr pomocny = malloc(sizeof(struct tDLElem));
		if(pomocny == NULL) //malloc sa nepodaril
		{
			DLError();
		}
		else //malloc sa podaril tak napojime novy prvok
		{
			pomocny->data = val;
			pomocny->lptr = L->Act;
			pomocny->rptr = L->Act->rptr;
			L->Act->rptr = pomocny;
			if(L->Act != L->Last) //ak aktivny prvok nebol posledny
			{
				pomocny->rptr->lptr = pomocny;
			}
			else //ak bol posledny
			{
				L->Last = pomocny;
			}
		}
	}	
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if(L->Act != NULL) //zoznam je aktivny
	{	
		tDLElemPtr pomocny = malloc(sizeof(struct tDLElem));
		if(pomocny == NULL) //malloc sa nepodaril
		{
			DLError();
		}
		else //malloc sa podaril tak napojime novy prvok
		{
			pomocny->data = val;
			pomocny->rptr = L->Act;
			pomocny->lptr = L->Act->lptr;
			L->Act->lptr = pomocny;
			if(L->Act != L->First) //ak aktivny nebol prvy
			{
				pomocny->lptr->rptr = pomocny;
			}
			else //ak bol prvy
			{
				L->First = pomocny;
			}
		}
	}	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
	if(L->Act == NULL) //neni aktivny
	{
		DLError();
	}
	else //je aktivny
	{
		*val = L->Act->data;
	}	
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if(L->Act != NULL)
	{
		L->Act->data = val;
	}	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
	if(L->Act != NULL) //zoznam je aktivny
	{
		if(L->Act == L->Last) //ak je aktivny posledny, aktivita sa straca
		{
			L->Act = NULL;
		}
		else //inak aktivitu posuneme doprava
		{
			L->Act = L->Act->rptr;
		}
	}	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	if(L->Act != NULL) //zoznam je aktivny
	{
		if(L->Act == L->First) //ak je aktivny prvy, aktivita sa straca
		{
			L->Act = NULL;
		}
		else //inak aktivitu posuneme dolava
		{
			L->Act = L->Act->lptr;
		}
	}	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/
	return(L->Act != NULL);
}

/* Konec c206.c*/
