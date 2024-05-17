

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

    univers.initialiser2();
    std :: vector<Cellule> cell = univers.getCellules();
    for (auto it = cell.begin() ; it != cell.end();it++)  {
        std :: cout << "********************************** "<< std :: endl;
        if (it->getNbParticules()>0) {
            std::vector<Particule3D> part  = it->getParticules();
            for (auto & itt : part) {
                std :: cout << "Particule :" << itt.getId() << "Coord  :  " << itt.getPos().getX() << " , " << itt.getPos().getY() << std :: endl ;
            }
        }
    }


    std::cout << "Evolution" << std::endl;
    univers.evolution();

    std::cout << "FIN" << std::endl;
    return 0;

}