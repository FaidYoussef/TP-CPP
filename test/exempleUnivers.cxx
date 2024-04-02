

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

    std::vector<Cellule> cellules;
    Univers univers = Univers(3, cellules, 250, 200, 0, 2.5, 0.005, 1.95);

    int dim_rouge = 40;
    int dim1_bleue = 40;
    int dim2_bleue = 160;

    std::vector<Particule3D> particules_bleues;
    particules_bleues.reserve(dim1_bleue*dim2_bleue);
    std::vector<Particule3D> particules_rouges;
    particules_rouges.reserve(dim_rouge*dim_rouge);

    Vector3D vitesse_bleue = Vector3D(0, 0, 0);
    Vector3D vitesse_rouge = Vector3D(0, 10, 0);

    float distance = std::pow(2, 1.0/6.0);


    for (int i = 0; i < dim_rouge; i++) {
        for (int j = 0; j < dim_rouge; j++) {
            Particule3D particule_rouge = Particule3D(i*dim_rouge + j, 1, 1, Vector3D(0, 0, 0), Vector3D(i*distance, j*distance, 0), vitesse_rouge);
            // print i*distance
            // std::cout << particule_rouge.getPos().getX() << std::endl;
            particules_rouges.insert(particules_rouges.begin() + i*dim_rouge + j, particule_rouge);
        }
    }

    for (int i = 0; i < dim1_bleue; i++) {
        for (int j = 0; j < dim2_bleue; j++) {
            Particule3D particule_bleue = Particule3D(i*dim2_bleue + j + dim_rouge*dim_rouge, 1, 0, Vector3D(0, 0, 0), Vector3D(i*distance, j*distance, 0), vitesse_bleue);
            particules_bleues.insert(particules_bleues.begin() + i*dim2_bleue + j, particule_bleue);
        }
    }

    int nCd1 = univers.getL1() / univers.getRCut();
    int nCd2 = univers.getL2() / univers.getRCut();
    
    // print nCd1, nCd2
    std::cout << "nCd1 : " << nCd1 << std::endl;
    std::cout << "nCd2 : " << nCd2 << std::endl;
    // print nCd1*nCd2
    std::cout << "nCd1*nCd2 : " << nCd1*nCd2 << std::endl;

    for (int i = 0; i < nCd1; i++) {
        for (int j = 0; j < nCd2; j++) {
            Vector3D centre = Vector3D((i + 1./2.)*univers.getRCut(), (j + 1./2.)*univers.getRCut(), 0);
            std::vector<Particule3D> particules;

            for (auto& particule : particules_bleues) {
                if (std::abs(particule.getPos().getX() - centre.getX()) <= univers.getRCut() / 2. && std::abs(particule.getPos().getY() - centre.getY()) <= univers.getRCut()/ 2.) {
                    particules.push_back(particule);
                }
            }

            for (auto& particule : particules_rouges) {
                if (std::abs(particule.getPos().getX() - centre.getX()) <= univers.getRCut() / 2. && std::abs(particule.getPos().getY() - centre.getY()) <= univers.getRCut() / 2.) {
                    particules.push_back(particule);
                }
            }

            Cellule cellule = Cellule(centre, particules);
            
            cellules.insert(cellules.begin() + i*nCd2 + j, cellule);
        }
    }

    // print les coordonnées des particules qui ne sont pas dans les cellules
    for (auto& particule : particules_rouges) {
        bool inCell = false;
        for (auto& cellule : cellules) {
            for (auto& cellule_particule : cellule.getParticules()) {
                if (particule.getId() == cellule_particule.getId()) {
                    inCell = true;
                    break;
                }
            }
            if (inCell) {
                break;
            }
        }
        if (!inCell) {
            std::cout << "Particule rouge non dans une cellule : "  << particule.getPos().getX() << " " << particule.getPos().getY() << std::endl;
        }
    }

    // print le centre avec les plus grandes coordonnées
    std::cout << "Centre avec les plus grandes coordonnées : " << cellules[nCd1*nCd2 - 1].getCentre().getX() << " " << cellules[nCd1*nCd2 - 1].getCentre().getY() << std::endl;
    // nomber de particules dans les cellules
    int nbParticules = 0;
    for (auto& cellule : cellules) {
        nbParticules += cellule.getNbParticules();
    }
    if (nbParticules != particules_bleues.size() + particules_rouges.size()) {
        std::cout << "Erreur dans les cellules" << std::endl;
        return 1;
    }
    univers.setCellules(cellules);
    std::cout << "Univers initialisé" << std::endl;
    univers.evolution();

    std::cout << "FIN" << std::endl;
    return 0;

}