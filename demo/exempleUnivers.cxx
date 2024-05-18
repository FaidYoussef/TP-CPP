

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

    Univers univers = Univers(3, 300, 200, 0, 1, 1,2.5,0.05, 1.95);

    univers.initialiser2(4,4,4,16,Vector3D(0,-10,0),Vector3D(0,5,0));
    std :: vector<Cellule> cell = univers.getCellules();




std::cout << "Evolution" << std::endl;
    univers.evolution();

    std::cout << "FIN" << std::endl;
    return 0;

}