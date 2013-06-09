#include <iostream>
#include "graph.h"
#include <cmath>
#include <cstdio>

#define PI 3.14159
#define e  2.71828
using namespace std;

int main()
{

    graph<float,float> g;

    for(float d1 =  0.0; d1 <= 10.0; d1 += 0.02)
    {
        g.addPoint(graphPoint<float,float>(d1,exp(d1)));
        g.display();
    }


    //g.plot(0.0,2.0*PI,0.1,sin);
    //g.display();
   // g.animate();
    cout << "Return key to close";
    getchar();
    return 0;
}
