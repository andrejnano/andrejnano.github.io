
/* c016.c: **********************************************************}
{* T�ma:  Tabulka s Rozpt�len�mi Polo�kami
**                      Prvn� implementace: Petr P�ikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      �pravy: Karel Masa��k, ��jen 2014
**                      �pravy: Radek Hranick�, ��jen 2014
**
** Vytvo�ete abstraktn� datov� typ
** TRP (Tabulka s Rozpt�len�mi Polo�kami = Hash table)
** s explicitn� �et�zen�mi synonymy. Tabulka je implementov�na polem
** line�rn�ch seznam� synonym.
**
** Implementujte n�sleduj�c� procedury a funkce.
**
**  xxxHTInit ....... inicializuje tabulku p�ed prvn�m pou�it�m
**  HTInsert ..... vlo�en� prvku
**  HTSearch ..... zji�t�n� p��tomnosti prvku v tabulce
**  HTDelete ..... zru�en� prvku
**  HTRead ....... p�e�ten� hodnoty prvku
**  HTClearAll ... zru�en� obsahu cel� tabulky (inicializace tabulky
**                 pot�, co ji� byla pou�ita)
**
** Definici typ� naleznete v souboru c016.h.
**
** Tabulka je reprezentov�na datovou strukturou typu tHTable,
** kter� se skl�d� z ukazatel� na polo�ky, je� obsahuj� slo�ky
** kl��e 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal�� synonymum 'ptrnext'. P�i implementaci funkc�
** uva�ujte maxim�ln� rozm�r pole HTSIZE.
**
** U v�ech procedur vyu��vejte rozptylovou funkci hashCode.  Pov�imn�te si
** zp�sobu p�ed�v�n� parametr� a zamyslete se nad t�m, zda je mo�n� parametry
** p�ed�vat jin�m zp�sobem (hodnotou/odkazem) a v p��pad�, �e jsou ob�
** mo�nosti funk�n� p��pustn�, jak� jsou v�hody �i nev�hody toho �i onoho
** zp�sobu.
**
** V p��kladech jsou pou�ity polo�ky, kde kl��em je �et�zec, ke kter�mu
** je p�id�n obsah - re�ln� ��slo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovac� funkce - jej�m �kolem je zpracovat zadan� kl�� a p�id�lit
** mu index v rozmez� 0..HTSize-1.  V ide�ln�m p��pad� by m�lo doj�t
** k rovnom�rn�mu rozpt�len� t�chto kl��� po cel� tabulce.  V r�mci
** pokus� se m��ete zamyslet nad kvalitou t�to funkce.  (Funkce nebyla
** volena s ohledem na maxim�ln� kvalitu v�sledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitn� z�et�zen�mi synonymy.  Tato procedura
** se vol� pouze p�ed prvn�m pou�it�m tabulky.
*/

