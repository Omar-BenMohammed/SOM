#include "reseau.h"
#include "iris.h"

/*
 *  calculer le vecteur moyen 
 */
double *avrVector(Data *data, int countL, int countF)
{
    double somme = 0;
    double *vec = malloc(countF * sizeof(double));

    for (int i = 0; i < countF; i++)
    {
        for (int j = 0; j < countL; j++)
        {
            somme += data[j].v[i];
        }
        vec[i] = somme / countL;
        somme = 0;
    }

    return vec;
}

/*
 * calculer le vecteur minimum
 */
double *getLimitVecMin(double *vec, int countF, double min)
{
    double *vecLimitMin = malloc(countF * sizeof(double));
    for (int i = 0; i < countF; i++)
    {
        vecLimitMin[i] = (vec[i] - min);
    }

    return vecLimitMin;
}

/*
 * calculer le vecteur maximum
 */
double *getLimitVecMax(double *vec, int countF, double max)
{
    double *vecLimitMax = malloc(countF * sizeof(double));
    for (int i = 0; i < countF; i++)
    {
        vecLimitMax[i] = (vec[i] + max);
    }
    return vecLimitMax;
}

/*
 * calculer les neurones
 */

void calculNeurons(int nbV, Network *network)
{
    //double nb = 8;//round(sqrt(5 * sqrt(nbV)));
    network->nbC = 10;
    network->nbL = 5;
    network->nbN = 10 * 5;
}

/*  
 * Générer un vecteur aléatoire 
 */

double randVector(double min, double max)
{
    double a = (rand() / (double)RAND_MAX) * (max - min) + min;
    return a;
}

/*  
 * Générer un nombre aléatoire 
 */

int randIndice(int min, int max)
{
    int a = (rand() / (double)RAND_MAX) * (max - min) + min;
    return a;
}

/*
 *  Mélanger un tableau
 */

int *shuffleTab(int countL)
{

    int tmp = 0;
    int random = 0;
    int *tab = NULL;
    tab = malloc(countL * sizeof(int));
    for (int i = 0; i < countL; i++)
    {
        tab[i] = i;
    }
    for (int i = 0; i < countL; i++)
    {
        random = randIndice(i, countL);
        tmp = tab[i];
        tab[i] = tab[random];
        tab[random] = tmp;
    }
    return tab;
}
/*
 * Initialiser un reseau de neurones
 */

Network *initNetwork(int countF, int countL, double *minVec, double *maxVec)
{
    Network *network = NULL;
    network = malloc(sizeof(Network));
    calculNeurons(countL, network);
    network->size = countF;
    Neuron **neuron = NULL;
    neuron = malloc(network->nbL * sizeof(*neuron));
    assert(neuron != NULL);

    for (int i = 0; i < network->nbL; i++)
    {
        neuron[i] = malloc(network->nbC * sizeof(**neuron));
        assert(neuron[i] != NULL);

        for (int j = 0; j < network->nbC; j++)
        {
            neuron[i][j].w = malloc(network->size * sizeof(double));
            assert(neuron[i][j].w  != NULL);

            for (int k = 0; k < countF; k++)
            {
                neuron[i][j].w[k] = randVector(minVec[k], maxVec[k]);
                //printf("%f,",neuron[i][j].w[k]);
            }
            //printf("\n");
        }
    }
    network->neuron = neuron;
    return network;
}

/*
 * calculer la distance euclidienne 
 */

double euclDistance(double *v, double *w, int countF)
{
    double somme = 0.0;
    for (int i = 0; i < countF; i++)
    {
        somme += pow(w[i] - v[i], 2);
    }
    return sqrt(somme);
}

