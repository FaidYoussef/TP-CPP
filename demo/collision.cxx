



// tester la classe univers

#include <iostream>

#include "Univers.hxx"
#include "Cellule.hxx"
#include "Particule3D.hxx"
#include "Vector3D.hxx"
#include <cmath>
#include <chrono> // Pour mesurer le temps
#include <vector>
#include <list>


int main() {

    Univers univers = Univers(3, 300, 200, 0, 1, 1,2.5,0.005, 1.95);

    univers.initialiserDemoCercle(70,20,10, Vector3D(0,-10,0),Vector3D(0,5,0));


    univers.evolution();

    return 0;

}