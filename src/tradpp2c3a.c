/* arbre.c */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bilquad.h"
#include "util.h"
#include "environ.h"
#include "../ppascalbison.h"
/* iimp.tab.h APRES arbre,h, sinon le type NOE est inconnu de gcc    */
/*-------------------------------------------------------------------*/
/* ----------------------------types---------------------------------*/
/* NOE,PILCOM,ENV : definis dans arbre.h                             */
/* QUAD,BILQUAD: definis dans codec3a.h                              */
/*-------------------------------------------------------------------*/
/*---------------------quadruplets-----------------------------------*/

/* retourne une nouvelle chaine */
char *gensym(char *prefix) {
    static int counter=0;
    char *chaine;char *chcounter;
    chcounter=Idalloc();
    chaine=Idalloc();
    strcpy(chaine,prefix);
    sprintf(chcounter,"%d",counter);   /* prefix+chaine(counter)*/
    counter=counter+1;
    strcat(chaine,chcounter);
    return(chaine);
}

/* test de gensym */
void test_constantes(int n) {
    char *ident;
    for(int i = 0; i < n; i++) {
        ident = gensym("CONS");
        printf("%s \n",ident);
    }
}


/* retourne bq +  skip */
BILQUAD ajouterskip(BILQUAD bq) {
    QUAD nq;
    BILQUAD nbq;
    nq = creer_quad(gensym("ET"), Sk, NULL, NULL, NULL); /* instruction skip */
    nbq = creer_bilquad(nq);
    return (concatq(bq, nbq));
}

/* retourne la "forme normale" de bq: dernier quad = skip */
BILQUAD normal(BILQUAD bq) {
    if (bq.fin == NULL) {
        return(ajouterskip(bq));
    }
    else {
        if (bq.fin->OP != Sk)        /* pas normal-> on normalise */
            return(ajouterskip(bq));
        else                      /* deja normal */
            return(bq);
    }
}

/* traduit entier (= codop) vers chaine (= nom operation)  */
char *nomop(int codop) {
    switch(codop) {
        case(I): return("I");
        case(V): return("V");
        case(Mp): return("Mp");
        case(Af): return("Af");
        case(Sk): return("Sk");
        case(Se): return("Se");
        case(If): return("If");
        case(Th): return("Th");
        case(El): return("El");
        case(Wh): return("Wh");
        case(Do): return("Do");
        case(Pl): return("Pl");
        case(Mo): return("Mo");
        case(Mu): return("Mu");
        case(And): return("And");
        case(Or): return("Or");
        case(Not): return("Not");
        case(Lt): return("Lt");
        case(Eq): return("Eq");
        case(Ind): return("Ind");
        case(Afc): return("Afc");
        case(AfInd): return("AfInd");
        case(St): return("St");
        case(Jp): return("Jp");
        case(Jz): return("Jz");
        case(Param): return("Param");
        case(Call): return("Call");
        case(Ret): return("Ret");
        /*case(halt): return("halt");
        case(nop): return("nop");
        case(rrmovl): return("rrmovl");
        case(irmovl): return("irmovl");
        case(rmmovl): return("rmmovl");
        case(mrmovl): return("mrmovl");
        case(addl): return("addl");
        case(subl): return("subl");
        case(andl): return("andl");
        case(xorl): return("xorl");
        case(jmp): return("jmp");
        case(je): return("je");
        case(jg): return("jg");
        case(call): return("call");
        case(ret): return("ret");
        case(pushl): return("pushl");
        case(popl): return("popl");*/
        case(0): return("");                   /* code 0: directive assembleur y86 */
        default: return(NULL);
    };
}


/* affiche le quadruplet (pour generer code) avec separateur":" ;
puis saute a la ligne */
void ecrire_sep_quad(QUAD qd) {
    if(strcmp(qd->ETIQ, "") == 0) {     /* etiquette= mot vide */
        printf("%-10s: ","");
    }
    else {
        printf("%-10s:", qd->ETIQ);
    }
    printf("%-6s:",nomop(qd->OP));
    if (qd->ARG1 != NULL) {
        printf("%-10s:", qd->ARG1);
    }
    else {
        printf("%-10s:", "");
    }
    if (qd->ARG2 != NULL) {
        printf("%-10s:", qd->ARG2);
    }
    else {
        printf("%-10s:", "");
    }
    if (qd->RES != NULL) {
        printf("%-10s\n", qd->RES);
    }
    else {
        printf("\n");
    }
}


