/*
Daryl Martin--Dipp
Elias Ailem
Sprint 4r
*/

// Librairies ------------------------------------------
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes -----------------------------------------
#define LGMOT 35
#define NBCHIFFREMAX 5
#define MAX_SPECIALITES 10
#define MAX_TRAVAILLEURS 50
#define MAX_CLIENTS 100
#define MAX_COMMANDES 500

// Messages -------------------------------------------
#define MSG_DEVELOPPE "## nouvelle specialite \"%s\" ; cout horaire \"%d\"\n"
#define MSG_INTERRUPTION "## fin de programme\n"
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n"
#define MSG_DEMARCHE "## nouveau client \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d'heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PROGRESSION_PASSE "## une reallocation est requise\n"
#define MSG_SPECIALITES "## consultation des specialites\n"
#define MSG_TRAVAILLEURS "## consultation des travailleurs competents pour la specialite \"%s\"\n"
#define MSG_TRAVAILLEURS_TOUS "## consultation des travailleurs competents pour chaque specialite\n"
#define MSG_CLIENT "## consultation des commandes effectuees par \"%s\"\n"
#define MSG_CLIENT_TOUS "## consultation des commandes effectuees par chaque client\n"
#define MSG_SUPERVISION "## consultation de l'avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"

// Type BOOL ------------------------------------------
// Définition du type booléen
typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// Fonctions de Base ----------------------------------
// Définition des fonctions de bases

// Fonction permettant de lire les mots écrits au clavier
typedef char Mot[LGMOT + 1];
void get_id(Mot id) {
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id);
}

// Fonction permettant de lire les nombres écrits au clavier
int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer);
	return atoi(buffer);
}

// Structures -----------------------------------------

// Type SP(S) -----------------------------------------
typedef struct {
	Mot nom;
	int cout_horaires;
} Specialite;

typedef struct {
	Specialite tab_specialites[MAX_SPECIALITES];
	unsigned int nb_specialites;
} Specialites;

// Type TRV(S) -----------------------------------------
typedef struct {
	Mot nom_cmd;
	int duree;
	Mot specialite;
} Tache_travailleur;

typedef struct {
	Mot nom;
	Booleen tags_competences[MAX_SPECIALITES];
	Tache_travailleur tab_taches_travailleur[MAX_COMMANDES];
	int nb_cmd_trv;
	unsigned int nb_duree_trv;	
} Travailleur;

typedef struct {
	Travailleur tab_travailleurs[MAX_TRAVAILLEURS];
	unsigned int nb_travailleurs;
} Travailleurs;

// Type CL(S) -----------------------------------------
typedef struct {
	Mot nom;
	int idx_client;
} Client;

typedef struct {
	Client tab_clients[MAX_CLIENTS];
	unsigned int nb_clients;
} Clients;

// Type TCH --------------------------------------------
typedef struct {
	unsigned int nb_heures_requises;
	unsigned int nb_heures_effectuees;
} Tache;

// Type CMD(S) -----------------------------------------
typedef struct {
	Mot nom;
	unsigned int idx_client;
	Tache taches_par_specialite[MAX_SPECIALITES]; // nb_heures_requises == 0 <=> pas de tache pour cette specialite
} Commande;

typedef struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
} Commandes;

// Variables ------------------------------------------
Specialite sp;
Specialites sps;
Travailleur trv;
Travailleurs tvs;
Tache tch;
Commande cmd;
Commandes cms;
Client cl;
Clients cls;

// Prototypes -----------------------------------------

int rech_specialite(Mot nom_sp);
int rech_idx_client(Mot nom_cl);
void affiche_travailleurs_tous();
void affiche_travailleurs(Mot nom_specialite);
void affiche_client_tous();
void affiche_client(Mot nom_client);
int rech_commande (Mot nom_cmd);
int nouveau_travailleur(int nom_tvr);
void affiche_supervision(int num_commande);

// Liste des fonctions utilisées dans le programme

/*
Fonction developpe --------------------------------------------------------------------------------------
Cette fonction sert à créer une nouvelle spécialité

On rentre le nom de la spécialité et son coût horaire dans la structure sp
On copie la structure sp dans la structure sps
Rajoute 1 au nombre de spécialité (sps.nb_specialites += 1)
*/

