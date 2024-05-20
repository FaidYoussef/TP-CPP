//
// Created by fishermoon on 18/05/24.
//

#include <iostream>
#include "Cellule.hxx"
#include "Vector3D.hxx"
#include "Univers.hxx"


// Main function to test the above function with a simple setup
int main() {
    int gridWidth = 3;
    int gridHeight = 3;
    std::vector<Cellule> cellules;

    // Populate the grid with cells and some particles
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            Vector3D vect(0,0,0);
            Cellule cell(i, j,vect);
            cell.addParticule(Particule3D(i + j,1.0,55, vect,Vector3D(i, j, 0),vect));
            cellules.push_back(cell);
        }
    }

    //Univers::processCells(cellules, gridWidth, gridHeight);
    return 0;
}
