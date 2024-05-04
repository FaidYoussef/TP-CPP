

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

    Univers univers = Univers(3, 250, 45, 0, 2.5, 0.05, 1.95);

    univers.initialiser();
    

    std::cout << "Evolution" << std::endl;
    univers.evolution();

    std::cout << "FIN" << std::endl;
    return 0;

}