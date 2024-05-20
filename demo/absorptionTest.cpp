//
// Created by fishermoon on 18/05/24.
//

#include <iostream>
#include "Univers.hxx"
#include "Particule3D.hxx"
#include "Cellule.hxx"

void Univers::printParticules() {
    for (auto& cell : cellules) {
        auto& particules = cell.getParticules();
        std::cout << "Remaining Particles: " << particules.size() << std::endl;
        for (auto& p : particules) {
            std::cout << "Position: (" << p.getPos().getX() << ", " << p.getPos().getY() << ")" << std::endl;
        }
    }
}

int main() {
    Univers myUnivers(3,100,1,1,0.1,1,1);

    // Create a cell and add particles at various positions including boundary positions
    Cellule myCell;
    myCell.addParticule(Particule3D(1, 1.0, 1, Vector3D(0,0,0), Vector3D(-1, 5, 0), Vector3D(0,0,0))); // Should be removed
    myCell.addParticule(Particule3D(2, 1.0, 1, Vector3D(0,0,0), Vector3D(50000, 5, 0), Vector3D(0,0,0)));  // Should remain
    myCell.addParticule(Particule3D(3, 1.0, 1, Vector3D(0,0,0), Vector3D(10, 500000, 0), Vector3D(0,0,0))); // Should be removed
    myCell.addParticule(Particule3D(4, 1.0, 1, Vector3D(0,0,0), Vector3D(50, 100000, 0), Vector3D(0,0,0))); // Should be removed
    myCell.addParticule(Particule3D(5, 1.0, 1, Vector3D(0,0,0), Vector3D(5, -1, 0), Vector3D(0,0,0))); // Should be removed

    myUnivers.addCellule(myCell);
    myUnivers.printParticules(); // Print before absorption
    myUnivers.absorptionBC();
    myUnivers.printParticules(); // Print after absorption

    return 0;
}