void traite_developpe() {
	Mot nom_specialite;
	get_id(nom_specialite);
	int cout_horaire = get_int();

	strcpy(sp.nom, nom_specialite);
	sp.cout_horaires = cout_horaire;

	sps.tab_specialites[sps.nb_specialites] = sp;
	sps.nb_specialites += 1;
}

// Fonction embauche ------------------------------------------------------------------------------------

/*
Cette fonction permet d'embaucher un nouveau travailleur, c'est-à-dire d'ajouter un nouveau travailleur dans la structure tvs
Mais elle permet aussi d'assigner un travailleur à une nouvelle tache

Recherche si le nom_travailleur entré existe déja (nouveau_travailleur) :
Renvoie son rang trouvé dans la structure tvs si c'est le cas, renvoie -1 si ce n'est pas le cas
Recherche le rang de la spécialité entrée (rech_specialite) :
Renvoie le rang de la spécialité trouvée dans la structure sps

Si le travailleur existe, on met à VRAI son nouveau tag de compétence dans la structure tvs (if)
Sinon, on crée un nouveau travailleur :
on copie son nom dans trv, on met tous les tags de compétences à FAUX, puis on met à vrai son tag de compétence
On copie ensuite ses informations dans tvs, et on augmente de 1 le nombre de travailleur (tvs.nb_travailleurs += 1)
*/

void traite_embauche() {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	Mot nom_specialite;
	get_id(nom_specialite);
	
	int nouveau = nouveau_travailleur(nom_travailleur);
	int rech = rech_specialite(nom_specialite);
	int i = 0;
	
	if (nouveau != -1) {
		tvs.tab_travailleurs[nouveau].tags_competences[rech] = VRAI;
	}
	else {
		strcpy(trv.nom, nom_travailleur);
		while (i < MAX_SPECIALITES) {
			trv.tags_competences[i] = FAUX;
			i += 1;
		}
		trv.tags_competences[rech] = VRAI;
		tvs.tab_travailleurs[tvs.nb_travailleurs] = trv;
		tvs.nb_travailleurs += 1;
	}
}

/*
Fonction nouveau_travailleur ----------------------------------------------------------------------------
Recherche si le travailleur existe déjà ou pas, en comparant le nom du travailleur entré à tous les noms de travailleurs existants
Renvoie son rang trouvé dans la structure tvs si c'est le cas, renvoie -1 si ce n'est pas le cas
*/

int nouveau_travailleur(int nom_trv) {
	int i = 0;
	while (i < tvs.nb_travailleurs) {
		if (strcmp(nom_trv, tvs.tab_travailleurs[i].nom) == 0) {
			return i;
		}
		else {
			i += 1;
		}
	}
	return -1;
}

/*
Fonction rech_specialite --------------------------------------------------------------------------------
Recherche si la specialite fait partie de sps, en comparant le nom de la spécialité entré à tous les noms de spécialités existantes
Renvoie la valeur de la specialite
*/

int rech_specialite(Mot nom_sp) {
	int i = 0;
	while (i < sps.nb_specialites) {
		if (strcmp(sps.tab_specialites[i].nom, nom_sp) == 0) {
			return i;
		}
		else {
			i += 1;
		}
	}
	//return -1;
}

// J'avais mis le return -1 au cas où l'utilisateur rentrait une spécialité inconnue, mais je l'ai enlevé car on m'a dit que cela n'était pas utile

/*
Fonction demarche ---------------------------------------------------------------------------------------
Cette fonction permet de démarcher un nouveau client, c'est-à-dire d'ajouter un nouveau client dans la structure cls

Rentre le nom du client dans la structure cl
La ligne "cl.idx_client = cls.nb_clients + 1" sert à créer l'idx du client, en rajoutant 1 à son rang, c'est-à-dire le nombre de client avant lui
Par exemple, le premier client (rang 0) aura l'idx 1

Copie des informations dans la structure cls
Nombre de client augmenté de 1

Les deux printf permettaient de vérifier le bon fonctionnement du programme
*/

void traite_demarche() {
	Mot nom_client;
	get_id(nom_client);

	strcpy(cl.nom, nom_client);
	cl.idx_client = cls.nb_clients + 1;

	cls.tab_clients[cls.nb_clients] = cl;
    cls.nb_clients += 1;
	//printf("CLIENT = %s\n", cls.tab_clients[cls.nb_clients - 1].nom);
	//printf("ID CLIENT = %d\n", cls.tab_clients[cls.nb_clients - 1].idx_client);
}