void htInit ( tHTable* ptrht ) {	
	for(int i = 0; i < HTSIZE; i++) //vsetky prvky tabullky budu ukazovat na NULL
	{
		(*ptrht)[i] = NULL;
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitn� z�et�zen�mi synonymy.
** Vyhled�n� prvku v TRP ptrht podle zadan�ho kl��e key.  Pokud je
** dan� prvek nalezen, vrac� se ukazatel na dan� prvek. Pokud prvek nalezen nen�, 
** vrac� se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	if(ptrht != NULL) //ak tabulka existuje
	{
		int index = hashCode(key); //najdeme index, pomocou hashovacej funkcie 
		tHTItem *pomocna = (*ptrht)[index]; //a vytvorime pomocnu premennu s indexom
		while(pomocna != NULL)
		{
			if(!strcmp(pomocna->key, key)) //ak sa kluc zhoduje s miestom v tabulke tak vrati ze naslo a kde
			{
				return pomocna;
			}
			pomocna = pomocna->ptrnext; //posune sa v zozname doprava na dalsie synonimum
		}
		return NULL; //nenaslo
	}
	return NULL; //neobsahuje
}

/* 
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vkl�d� do tabulky ptrht polo�ku s kl��em key a s daty
** data.  Proto�e jde o vyhled�vac� tabulku, nem��e b�t prvek se stejn�m
** kl��em ulo�en v tabulce v�ce ne� jedenkr�t.  Pokud se vkl�d� prvek,
** jeho� kl�� se ji� v tabulce nach�z�, aktualizujte jeho datovou ��st.
**
** Vyu�ijte d��ve vytvo�enou funkci htSearch.  P�i vkl�d�n� nov�ho
** prvku do seznamu synonym pou�ijte co nejefektivn�j�� zp�sob,
** tedy prove�te.vlo�en� prvku na za��tek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	if(ptrht != NULL) //ak tabulka existuje
	{
		tHTItem *pomocna = htSearch(ptrht, key); //zistime ci neexistuje v tabulke polozka s totoznym klucom
		if(pomocna != NULL)
		{
			pomocna->data = data; //ak sa nachadza, zaktualizujeme iba data
		}
		else //neexistuje polozka s rovnakym klucom
		{
			pomocna = malloc(sizeof(tHTItem)); //naalokujeme miesto pre polozku
			if(pomocna == NULL) //alokacia sa nepodarila //asi zmazem este...
			{
				return;
			}
			else //podarila sa alokacia
			{
				int dlzka = strlen(key); //zistime si dlzku pre kluc
				int index = hashCode(key); //zistime si index kde budeme ukladat
				pomocna->key = malloc(dlzka + 1); //naalokujeme miesto pre kluc
				if(pomocna->key == NULL) //alokacia sa nepodarila //asi zmazem este...
				{
					free(pomocna);
				}
				else //podarila sa alokacia
				{
					strcpy(pomocna->key, key); //nakopirujeme kluc do novej polozky
					pomocna->data = data; //nakopirujeme data
					pomocna->ptrnext = (*ptrht)[index]; //previazeme
					(*ptrht)[index] = pomocna; //dame na prve miesto
				}
			}
		}
	}
}

/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato funkce zji��uje hodnotu datov� ��sti polo�ky zadan� kl��em.
** Pokud je polo�ka nalezena, vrac� funkce ukazatel na polo�ku
** Pokud polo�ka nalezena nebyla, vrac� se funk�n� hodnota NULL
**
** Vyu�ijte d��ve vytvo�enou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	if(ptrht != NULL) //ak existuje
	{
		tHTItem *pomocna = htSearch(ptrht, key); //vyhladame polozku
		if(pomocna != NULL) //ak sa polozka nachadza v tabulke 
		{
			return &(pomocna->data); //vratime ju
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vyjme polo�ku s kl��em key z tabulky
** ptrht.  Uvoln�nou polo�ku korektn� zru�te.  Pokud polo�ka s uveden�m
** kl��em neexistuje, d�lejte, jako kdyby se nic nestalo (tj. ned�lejte
** nic).
**
** V tomto p��pad� NEVYU��VEJTE d��ve vytvo�enou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	if(ptrht != NULL) //ak existuje tabulka
	{
		int index = hashCode(key); //zistime si index, kde sa nachadza polozka, ktoru chceme zrusit
		tHTItem *pomocna = (*ptrht)[index];
		tHTItem *pomocna2 = (*ptrht)[index];
		if(pomocna == NULL) //ak je index tabulky prazdny
		{
			return;
		}
		if(!strcmp(pomocna->key, key)) //nasli sme polozku na prom mieste v tabulke
			{
				(*ptrht)[index] = pomocna->ptrnext; //dalsia bude prva 
				free(pomocna->key);
				free(pomocna);
				return;
			}
		while(pomocna->ptrnext != NULL)	//ak nebola na prvom mieste musime prechadzat vsetky synonima
		{
			pomocna2 = pomocna; //aby sme nestratili ukazatel
			pomocna = pomocna->ptrnext; //dalsi prvok
			if(!strcmp(pomocna->key, key)) //uz me ju nasli
			{
				pomocna2->ptrnext = pomocna->ptrnext; //naviazeme na ob polozku
				free(pomocna->key);
				free(pomocna);
				return;
			}
			pomocna2 = pomocna; 	
		}
	}
}

/* TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura zru�� v�echny polo�ky tabulky, korektn� uvoln� prostor,
** kter� tyto polo�ky zab�raly, a uvede tabulku do po��te�n�ho stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	if(ptrht != NULL) //ak existuje tabulka
	{
		tHTItem *pomocna;
		tHTItem *pomocna2; //pomocna aby sme nestratili ukazatel po uvolneni
		for(int i = 0; i < HTSIZE; i++) //budeme prechadzat v tabulke
		{
			pomocna = (*ptrht)[i];
			while(pomocna != NULL) //a tiez budeme prechadzat vsetky synonyma
			{
				pomocna2 = pomocna->ptrnext; //posuneme sa 
				free(pomocna->key);
				free(pomocna);
				pomocna = pomocna2;
			}
			(*ptrht)[i] = NULL; //nakoniec uvolnime polozku pola tabulky
		}
	}
}
