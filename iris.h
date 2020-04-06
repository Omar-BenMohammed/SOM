#ifndef IRIS_H
#define IRIS_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include"reseau.h"

/*
 * Structure des données 
 */
typedef struct Data Data ; 
struct Data{
	double* v ; 
	double norm ; 
	char* label ;  
}; 

/*
 * Tableau qui contient structure des données 
 */
typedef struct Iris Iris ; 
struct Iris{
	int countL;
	int countF ; 
	Data* data ; 
}; 


Iris* parse(char const* fichier);  
int countLine(FILE* fichier); 
int countField(FILE* fichier);
//int isNumber(char* token); 
void freeData(Iris* tabData,int countL); 
void printData(Data* data, int line , int filed); 
void normData(Data* data, int lines , int fileds); 

#endif