/*
Fonction commande ---------------------------------------------------------------------------------------
Cette fonction permet de créer une nouvelle commande, c'est-à-dire d'ajouter une nouvelle commande dans la structure cms

Rentre les informations dans la structure cmd
Renvoie l'idx du client grâce au nom du client rentré (rech_idx_client), puis le rentre dans la structure cmd
Le while initialise le tableau taches_par_specialite à 0, pour les nombres d'heures requises et effectuées
Copie de la structure cmd dans cms
Rajoute 1 au nombre de commandes

Les trois printf permettaient de vérifier le bon fonctionnement du programme
*/

void traite_commande() {
	Mot nom_commande;
	get_id(nom_commande);
	Mot nom_client;
	get_id(nom_client);

	strcpy(cmd.nom, nom_commande);
    int rech = rech_idx_client(nom_client);
	//printf("%d\n", rech);
	cmd.idx_client = rech;

	int i = 0;
	while (i < MAX_SPECIALITES) {
		cmd.taches_par_specialite[i].nb_heures_requises = 0;
		cmd.taches_par_specialite[i].nb_heures_effectuees = 0;
		i += 1;
	}

	cms.tab_commandes[cms.nb_commandes] = cmd;
	cms.nb_commandes += 1;

	//printf("NOM COMMANDE : %s\n", cms.tab_commandes[cms.nb_commandes - 1].nom);
	//printf("ID CLIENT : %d\n", cms.tab_commandes[cms.nb_commandes - 1].idx_client);
	//printf("NB COMMANDE %d\n", cms.nb_commandes);
}

/*
Fonction rech_idx_client --------------------------------------------------------------------------------
Permet de renvoyer l'idx du client

On rentre le nom du client
La fonction compare ce nom à tous les noms de la table cls
Quand le nom est trouvé, la fonction renvoie l'idx correspondant (return cls.tab_clients[i].idx_client)

Les printf permettaient de vérifier le bon fonctionnement du programme
*/

int rech_idx_client(Mot nom_cl) {
	int i = 0;
	//printf("On est la: cls.nb_clients : %d\n", cls.nb_clients);
	while (i < cls.nb_clients) {
		//printf("cls.tab_clients[i].nom: %s\n", cls.tab_clients[i].nom);
		//printf("cls.tab_clients[i].idx_client: %d\n", cls.tab_clients[i].idx_client);
		if (strcmp(cls.tab_clients[i].nom, nom_cl) == 0) {
			//printf("Vrai\n");
			return cls.tab_clients[i].idx_client;
		}
		else {
			//printf("FAUX\n");
			i += 1;
		}
	}
}

/*
Fonction tache ------------------------------------------------------------------------------------------
Cette fonction permet d'assigner un travailleur à une commande
Le travailleur choisi est celui ayant la bonne spécialité et le moins de durée de travail

Création d'un tableau compare_duree_trv, qui permet de stocker le temps de travail des travailleurs qui ont la spécialité entrée
Le tableau est de taille MAX_TRAVAILLEURS, pour être sur de pouvoir stocker toutes les durées des travailleurs dedans
Initialisation du tableau à MAX_COMMANDES + 1, pour être sûr que toutes les cases de ce tableau ont une valeur très élevée, afin de faciliter la comparaison suivante

rech_commande renvoie le rang de la commande entrée
rech_specialite renvoie le rang de la spécialité entrée
Le nombre d'heure entrée par l'utilisateur est mise dans le tableau taches_par_specialite, à la ligne de la spécialité entrée, dans la colonne nb_heures_requises, --
-- pour la commande rentrée, dans la structure cms

Les printf permettaient de vérifier le bon fonctionnement du programme
*/