/* affiche la biliste de quad (avec separateurs) */
void ecrire_sep_bilquad(BILQUAD bq) {
    QUAD qcour;
    qcour = bq.debut;
    while(qcour != NULL) {
        ecrire_sep_quad(qcour);
        qcour = qcour->SUIV;
    }
}

/*-------------------------------------------------------------------*/
/*------------------------pp-vers-quad------------------------------*/

/* traduit une (expression ou commande) en biliste de quadruplets */
/* met a jour l'environnement (var globale)                      */
BILQUAD pp2quad(Noeud* ec) {
    extern ENV envrnt;
    BILQUAD bilq1, bilq2, bilexp, bilres; /* trad de: fg, fd, expression, resultat */
    int newop;
    char *netiq, *netiqf, *nres;        /* nouveaux ingredients */
    char *narg1 = NULL;
    char *narg2 = NULL;
    QUAD nquad;      /* nouveau quadruplet   */
    assert(ec != NULL);
    // printf("\n*************** | DANS PP2QUAD() | ***************\n\n");
    // printf("Noeud: %p\n", ec);
    // printf("Opérateur: %s\n", nomop(ec->codop));
    switch(ec->codop) {
      // printf("Dans switch !\n");
        /* CAS: ec est une EXPRESSION */
        case Pl: case Mo: case Mu:                   /* operation binaire */
            /* les ingredients */
            netiq = gensym("ET");
            newop = ec->codop;
            /* les traductions des deux arguments */
            bilq1 = pp2quad(ec->gauche);
            bilq2 = pp2quad(ec->droit);
            /* se simplifie ? */
            if (ec->gauche->codop != V) {
                narg1 = Idalloc();
                strcpy(narg1, bilq1.fin->RES);
            }
            else {
                narg1 = Idalloc();
                strcpy(narg1, ec->gauche->ETIQ);
            }
            if (ec->droit->codop != V) {
                narg2 = Idalloc();
                strcpy(narg2, bilq2.fin->RES);
            }
            else {
                narg2 = Idalloc();
                strcpy(narg2, ec->droit->ETIQ);
            }
            nres = gensym("VA");
            /* on insere le nom de var dans l'environnement */
            initenv(&envrnt, nres, 0); // Le 0 juste ajouté à titre indicatif. A changer !
            /* le quadruplet: ETnum, Afc, chaineconst,-, VAnum */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            bilres = creer_bilquad(nquad);
            /* la suite de quadruplets */
            bilq2 = concatq(bilq1, bilq2);
            bilres = concatq(bilq2, bilres);
            break;
        case I:
            /* les ingredients */
            netiq = gensym("ET");
            newop = Afc;
            narg1 = Idalloc();
            // printf("case I: ec->ETIQ = %s\n", ec->ETIQ);
            sprintf(narg1,"%s", ec->ETIQ);
            narg2 = NULL;
            nres = gensym("CT");
            /* on insere le nom de const dans l' environnement */
            // initenv(&envrnt, nres, 0); // Le 0 juste ajouté à titre indicatif. A changer !
            /* le quadruplet: ETnum, Afc, chaineconst,-, CTnum */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            bilres = creer_bilquad(nquad);
            // printf("J'ai fini avec case I\n");
            break;
        case V:
            /* le quadruplet: skip, resultat dans chainevar */
            // printf("Je suis entré dans case V\n");
            netiq = gensym("ET");
            newop = Sk;
            narg1 = NULL;
            narg2 = NULL;
            nres = ec->ETIQ;
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            bilres = creer_bilquad(nquad);
            break;
        /* CAS: ec est une COMMANDE */
        case Mp:
            bilq1 = pp2quad(ec->gauche);
            /* les ingredients */
            netiq = gensym("ET");
            newop = St;
            narg1 = NULL;
            narg2 = NULL;
            nres = NULL;
            /* le quadruplet final: stop  (pas d'adresse de resultat) */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            bilq2 = creer_bilquad(nquad);
            bilres = concatq(bilq1, bilq2);
        break;
            case Af:
            /* les ingredients */
            netiq = gensym("ET");
            newop = Af;
            /* assert(ec->gauche->codop == V); */
            /* narg1 =  chaine en lhs */
            narg1 = ec->gauche->ETIQ;
            // printf("La partie gauche de Af est: %s\n", narg1);
            /* narg2 =  adresse res du code du rhs */
            bilq2 = pp2quad(ec->droit);
            narg2 = Idalloc();
            strcpy(narg2, bilq2.fin->RES);
            nres = NULL;
            /* le quadruplet: ETnum, Af, chainevar1,chaineres2, NULL */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            bilres = concatq(bilq2, creer_bilquad(nquad));
            break;
        case Sk:
            /* les ingredients */
            netiq = gensym("ET");
            newop = Sk;
            narg1 = NULL;
            narg2 = NULL;
            nres = NULL;
            /* le quadruplet: skip  (pas d'adresse de resultat) */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            bilres = creer_bilquad(nquad);
            break;
        case Se:
            bilq1 = pp2quad(ec->gauche);
            // printf("I'm here \n");
            bilq2 = pp2quad(ec->droit);
            bilres = concatq(bilq1, bilq2);
            break;
        case If:
            bilexp = pp2quad(ec->gauche);    /* traduction de l'expression */
            bilq1 = pp2quad(ec->droit->gauche); /* commande (cas vrai) */
            bilq2 = pp2quad(ec->droit->droit); /* commande (cas faux) */
            bilq2 = normal(bilq2);
            /* les ingredients de Q1 */
            netiq = gensym("ET");
            newop = Jz;
            narg1 = bilexp.fin->RES;
            narg2 = NULL;
            nres = bilq2.debut->ETIQ;
            /* le quadruplet Q1 */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            /* nouvelle biliste */
            bilres = concatq(bilexp, creer_bilquad(nquad));
            bilres = concatq(bilres, bilq1);
            /* les ingredients de Q2 */
            netiq = gensym("ET");
            newop = Jp;
            narg1 = NULL;
            narg2 = NULL;
            nres = bilq2.fin->ETIQ;
            /* le quadruplet Q2 */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            /* nouvelle biliste */
            bilres = concatq(bilres, creer_bilquad(nquad));
            bilres = concatq(bilres, bilq2);
            break;
        case Wh:
            bilexp = pp2quad(ec->gauche);    /* traduction de l'expression */
            bilq1 = pp2quad(ec->droit);     /* traduction du corps        */
            bilq1 = normal(bilq1);
            /* les ingredients de Q1 */
            netiq = gensym("ET");
            newop = Jz;  /* etiquette de Q1            */
            narg1 = bilexp.fin->RES;
            narg2 = NULL;
            netiqf = gensym("ET");        /* etiquette fin de traduction */
            nres = netiqf;
            /* le quadruplet Q1 */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            /* nouvelle biliste */
            bilres = concatq(bilexp, creer_bilquad(nquad));
            bilres = concatq(bilres, bilq1);
            /* les ingredients de Q2 */
            newop = Jp;
            /* narg1 = narg2 = NULL; */
            nres = bilexp.debut->ETIQ;
            /* on substitue Q2 a la fin de bilres */
            bilres.fin->OP = newop;
            assert(bilres.fin->ARG1 == NULL); /* vrai si bilq1 est normalise */
            assert(bilres.fin->ARG2 == NULL);
            bilres.fin->RES = nres;
            /* les ingredients de Q3 */
            netiq = netiqf;                   /* etiquette de Q3            */
            newop = Sk;
            narg1 = NULL;
            narg2 = NULL;
            nres = NULL;
            /* le quadruplet Q3 */
            nquad = creer_quad(netiq, newop, narg1, narg2, nres);
            /* nouvelle biliste */
            bilres = concatq(bilres, creer_bilquad(nquad));
            break;
        default: break;
    };
    return(bilres);
}
