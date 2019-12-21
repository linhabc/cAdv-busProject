#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "weighted_graph.h"
#include "fields.h"
#define MAX 100

typedef struct station {
    int idStat;
    char name[MAX];
    int route[MAX];
    int countRoute;
} station;  

typedef struct dist {
    int node1;
    int node2;
    double distance;
} dist; // distance between two node

typedef struct busRoute {
    int idBus;
    int routeStat[MAX + 1];
    dist distance[MAX];
    int countStat;
} bus;  // 

// get station name and id
int getStatNameNId(char *fn1, station statList[]) {
    FILE *f1 = fopen(fn1, "r");
    int count = 0;
    
    if(f1 == NULL ) {
        printf("fail!\n");
        return -1;
    }else {
        int i = 0;
        char *line = NULL;
        size_t len = 0;
        __ssize_t read;
        do {
            char c;
            fscanf(f1,"%d%c%[^\n]", &statList[i].idStat, &c, statList[i].name);
            i++;
            count ++;
        }while((read = getline(&line, &len, f1))!= -1);
    }
    fclose(f1);
    return count; // return total number of station
}

// get list of bus pass through the station
void getStatBusList(char *fn1, station statList[]) {
    IS is;
    is =new_inputstruct(fn1);
    if(is == NULL) {
        printf("fail!\n");
        return ;
    }else {
        int i;
        while(get_line(is) >= 0) {
            for(i = 1; i < is->NF; i++) {
                statList[is->line -1].route[i -1] = atoi(is->fields[i]);
            }
            statList[is->line -1].countRoute = is->NF - 1;
        }
        jettison_inputstruct(is);
    }
}

// get list of station that the bus pass through
int getBusStatList(char *fn1, bus busList[]) {
    int count = 0;
    IS is;
    is = new_inputstruct(fn1);
    if(is == NULL) {
        printf ("fail\n");
        return -1;
    }else {
        int i;
        while(get_line(is) >= 0) {
            busList[is->line -1].idBus = atoi(is->fields[0]);

            for(i = 1; i < is->NF; i++) {
                busList[is->line -1].routeStat[i-1] = atoi(is->fields[i]);
            }
            busList[is->line -1].countStat = is->NF - 1;
            count ++;
        }
        jettison_inputstruct(is);
    }
    return count; // return total number of station
}

// get distance between 2 bus station next to each other
void getBusDistList(char *fn1, bus busList[], int countBus) {
    IS is;
    is =new_inputstruct(fn1);
    if(is == NULL) {
        return ;
    }
    
    while(get_line(is) >= 0) {
        int x ,y;
        for(int i = 0; i < countBus; i ++) 
            for(int j = 0; j < busList[i].countStat - 1; j ++) {
                x = busList[i].routeStat[j];
                y = busList[i].routeStat[j+ 1];
                if((atoi(is->fields[0]) == x) && (atoi(is->fields[1]) == y)) {
                    busList[i].distance[j].node1 = x; 
                    busList[i].distance[j].node2 = y;
                    busList[i].distance[j].distance = atof(is->fields[2]);
                }
            }
    }
    jettison_inputstruct(is);
    return ;
}

//lay ten tram tu index
void displayStationByIndex(station statList[], int countStat,int index) {
    for(int i = 0; i < countStat; i++) {
        if(statList[i].idStat == index) {
            printf("%s", statList[i].name);
        }
    }
    return ;
}

char* getStationByIndex(station statList[], int countStat, int index){
    for(int i = 0; i < countStat; i++) {
        if(statList[i].idStat == index) {
            return statList[i].name;
        }
    }         
    return NULL;
}

//the hien lo trinh bang index
void displayBusRouteByIndex(bus busList[], station statList[],int countBus,int index) {
    for(int i = 0; i< countBus; i++) {
        if(busList[i].idBus == index){
            for(int j = 0; j < busList[i].countStat - 1; j++) {
                printf("%s->", statList[busList[i].routeStat[j]].name);
            }
            printf("%s\n",statList[busList[i].routeStat[busList[i].countStat - 1]].name);
        }
    }

    return ;
}

