

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

    Univers univers = Univers(3, 240, 130, 0, 1, 1,2.5,0.05, 29.5,1,-12,0);

    //univers.initialiserDemoCercle(16,16,3,Vector3D(0,0,0),Vector3D(0,0,0));
    univers.initialiser(20,20,80,120,Vector3D(0,-10,0),Vector3D(0,0,0));


    std :: vector<Cellule> cell = univers.getCellules();


    std::cout << "Evolution" << std::endl;
    univers.evolution();

    std::cout << "FIN" << std::endl;
    return 0;

}