void traite_tache() {
	Mot nom_commande;
	get_id(nom_commande);
	Mot nom_specialite;
	get_id(nom_specialite);
	int nombre_heure = get_int();
	
	int i = 0;
	
	int compare_duree_trv[MAX_TRAVAILLEURS] = {0};
	for (i = 0 ; i < MAX_TRAVAILLEURS ; i++)
    {
        compare_duree_trv[i] = MAX_COMMANDES + 1;
    }
		
	int commande = rech_commande(nom_commande);
	int specialite = rech_specialite(nom_specialite);
	cms.tab_commandes[commande].taches_par_specialite[specialite].nb_heures_requises = nombre_heure;
	//printf("Commande : %d\n", commande);
	//printf("specialite : %d\n", specialite);
	//printf("Nb heure : %d\n", cms.tab_commandes[commande].taches_par_specialite[specialite].nb_heures_requises);
	
/*
Remplissage du tableau tvs.tab_taches_travailleur
Le premier while sert à compléter le tableau compare_duree_trv avec le temps de travail des travailleurs qui ont la spécialité entrée
Le deuxième while sert à comparer toutes les valeurs du tableau compare_duree_trv, afin de ressortir la plus petite durée (a) et le rang du travailleur (b)

Les printf permettaient de vérifier le bon fonctionnement du programme
*/
	
	i = 0; // remise du i à 0 pour le réutiliser
	while (i < tvs.nb_travailleurs) {
		if (tvs.tab_travailleurs[i].tags_competences[specialite] == VRAI) {
			compare_duree_trv[i] = tvs.tab_travailleurs[i].nb_duree_trv;
		}
		i += 1;
	}
	
	//printf("NUM 3 : %d\n", compare_duree_trv[3]);
	i = 0; // remise du i à 0 pour le réutiliser
	int a = compare_duree_trv[i]; // Le a prend la valeur de la première valeur du tableau
	//printf("NB TACHE AVANT = %d\n", a);

	int b = 0;
	while (i < tvs.nb_travailleurs) {
		if (a > compare_duree_trv[i+1]) {
			//printf("ICI\n");
			a = compare_duree_trv[i+1];
			b = i+1;
		}
		i += 1;
	}
	
	//printf("NB TACHE APRES = %d\n", a);
	//printf("TRV = %d\n", b);
	
/*
Copie du nom de la spécialité, du nom de la commande, et de la durée dans la structures tvs, pour le travailleur trouvé grâce au while précédant --
-- (ayant le moins de durée de travail)
Rajoute +1 au nombre de commande du travailleur qui s'est vu assigné la commande
*/
		
	strcpy(tvs.tab_travailleurs[b].tab_taches_travailleur[tvs.tab_travailleurs[b].nb_cmd_trv].specialite, nom_specialite);
	strcpy(tvs.tab_travailleurs[b].tab_taches_travailleur[tvs.tab_travailleurs[b].nb_cmd_trv].nom_cmd, nom_commande);
	tvs.tab_travailleurs[b].tab_taches_travailleur[tvs.tab_travailleurs[b].nb_cmd_trv].duree = (cms.tab_commandes[commande].taches_par_specialite[specialite].nb_heures_requises) - (cms.tab_commandes[commande].taches_par_specialite[specialite].nb_heures_effectuees);
	tvs.tab_travailleurs[b].nb_duree_trv += tvs.tab_travailleurs[b].tab_taches_travailleur[tvs.tab_travailleurs[b].nb_cmd_trv].duree;
	tvs.tab_travailleurs[b].nb_cmd_trv += 1;
}

/*
Fonction rech_commande ----------------------------------------------------------------------------------
Permet de renvoyer le rang de la commande

Compare le nom de la commande entrée à tous les noms de commande existants

Les printf permettaient de vérifier le bon fonctionnement du programme
*/

int rech_commande (Mot nom_cmd) {
	//printf("rech commande : %d\n", cms.nb_commandes);
	int i = 0;
	while (i < cms.nb_commandes) {
		if (strcmp(cms.tab_commandes[i].nom, nom_cmd) == 0) {
			//printf("VRAI\n");
			return i;
		}
		else {
			//printf("FAUX\n");
			i += 1;
		}
	}
}

/*
Fonction progression ------------------------------------------------------------------------------------
Cette fonction permet d'entrer l'avancement de la tache sur une commande et une spécialité précise, en nombre d'heure
Les progressions peuvent donc s'accumuler

rech_commande renvoie le rang de la commande entrée
rech_specialite renvoie le rang de la spécialité entrée
Rajout du nombre d'heure de plus entré au nombre d'heure déjà effectuées, dans la structure cms, au rang de la commande, dans le tableau taches_par_specialite --
-- à la ligne de la spécialité

Les printf permettaient de vérifier le bon fonctionnement du programme
*/

