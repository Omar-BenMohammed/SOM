#ifndef RESEAU_H
#define RESEAU_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

struct Data;
typedef struct Data Data ; 
struct Iris;
typedef struct Iris Iris ; 

/*
 * Structure de neurones 
 */ 
typedef struct Neuron Neuron ; 
struct Neuron{
	double* w; 
	int label ;  
};

/*
 * Structure du reseau neurones 
 */ 
typedef struct Network Network ; 
struct Network{
    int nbN ; 
	int nbL ;
    int nbC ; 
	int size ; 
	Neuron** neuron ; 
};

/*
 * 
 */
typedef enum Label Label ; 
enum Label{
	setosa, versicolor, virginica
}; 

double* avrVector(Data* data,int countL , int countF);
double* getLimitVecMin(double* vec ,int countF, double min); 
double* getLimitVecMax(double* vec ,int countF, double max) ; 
void calculNeurons(int nbV, Network* network);
double randVector(double min,double max);  
int randIndice(int min,int max); 
int* shuffleTab(int countL); 
double euclDistance(double* v , double* w, int countF); 
void learning(int nbIter, Iris* data , Network* network ,int radius , double aplpha);
Network* initNetwork(int countF ,int countL, double* min , double* max);
int label(char* label); 
void labeling(Iris *data, Network *network); 
void printNetworkFinal(Network *network);
void printNetwork(Network *network);
void test(Iris *data, Network *network);

#endif