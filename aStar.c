#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structure.h"

int estVideListe(L_SOMMET l)
{
	return !l;
}

L_SOMMET ajoutTete(L_SOMMET l, int k)
{
	L_SOMMET p;
	if ((p = (L_SOMMET)calloc(1, sizeof(*p))) == NULL)
		return NULL;

	p->val = k;
	p->suiv = l;
	return p;
}

L_SOMMET creerListe(void)
{
	return NULL;
}

int rechercheElement(L_SOMMET l, int s)
{
	L_SOMMET p = l;
	while ((!estVideListe(p)) && (p->val != s))
	{
		p = p->suiv;
	}
	if (!estVideListe(p))
		return 1;
	return 0;
}

/*

ajout � T_SOMMET :(( T_sommet.lf=1 c�d S est ds la liste ferm�e)) 
et ((T_sommet.indiceLO indice du sommet dans le tas(liste ouverte)))
T_sommet.indiceLO initialis� � -1 pour tous les sommets*
LO est un tas: changer les fct du tas tq le min est indice 0
lf est une liste, ajouter definition liste (visualiser liste/ 
recherche_element resultat booleen)
h re�oit les numeros de sommet 

//creer liste L_ARC
// fct recherche si le sommet est dans le tableau de sommet
*/
//#define INFINITY 99999

double heuristique(int s1, int s2, T_SOMMET *graphe)
{
	double h;
	double x1 = graphe[s1].x;
	double x2 = graphe[s2].x;
	double y1 = graphe[s1].y;
	double y2 = graphe[s2].y;
	h = (abs(x1 - x2) + abs(y1 - y2)) / 2;
	return (h);
}

// a et d numero de sommets demand�s � l'utilisateur

int minListe(double *f, L_SOMMET l)
{
	int min = f[l->val];
	L_SOMMET p;
	for (p = l; !estVideListe(p); p = p->suiv)
	{
		if (f[p->val] < min)
		{
			min = f[p->val];
		}
	}
	return min;
}
int *initTableauPeres(int *T, int nbsommet)
{
	int i;
	for (i = 0; i < nbsommet; i++)
	{
		T[i] = -1;
	}
}

L_SOMMET supprimerElement(L_SOMMET l, int s)
{
	L_SOMMET p, c;
	p = l;
	c = p;
	if (estVideListe(l)) // S'il y'a un element dans la liste ==> on n'a pas de p->suiv !!
		return l;

	while (!estVideListe(c) && (c->val != s))
	{
		p = c->suiv;
		c = p;
	}
	if (estVideListe(c))
	{
		return NULL;
	}
	p = c->suiv;
	free(c);
	return l;
}

void afficheListe(L_SOMMET l)
{
	L_SOMMET p;
	int i = 0;
	for (p = l; !estVideListe(p); p = p->suiv)
	{
		printf("liste[%d] = %d\n", i, p->val);
		i++;
	}
}
void afficheChemin(int d, int a, int *pere)
{
	int i = a;
	while (i != d)
	{
		printf("pere [%d] = %d\n", i, pere[i]);
		i = pere[i];
	}
}
int *pathfinder(int d, int a, T_SOMMET *graphe, int nbsommet)
{
	int k, s, i, w, jk;
	double *g = NULL, *f = NULL, *h = NULL;
	L_ARC p;
	int *pere = NULL;
	L_SOMMET LO, LF; //liste initialisation -1

	//int tlf = 0;
	g = calloc(nbsommet, sizeof(*g));
	f = calloc(nbsommet, sizeof(*f));
	h = calloc(nbsommet, sizeof(*h));
	pere = calloc(nbsommet, sizeof(*pere));
	//pere = initTableauPeres(pere, nbsommet);
	//fct ajout elt dans une liste
	//fct verifie que l'element n'est pas dans le tableau LF :int verifElement(int s, int* LF, int tlf)

	LO = calloc(1, sizeof(*LO));
	LF = calloc(1, sizeof(*LO));
	LO = creerListe();
	LF = creerListe();
	//Initialisation
	g[d] = 0;
	h[d] = heuristique(d, a, graphe);
	f[d] = g[d] + h[d];
	for (jk = 0; (jk < nbsommet); jk++)
	{
		if (jk != d)
		{
			g[jk] = INFINITY;
			h[jk] = INFINITY;
			f[jk] = INFINITY;
		}
	}

	//pere[d] = d;

	LO = ajoutTete(LO, d);
	printf("ajout tete de LO \n");
	afficheListe(LO);
	k = d;

	while ((!estVideListe(LO)) && (k != a))
	{
		k = minListe(f, LO);
		printf("k apres minListe = %d\n", k);

		if (k == a)
		{
			printf("on a atteint l'arrivee, et on a trouve le plus court chemin FIN \n");
		}
		else
		{
			LF = ajoutTete(LF, k);
			printf("ajout tete de LF\n");
			afficheListe(LF);
			LO = supprimerElement(LO, k);
			printf("LO supprime\n");
			afficheListe(LO);
			for (p = graphe[k].voisins; p != NULL; p = p->suiv)
			{
				s = p->val.arrivee;
				if (rechercheElement(LF, s) == 0)
				{
					if (rechercheElement(LO, s) == 0)
					{
						pere[s] = k;
						g[s] = g[k] + p->val.cout;
						h[s] = heuristique(s, a, graphe);
						f[s] = g[s] + h[s];
						LO = ajoutTete(LO, s);
						printf("Apres l'ajout des voisins dans LO \n");
						afficheListe(LO);
					}

					else
					{
						if ((g[k] + p->val.cout) < g[s])
						{
							pere[s] = k;
							g[s] = g[k] + p->val.cout;
							LO = supprimerElement(LO, s);
							printf("Apres suppression de l'element dans LO\n");

							afficheListe(LO);
							h[s] = heuristique(s, a, graphe);
							f[s] = g[s] + h[s];
							LO = ajoutTete(LO, s);
							printf("Apres l'ajout de l'element dans LO\n");
							afficheListe(LO);
						}
					}
				}
			}
		}
		free(g);
		free(f);
		free(h);
		return pere;
	}
}