void traite_progression() {
	Mot nom_commande;
	get_id(nom_commande);
	Mot nom_specialite;
	get_id(nom_specialite);
	int nombre_heure_de_plus = get_int();

	int commande = rech_commande(nom_commande);
	int specialite = rech_specialite(nom_specialite);
	cms.tab_commandes[commande].taches_par_specialite[specialite].nb_heures_effectuees += nombre_heure_de_plus;
	//printf("NB HEURE EFFECTUEE: %d\n", cms.tab_commandes[commande].taches_par_specialite[specialite].nb_heures_effectuees);
	
/*
On soustrait ici le nombre d'heure de plus entré à la case durée, du travailleurs correspondant
Pour cela, on compare les noms de commandes existantes assignées à un travailleur au nom de commande entré, ainsi que la spécialité des travailleurs --
-- ayant cette commande à la spécialité entrée
*/

	int i = 0;
	int j = 0;
	while (i < tvs.nb_travailleurs) {
		while (j < tvs.tab_travailleurs[i].nb_cmd_trv) {
			if (strcmp(nom_commande, tvs.tab_travailleurs[i].tab_taches_travailleur[j].nom_cmd) == 0 && strcmp(nom_specialite, tvs.tab_travailleurs[i].tab_taches_travailleur[j].specialite) == 0) {
				tvs.tab_travailleurs[i].tab_taches_travailleur[j].duree -= nombre_heure_de_plus;
				//printf("Prog: %d %d %d\n",i,j,tvs.tab_travailleurs[i].tab_taches_travailleur[j].duree);
				break;
			}
			else {
				j += 1;
			}
		}
		i += 1;
		j = 0;
	}
}

/*
Fonction specialite -------------------------------------------------------------------------------------
La fonction spécialité permet de consulter toutes les spécialités, avec leur coût horaire

Ecrit "specialites traitees : "
Lors du while, le programme écrit tous les nom de spécialités, avec les coûts horaires associés*
Le i sert à aller de ligne en ligne de la structure sps, afin de trouver les informations
Il est écrit while (i < sps.nb_specialites - 1) pour que le programme écrive la virgule, jusqu'à l'avant dernier
Ensuite, pour le dernier, il écrira sans virgule, et avec un retour à la ligne
*/

void traite_specialites() {
	int i = 0;
	printf("specialites traitees : ");
	while (i < sps.nb_specialites - 1) {
		printf("%s/%d, ", sps.tab_specialites[i].nom, sps.tab_specialites[i].cout_horaires);
		i += 1;
	}
	printf("%s/%d\n", sps.tab_specialites[i].nom, sps.tab_specialites[i].cout_horaires);
}

/*
Fonction travailleurs -----------------------------------------------------------------------------------
Cette fonction permet d'afficher tous les travailleurs opérant dans une spécialité donnée
Si la spécialité donnée est "tous", alors cette fonction doit afficher tous les travailleurs opérant dans toutes les spécialités

Cette fonction est la fonction appelée dans le main()
Si le nom de la spécialité est "tous" (strcmp(nom_specialite, "tous") == 0), alors cette fonction va appeler la fonction affiche_travailleurs_tous
Sinon, cette fonction va appeler la fonction affiche_travailleurs
*/

void traite_travailleurs() {
	Mot nom_specialite;
	get_id(nom_specialite);
	if (strcmp(nom_specialite, "tous") == 0)
		affiche_travailleurs_tous();
	else {
		affiche_travailleurs(nom_specialite);
	}
}

/*
Fonction affiche_travailleurs_tous ----------------------------------------------------------------------
Cette fonction doit afficher tous les travailleurs opérant dans toutes les spécialités

Cette fonction est appelée lorsque il est tapé à l'écran "travailleurs tous"
Elle va appeler la fonction affiche_travailleurs en boucle, en envoyant comme argument à chaque fois un nouveau nom de spécialité --
-- contenu dans la table sps (affiche_travailleurs(sps.tab_specialites[i].nom)
*/

void affiche_travailleurs_tous() {
	int i = 0;
	while (i < sps.nb_specialites) {
		affiche_travailleurs(sps.tab_specialites[i].nom);
		i += 1;
	}
}

