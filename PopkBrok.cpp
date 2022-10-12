



#include <iostream>
#include <thread>
#include <conio.h>

#include "brain.h"
#include "think.h"
#include "Graph.h"





think th1;
Graph graph1;

void graphs() {
    graph1.initialize();
}

string anything;

int main()
{
    if (bdrawGraph) {
        graph1.initializeAxis();
        thread thread1(graphs);
        thread1.detach();
    }
    
    srand(time(0));
   // th1.run();
    cout << "\n\nPrint anything\n";
    cin>>anything;

    
 
   
  
       
 

}


