#include"iris.h"
#include"reseau.h"

/*
 *   Fonction parser les données 
 */

Iris* parse(char const* fichier){
    
    double somme = 0 ; 
    Iris* tabData  ; 
    size_t tailleAllouer = 0 ; 
    char *chaine = NULL; 
    char s[2] = "," ; 
    char* token ;
    char* nb ; 
    fpos_t pos;

    FILE* iris = fopen(fichier,"r");
    
    tabData = malloc(sizeof(Iris)); 
    fgetpos(iris, &pos);
    //claculer le nombres de champs dans une ligne  
    tabData->countF = countField(iris);

    fsetpos(iris, &pos);

   //claculer le nombres de ligne dans un fichier 
    tabData->countL = countLine(iris);
    fsetpos(iris, &pos);   

    //ajouter les données 
    tabData->data = malloc(tabData->countL*sizeof(Data));
    assert(tabData->data != NULL);
    for(int i = 0; i < tabData->countL; i++){
        tabData->data[i].v = malloc(tabData->countF*sizeof(double)) ;
        assert(tabData->data[i].v != NULL);
        getline(&chaine , &tailleAllouer , iris) ;
        token = strtok(chaine, s); 
        for(int j = 0; j < tabData->countF; j++){
            tabData->data[i].v[j] = strtod(token, &nb);
            somme+= pow(tabData->data[i].v[j],2); 
            token = strtok(NULL, s);
        }

        tabData->data[i].label = strdup(token);
        tabData->data[i].label[strlen(tabData->data[i].label) - 1] = 
        (tabData->data[i].label[strlen(tabData->data[i].label) - 1] == '\n') ? '\0' :  
        tabData->data[i].label[strlen(tabData->data[i].label) - 1];
         
        tabData->data[i].norm = sqrt(somme); 
        somme = 0 ; 
    }
    normData(tabData->data, tabData->countL, tabData->countF); 
    fclose(iris); 
    
    return tabData;
}

/*
 *   compter les ligne d'un fichier
 */

int countLine(FILE* fichier){
    int count = 0 ; 
    char* line = NULL ; 
    size_t tailleAllouer = 0 ; 
    while(getline(&line , &tailleAllouer , fichier) != -1){
        count++; 
    }
    
    return count ; 
}

/*
 * compter les champs des données
 */

int  countField(FILE* fichier){
    int c = 0 ; 
    int i = 0 ; 
    while((c = fgetc(fichier)) != '\n' && c != EOF ){
        if(c == 44){
            i++; 
        }
    }
    return i;
}

/*
 *  verifi le champs si c'est un double
 */

/*int isNumber(char* token){
    int i = 0 ; 
    while(token[i]){
        if (!(isdigit(token[i]) || token[i] == '.')){
            return 0 ; 
        }
        i++; 
    }
    return 1 ;
}*/

/*
 * liberer la memoire
 */

void freeData(Iris* tabData,int countL){
    for(int i = 0; i < countL; i++){
        free(tabData->data[i].v); 
        tabData->data[i].v = NULL ;    
        free(tabData->data[i].label); 
        tabData->data[i].label=NULL ;
    }
    free(tabData->data); 
    tabData->data = NULL; 
    free(tabData); 
    tabData = NULL ;  
}

/*
 * afficher les données 
 */
void printData(Data* data, int countL , int countF){
    for(int i = 0; i < countL; i++){
        printf("   %d -",i+1) ;
        for(int j = 0; j < countF; j++){
            printf("%f ,",data[i].v[j]) ; 
        }
        printf("%s",data[i].label) ;
    }
}

/*
 * normaliser les vecteurs 
 */

void normData(Data* data, int countL , int countF){
    for(int i = 0; i < countL; i++){
        for(int j = 0; j < countF; j++){
            data[i].v[j] = (data[i].v[j])/(data[i].norm) ; 
        }
    }
}