/*
Fonction affiche_travailleurs ---------------------------------------------------------------------------
Cette fonction permet d'afficher tous les travailleurs opérant dans une spécialité donnée

rech_specialite renvoie le rang de la spécialité entrée
On va ensuite parcourir toutes les lignes du tableau tab_travailleurs, dans la structure tvs, et regarder si le tag de compétence pour la spécialité donnée est vrai
S'il est vrai va alors s'afficher le nom du travailleurs correspondant
Ce processus est répété pour chaque travailleur.

La variable "trouve" sert à éviter un problème d'affichage
La première fois que le if va se lancer, le printf va se faire sans virgule avant, puis trouve va prendre la valeur 1
Grâce à cela, les autres fois que le if va se lancer, la virgule va se placer avant le printf
On a pas pu utiliser la technique du "while (i < tvs.nb_travailleurs -1)", car le dernier travailleurs ne va pas obligatoirement être affiché, et donc les virgules --
-- pourront encore être mal placées
*/

void affiche_travailleurs(Mot nom_specialite) {
	int i = 0;
	int trouve = 0;
	printf("la specialite %s peut etre prise en charge par : ", nom_specialite);
	int spe = rech_specialite(nom_specialite);
	while (i < tvs.nb_travailleurs) {
		if (tvs.tab_travailleurs[i].tags_competences[spe] == VRAI) {
			if (trouve == 0) {
				printf("%s", tvs.tab_travailleurs[i].nom);
				trouve = 1;
			}
			else {
				printf(", %s", tvs.tab_travailleurs[i].nom);
			}
		}
		i += 1;
	}
	printf("\n");
}

/*
Fonction client -----------------------------------------------------------------------------------------
Cette fonction permet d'afficher toutes les commandes d'un client donné
Si le client donné est "tous", alors cette fonction doit afficher toutes les commandes de tous les clients

Cette fonction est la fonction appelée dans le main()
Si le nom du client est "tous" (strcmp(nom_client, "tous") == 0), alors cette fonction va appeler la fonction affiche_client_tous
Sinon, cette fonction va appeler la fonction affiche_client
*/

void traite_client() {
	Mot nom_client;
	get_id(nom_client);
	if (strcmp(nom_client, "tous") == 0)
		affiche_client_tous();
	else {
		affiche_client(nom_client);
	}
}

/*
Fonction affiche_client_tous ----------------------------------------------------------------------
Cette fonction doit afficher toutes les commandes de tous les clients

Cette fonction est appelée lorsque il est tapé à l'écran "client tous"
Elle va appeler la fonction affiche_client en boucle, en envoyant comme argument à chaque fois un nouveau nom de client --
-- contenu dans la table cls (affiche_client(cls.tab_clients[i].nom))
*/

void affiche_client_tous() {
	int i = 0;
	while (i < cls.nb_clients) {
		affiche_client(cls.tab_clients[i].nom);
		i += 1;
	}
}

/*
Fonction affiche_client ---------------------------------------------------------------------------
Cette fonction permet d'afficher toutes les commandes d'un client donné

La fonction rech_idx_client permet de renvoyer l'idx du client, en entrant juste son nom
Une fois l'idx trouvé, on va alors le comparer aux idx clients présents dans la structure cms, pour chaque commande
Lorsque les deux idx sont égaux, il va s'afficher le nom de la commande.
Ce processus est répété pour chaque commande.

La variable "trouve" sert à éviter un problème d'affichage
La première fois que le if va se lancer, le printf va se faire sans virgule avant, puis trouve va prendre la valeur 1
Grâce à cela, les autres fois que le if va se lancer, la virgule va se placer avant le printf
On a pas pu utiliser la technique du "while (i < cms.nb_commandes -1)", car la dernière commande ne va pas obligatoirement être affiché, et donc les virgules --
-- pourront encore être mal placées

Les printf permettaient de vérifier le bon fonctionnement du programme
*/

