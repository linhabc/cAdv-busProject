#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSTAT 1000
#define MAXBUS  200
#include "fields.h"
#include "main.h"

int main(){
    station *statList = (station *)malloc(sizeof(station)* MAXSTAT);
    bus *busList = (bus *)malloc(sizeof(bus)*MAXBUS);
    //read station and name
    int countStat  = getStatNameNId("station_name.txt",statList);
    //bus through that station
    getStatBusList("station_route.txt", statList);

    int countBus = getBusStatList("route_dat.txt", busList);
    getBusDistList("nextToStation1.txt", busList, countBus);

    Graph g = createGraph();
    int choice;
    int arrStat[50], count = 0;//lst cac node trong shortest path
    int node1=0,node2=0;
    int indexStat;
    int indexBus;

    while(1)
    {   
        printf("\n");
        printf("1. Find station by index \n");
        printf("2. Find route by bus index\n");
        printf("3. Shortest path from a to b\n");
        printf("4. Shorttest path Bus route from a to b\n");
        printf("5. Exit\n");
        printf("Enter your choice :  ");
        scanf("%d",&choice);
        
        switch(choice)
        {
            case 1:
                printf("Enter station index: \n");
                scanf("%d", &indexStat);
                printf("Name of station: \n");
                displayStationByIndex(statList, countStat,indexStat);
                printf("\n");
                break;
            
            case 2: 
                printf("Enter Bus index: \n");
                scanf("%d", &indexBus);
                printf("Bus route %d: ", indexBus);
                displayBusRouteByIndex(busList, statList, countBus, indexBus);
                break;
            
            case 3:
                read_weighted_graph(g, statList, "nextToStation.txt");
                printf("Choose start destination: \n");
                scanf("%d", &node1);
                printf("Choose end destination: \n");
                scanf("%d", &node2);
                printf("Shortest path:\n");
                getShortestPathBtweenTwoBusStation(g, node1, node2);
                // printGraph(g);
                printf("\n");
                break;
            
            case 4:
                getDirection(g, node1, node2, busList, countBus, statList, countStat);
                break;

            case 5: 
                exit(0);
            
        }
    }

    return 0;
}