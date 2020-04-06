#include<time.h>
#include"iris.h"
#include"reseau.h"

int main(int argc, char const *argv[]){
    srand(time(NULL)); 
    double min = 0.05 ; 
    double max = 0.05 ; 

    Iris* tabData = parse(argv[1]); 
    double *vec = avrVector(tabData->data,tabData->countL ,tabData->countF);
    double *vecMax = getLimitVecMin(vec ,tabData->countF,min);
    double *vecMin = getLimitVecMax(vec ,tabData->countF,max) ; 
    Network *network = initNetwork(tabData->countF ,tabData->countL, vecMin , vecMax);
    learning(500, tabData , network ,12, 0.7); 
    learning(2000, tabData , network ,1, 0.07); 
    labeling(tabData, network); 
    printNetworkFinal(network);
    test(tabData, network); 
    freeData(tabData, tabData->countL);
    return 0;
}