void affiche_client(Mot nom_client) {
	int i = 0;

	printf("le client %s a commande : ", nom_client);
	
	int idx_cl = rech_idx_client(nom_client);
	//printf("id client = %d\n", idx_cl);
	//printf("NB COMMANDE = %d\n", cms.nb_commandes);
	
	int trouve = 0;
	while (i < cms.nb_commandes) {
		//printf("IDX = %d", cms.tab_commandes[i].idx_client);
		if (cms.tab_commandes[i].idx_client == idx_cl) {
			if (trouve == 0) {
				printf("%s", cms.tab_commandes[i].nom);
				trouve = 1;
			}
			else {
				printf(", %s", cms.tab_commandes[i].nom);
			}
			i += 1;
		}
		else {
			i += 1;
		}
	}
	printf("\n");
}

/*
Fonction supervision ------------------------------------------------------------------------------------
Cette fonction permet l'affichage de l'état des tâches pour chaque commande, avec sa ou ses spécialités, son nombre d'heures effectuées et son nombre d'heures requises
Cette fonction est divisée en deux sous-fonctions : "traite_supervision" et "affiche_supervision"
*/

/*
Fonction traite_supervision ------------------------------------------------------------------------------------
Cette fonction est appelée dans le main()
Elle permet juste l'affichage de "etat des taches pour %s : ", %s étant le nom de chaque commande.
Pour chaque commande, cette fonction appelle l'autre fonction "affiche_supervision", avec comme argument le nom de la commande

Le while sert à parcourir toutes les commandes de la structure cms
*/

void traite_supervision() {
	int i = 0;
	while (i < cms.nb_commandes) {
		printf("etat des taches pour %s : ", cms.tab_commandes[i].nom);
		affiche_supervision(i);
		i += 1;
	}
}

/*
Fonction affiche_supervision ------------------------------------------------------------------------------------
Cette fonction sert à afficher la ou les spécialités, le nombre d'heures effectuées et le nombre d'heures requises de la commande donnée par la fonction "traite_supervision"
Nous avons créé le tableau liste_spe, qui va contenir les rangs des spécialités utilisées pour la commande en question
Pour savoir si une spécialité est utilisée, nous avons juste à voir si sa ligne nb_heures_requises dans son tableau dans la structure cms est différente de 0
Ceci est donc la fonction du premier while

Ensuite, le deuxième while est utile pour l'affichage
Pour cet affichage, j doit être différent de 0, sinon cela signifie que liste_spe est vide, et donc que la commande ne requiert aucune spécialité
Le système va alors affiché en premier la spécialité, trouvée dans la structure sps, dans le tableau tab_specialites, à la ligne liste_spe[k], donc au i stocké (i étant --
-- toujours le rang de la spécialité), puis le nombre d'heures effectuées et le nombre d'heures requises par le même procédé

Par soucis d'affichage, nous avons utilisé le système de while (k < j - 1), pour que la virgule soit affiché jusqu'à l'avant-dernier

Si liste_spe est vide, le programme va juste afficher un retour à la ligne
*/

void affiche_supervision(int num_commande) {
	int i = 0;
	int j = 0;
	int k = 0;
	int liste_spe[MAX_SPECIALITES] = {0};
	while (i < MAX_SPECIALITES) {
		if (cms.tab_commandes[num_commande].taches_par_specialite[i].nb_heures_requises != 0) {
			liste_spe[j] = i;
			j += 1;
		}
		i += 1;
	}
	if (j != 0) {
		while (k < j - 1) {
		printf("%s:%d/%d, ", sps.tab_specialites[liste_spe[k]].nom, cms.tab_commandes[num_commande].taches_par_specialite[liste_spe[k]].nb_heures_effectuees, cms.tab_commandes[num_commande].taches_par_specialite[liste_spe[k]].nb_heures_requises);
		k += 1;
		}
		printf("%s:%d/%d", sps.tab_specialites[liste_spe[k]].nom, cms.tab_commandes[num_commande].taches_par_specialite[liste_spe[k]].nb_heures_effectuees, cms.tab_commandes[num_commande].taches_par_specialite[liste_spe[k]].nb_heures_requises);
		printf("\n");
	}
	else {
		printf("\n");
	}
}

/*
Fonction traite_charge ----------------------------------------------------------------------------------
Cette fonction permet l'affichage de la charge de travail pour le travailleur donné
Le plus dur pour nous dans cette fonction a été le tri, car il faut trier par ordre de déclaration des travailleurs, puis de commandes, puis de spécialités
*/