void learning(int nbIter, Iris *data, Network *network, int radius, double alpha)
{
    double iAlpha = alpha;
    int *tab = NULL;
    double dist = 0.0;
    double distMin = 0.0;
    int bL = 0;
    int bC = 0;
    int voisinLMin = 0;
    int voisinCMin = 0;
    int voisinCMax = 0;
    int voisinLMax = 0;
    int updateRadius = nbIter / radius;

    for (int i = 1; i <= nbIter; i++)
    {
        iAlpha = alpha * (1.0 - (double)(i / (double)nbIter));

        if(i % updateRadius == 0 && radius > 0){
            radius-=1 ; 
        }
        
        tab = shuffleTab(data->countL);
        for (int iData = 0; iData < data->countL; ++iData)
        {
            distMin = INFINITY;
            for (int l = 0; l < network->nbL; l++)
            {
                for (int c = 0; c < network->nbC; c++)
                {
                    dist = euclDistance(data->data[tab[iData]].v, network->neuron[l][c].w, data->countF);
                    if (dist < distMin)
                    {
                        distMin = dist;
                        bL = l;
                        bC = c;
                    }
                }
            }
            voisinLMin = bL - radius;
            voisinCMin = bC - radius;
            voisinLMax = bL + radius;
            voisinCMax = bC + radius;

            if (voisinLMin < 0)
            {
                voisinLMin = 0;
            }
            if (voisinCMin < 0)
            {
                voisinCMin = 0;
            }
            if (voisinLMax >= network->nbL)
            {
                voisinLMax = network->nbL - 1;
            }
            if (voisinCMax >= network->nbC)
            {
                voisinCMax = network->nbC - 1;
            }
            for (int l = voisinLMin; l <= voisinLMax; l++)
            {
                for (int c = voisinCMin; c <= voisinCMax; c++)
                {

                    for (int f = 0; f < data->countF; f++)
                    {
                        network->neuron[l][c].w[f] += iAlpha * (data->data[tab[iData]].v[f] - network->neuron[l][c].w[f]);
                    }
                    network->neuron[l][c].label = 5 ; 
                }
            }
        }

        free(tab);
    }
}

/*
 * trouve le label de la donnée  
 */

int label(char *label)
{

    if (strncmp("Iris-setosa", label, 11) == 0)
    {
        return setosa;
    }
    if (strncmp("Iris-versicolor", label, 15) == 0)
    {
        return versicolor;
    }
    if (strncmp("Iris-virginica", label, 14) == 0)
    {
        return virginica;
    }
    return -1;
}

/*
 * Etiquetage de neurones
 */

void labeling(Iris *data, Network *network)
{
    int bData = 0;
    double dist = 0.0;
    double distMin = 0.0;
    for (int l = 0; l < network->nbL; l++)
    {
        for (int c = 0; c < network->nbC; c++)
        {
            distMin = INFINITY;
            for (int iData = 1; iData < data->countL; ++iData)
            {
                dist = euclDistance(data->data[iData].v, network->neuron[l][c].w, data->countF);
                if (dist < distMin)
                {
                    distMin = dist;
                    bData = iData;
                }
            }
            
            network->neuron[l][c].label = label(data->data[bData].label);
            printNetwork(network);
        }
    }
}

/*
 * Tester l'apprentiassage de mon réseau
 */
void test(Iris *data, Network *network)
{
    int count = 0;
    double distMin = 0;
    double dist = 0;
    int bL = 0;
    int bC = 0;
    double reussite = 0.0 ; 
    for (int iData = 0; iData < data->countL; ++iData)
    {
        distMin = INFINITY;
        for (int l = 0; l < network->nbL; l++)
        {
            for (int c = 0; c < network->nbC; c++)
            {
                dist = euclDistance(data->data[iData].v, network->neuron[l][c].w, data->countF);
                if (dist < distMin)
                {
                    distMin = dist;
                    bL = l;
                    bC = c;
                }
            }
        }
        if (label(data->data[iData].label) == network->neuron[bL][bC].label){
            count++;
        }
    }
    printf("%d \n", count);

    reussite = (double)count/data->countL ; 

    printf("%f \n", reussite);
}


/*
 * Afficher ma maquette
 */

void printNetworkFinal(Network *network)
{
    for (int i = 0; i < network->nbL; i++)
    {
        for (int j = 0; j < network->nbC; j++)
        {
            printf("%d ", network->neuron[i][j].label);
        }
        printf("\n");
    }
    printf("\n");
    printf("Iris-setosa: 0\nIris-versicolor: 1\nIris-virginica: 2 \n");
    printf("\n");
}

void printNetwork(Network *network)
{
    for (int i = 0; i < network->nbL; i++)
    {
        for (int j = 0; j < network->nbC; j++)
        {
            printf("%d ", network->neuron[i][j].label);
        }
        printf("\n");
    }
    printf("\n");
}