void read_weighted_graph(Graph g, station statList[], char *fileName){
    FILE *fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Cannot open file! \n");
        return;    
    }
    
    int totalVertice, totalEdge;

    fscanf(fp, "%d %d\n", &totalEdge, &totalEdge);

    int vertex1, vertex2;
    double weight;
    for(int i=0; i<totalEdge; i++){
        fscanf(fp, "%d %d %lf\n", &vertex1, &vertex2, &weight);
        char* nameVertex1 = getStationByIndex(statList, totalVertice, vertex1);
        addVertex(g, vertex1, nameVertex1);
        char* nameVertex2 = getStationByIndex(statList, totalVertice, vertex2);
        addVertex(g, vertex2, nameVertex2);
        // printf("%d %d %lf\n", vertex1, vertex2, weight);
        addEdge(g, vertex1, vertex2, weight);
    }

    fclose(fp);

    return;
}

void printGraph(Graph g){
    JRB node, nodeVal, tmp;
    jrb_traverse(node, g.vertices){
        printf("%s- %d \n", jval_s(node->val), jval_i(node->key));
    }

    jrb_traverse(node, g.edges){
        printf("Station %d \n", jval_i(node->key));
        nodeVal = (JRB) jval_v(node->val);
        jrb_traverse(tmp, nodeVal){
            printf("%lf ", jval_d(tmp->val));
        }
        printf("\n");
    }
}

void getShortestPathBtweenTwoBusStation(Graph g, int s, int t){
    int path[100], length;
    double totalDistance = shortestPath(g, s, t, path, &length);
    if (totalDistance == INFINITIVE_VALUE) {
         printf("No path from %s to %s\n", getVertex(g, s), getVertex(g, t));
    } else {
         printf("Path from %s to %s\n", getVertex(g, s), getVertex(g, t));
         for (int i=0; i<length; i++)
             printf(" => %s", getVertex(g, path[i]));
    }
}

void getDirection(Graph g, int s, int t, bus busList[], int countBus, station statList[], int countStat){
    int path[100], length;
    int node1, node2;
    double totalDistance = shortestPath(g, s, t, path, &length);
    if (totalDistance == INFINITIVE_VALUE) {
         printf("No path from %s to %s\n", getVertex(g, s), getVertex(g, t));
    } else {
        //  for (int i=0; i<length; i++){
            // node1 = path[i];
            // node2 = path[i+1];
            // printf("Go from %s to %s by bus number %d\n", getVertex(g, s), getVertex(g, t), totalDistance);
        // }
        for(int i = length; i > 0; i--) {
            int arrBus[50];
            int countFT = 0;
            countFT = busFromTo(busList, path[i], path[i-1],countBus, arrBus);
            if(countFT == 0) {
                continue;
            } else {
                printf("Go by bus ");
                for(int j = 0; j < countFT; j++) {
                    printf("%d ", arrBus[j]);
                }
                printf("from ");
                displayStationByIndex(statList, countStat, path[i]);
                printf("to ");
                displayStationByIndex(statList, countStat, path[i-1]);
                printf("\n");
            }    
        }
    }
}

int busFromTo(bus busList[], int node1, int node2, int countBus, int arrBus[]) {
    int count = 0;
    for(int i = 0; i < countBus; i ++) {
        for(int j = 0; j < busList[i].countStat -1; j ++) {
            if((busList[i].distance[j].node1 == node1) && (busList[i].distance[j].node2 == node2)) {
                arrBus[count] = busList[i].idBus;
                count ++;
            }
            else if((busList[i].distance[j].node1 == node2) && (busList[i].distance[j].node2 == node1)) {
                arrBus[count] = busList[i].idBus;
                count ++;
            }
        }
    }
    return count;
}