void traite_charge() {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	
	int trouve = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	while (i < tvs.nb_travailleurs) { // Cette boucle permet de trier par ordre de déclaration des travailleurs
		if (strcmp(tvs.tab_travailleurs[i].nom, nom_travailleur) == 0) { // Lorsque le travailleur donné par l'utilisateur est trouvé
			printf("charge de travail pour %s : ", nom_travailleur); // Affichage du message
			while (j < cms.nb_commandes) { // Regarde toutes les commandes
				while (k < tvs.tab_travailleurs[i].nb_cmd_trv) { // Cherche si le travailleur à cette commande (Tri par ordre de commande)
					if (strcmp(cms.tab_commandes[j].nom, tvs.tab_travailleurs[i].tab_taches_travailleur[k].nom_cmd) == 0) { // Lorsque c'est le cas
						while (m < MAX_SPECIALITES) { // Regarde toutes les spécialités
							if (cms.tab_commandes[j].taches_par_specialite[m].nb_heures_requises != 0) { // Cherche si la commande a cette spécialité (Tri par ordre de spécialité)
								if (strcmp(sps.tab_specialites[m].nom, tvs.tab_travailleurs[i].tab_taches_travailleur[k].specialite) == 0 && tvs.tab_travailleurs[i].tab_taches_travailleur[k].duree > 0) { // Si c'est le cas et si tache n'est pas finie (tvs.tab_travailleurs[i].tab_taches_travailleur[k].duree > 0)
									if (trouve == 0) { // La variable "trouve" sert à éviter un problème d'affichage
										printf("%s/%s/%dheure(s)", tvs.tab_travailleurs[i].tab_taches_travailleur[k].nom_cmd, tvs.tab_travailleurs[i].tab_taches_travailleur[k].specialite, tvs.tab_travailleurs[i].tab_taches_travailleur[k].duree); // Affichage du message
										trouve = 1;
									}
									else {
										printf(", %s/%s/%dheure(s)", tvs.tab_travailleurs[i].tab_taches_travailleur[k].nom_cmd, tvs.tab_travailleurs[i].tab_taches_travailleur[k].specialite, tvs.tab_travailleurs[i].tab_taches_travailleur[k].duree); 
									}
								}
							}
							m += 1;
						}
						m = 0;
					}
					k += 1;
				}
				j += 1;
				k = 0;
			}
		}
		i += 1;
	}
	printf("\n");
}

/*
Fonction interruption -----------------------------------------------------------------------------------
Cette fonction permet l'interruption du programme, lorsque "interruption" est écrit
*/

void traite_interruption() {
	printf(MSG_INTERRUPTION);
}

/*
Fonction progression passe ------------------------------------------------------------------------------
Cette fonction permet juste l'affichage de "## une reallocation est requise"
*/

void traite_progression_passe() {
	printf(MSG_PROGRESSION_PASSE);
}

//Boucle principale -------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;

	//Initialisation des variables --------------------
	sps.nb_specialites = 0;
	cls.nb_clients = 0;
	trv.nb_cmd_trv = 0;
	trv.nb_duree_trv = 0;
	tvs.nb_travailleurs = 0;
	cms.nb_commandes = 0;

	while (VRAI) {
		get_id(buffer);

		if (strcmp(buffer, "developpe") == 0) {
			if (sps.nb_specialites < MAX_SPECIALITES) {
				traite_developpe();
			}
			/*else {
				printf("Nombre maximum de specialité atteint!\n");
			}*/
			continue;
		}

			if (strcmp(buffer, "embauche") == 0) {
			if (tvs.nb_travailleurs < MAX_TRAVAILLEURS) {
				traite_embauche();
			}
			/*else {
				printf("Nombre maximum de travailleurs atteint!\n");
			}*/
			continue;
		}

		if (strcmp(buffer, "demarche") == 0) {
			if (cls.nb_clients < MAX_CLIENTS) {
				traite_demarche();
			}
			/*else {
				printf("Nombre maximum de clients atteint!\n");
			}*/
			continue;
		}

		if (strcmp(buffer, "commande") == 0) {
			traite_commande();
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			traite_tache();
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression();
			continue;
		}
		if (strcmp(buffer, "passe") == 0) {
			//traite_progression_passe();
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites();
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs();
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client();
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision();
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge();
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
