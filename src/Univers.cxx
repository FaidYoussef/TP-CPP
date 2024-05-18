#include "Univers.hxx"
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Cellule.hxx"


Univers::Univers(int dimension, int L1, int L2, int L3, float rCut, float dt, float tmax) {
    if (dimension < 1 || dimension > 3) {
        std::cerr << "Dimension invalide" << std::endl;
        exit(1);
    }
    this->dimension = dimension;
    this->cellules = std::vector<Cellule>();
    this->rCut = rCut;
    this->L1 = L1;
    if (dimension < 3) {
        this->L3 = 0;
    } else {
        this->L3 = L3;
    }
    if (dimension < 2) {
        this->L2 = 0;
    } else {
        this->L2 = L2;
    }

    this->nbParticules = 0;

    this->dt = dt;
    this->tmax = tmax;
    this->eps = 0;
    this->sigma = 0;


}
Univers::Univers(int dimension, int L1, int L2, int L3,int eps,int sigma, float rCut, float dt, float tmax) {
    if (dimension < 1 || dimension > 3) {
        std::cerr << "Dimension invalide" << std::endl;
        exit(1);
    }
    this->dimension = dimension;
    this->cellules = std::vector<Cellule>();
    this->rCut = rCut;
    this->L1 = L1;
    if (dimension < 3) {
        this->L3 = 0;
    } else {
        this->L3 = L3;
    }
    if (dimension < 2) {
        this->L2 = 0;
    } else {
        this->L2 = L2;
    }
    
    this->nbParticules = 0;

    this->dt = dt;
    this->tmax = tmax;
    this->sigma = sigma;
    this->eps = eps;

}

Univers::Univers() : dimension(0), nbParticules(0), cellules(std::vector<Cellule>()) {}

void Univers::initialiser() {
    int dim_rouge = 10;
    int dim1_bleue = 15;
    int dim2_bleue = 30;

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

    for (int i = 0; i < dim2_bleue; i++) {
        for (int j = 0; j < dim1_bleue; j++) {
            Particule3D particule_bleue = Particule3D(i*dim1_bleue + j + dim_rouge*dim_rouge, 1, 0, Vector3D(0, 0, 0), Vector3D(i*distance, j*distance, 0), vitesse_bleue);
            particules_bleues.insert(particules_bleues.begin() + i*dim1_bleue + j, particule_bleue);
        }
    }

    int nCd1 = L1 / rCut;
    int nCd2 = L2 / rCut;

    // print nCd1, nCd2
    std::cout << "nCd1 : " << nCd1 << std::endl;
    std::cout << "nCd2 : " << nCd2 << std::endl;
    // print nCd1*nCd2
    std::cout << "nCd1*nCd2 : " << nCd1*nCd2 << std::endl;

    cellules.reserve(nCd1*nCd2);
    for (int i = 0; i < nCd1; i++) {
        for (int j = 0; j < nCd2; j++) {
            Vector3D centre = Vector3D((i + 1./2.) * rCut, (j + 1./2.)*rCut, 0);
            std::vector<Particule3D> particules;

            for (auto& particule : particules_bleues) {
                if (std::abs(particule.getPos().getX() - centre.getX()) <= rCut / 2. && std::abs(particule.getPos().getY() - centre.getY()) <= rCut / 2.) {
                    particules.push_back(particule);
                }
            }

            for (auto& particule : particules_rouges) {
                if (std::abs(particule.getPos().getX() - centre.getX()) <= rCut / 2. && std::abs(particule.getPos().getY() - centre.getY()) <= rCut / 2.) {
                    particules.push_back(particule);
                }
            }

            Cellule cellule = Cellule(i, j, centre, particules);

            cellules[i*nCd2 + j] = cellule;
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
    for (auto& cellule : cellules) {
        nbParticules += cellule.getNbParticules();
    }
    if (nbParticules < particules_bleues.size() + particules_rouges.size()) {
        std::cout << "Erreur dans les cellules" << std::endl;
        exit(1);
    }
}


void Univers ::initialiser2() {
    int nCellsX = L1 / rCut;
    int nCellsY = L2 / rCut;
    cellules.reserve(nCellsX*nCellsY);

    // Créer toutes les cellules
    for ( int i = 0; i < nCellsX; i++) {
        for ( int j = 0 ; j < nCellsY;j++) {
            Vector3D centre((i + 1./2.) * rCut, (j + 1./2.)*rCut, 0);
            Cellule cellule(i,j,centre);
            cellules.push_back(cellule);

        }
    }

    //Génération des particules rouges et bleues
    int dim_rouge = 40;
    int dim1_bleue = 40;
    int dim2_bleue = 100;

    std::vector<Particule3D> particules_bleues;
    particules_bleues.reserve(dim1_bleue*dim2_bleue);
    std::vector<Particule3D> particules_rouges;
    particules_rouges.reserve(dim_rouge*dim_rouge);

    Vector3D vitesse_bleue(0, 0, 0);
    Vector3D vitesse_rouge(0, -10, 0);

    float distance = std::pow(2, 1.0/6.0);

    for (int i = 0; i < dim_rouge; i++) {
        for (int j = 0; j < dim_rouge; j++) {
            Particule3D particule_rouge = Particule3D(i*dim_rouge + j, 1, 1, Vector3D(0, 0, 0), Vector3D(i*distance +  dim_rouge*distance, j*distance + 1.25*dim_rouge*distance, 0), vitesse_rouge);
            // print i*distance
            // std::cout << particule_rouge.getPos().getX() << std::endl;
            particules_rouges.insert(particules_rouges.begin() + i*dim_rouge + j, particule_rouge);
        }
    }

    for  (int j = 0; j < dim1_bleue; j++)  {
        for (int i = 0; i < dim2_bleue; i++){
            Particule3D particule_bleue = Particule3D(i*dim1_bleue + j + dim_rouge*dim_rouge, 1, 0, Vector3D(0, 0, 0), Vector3D(i*distance, j*distance, 0), vitesse_bleue);
            particules_bleues.insert(particules_bleues.begin() + j*dim2_bleue + i, particule_bleue);
        }
    }

    //Assignation des paricules aux cellules

    for (const auto& particule : particules_rouges) {
        assignParticule(particule,nCellsX);
    }

    for (const auto& particule : particules_bleues) {
        assignParticule(particule,nCellsX);
    }


    // print nCd1, nCd2
    std::cout << "nCd1 : " << nCellsX << std::endl;
    std::cout << "nCd2 : " << nCellsY << std::endl;
    // print nCellsX*nCells
    std::cout << "nCellsX*nCd2 : " << nCellsX*nCellsY << std::endl;
    std :: cout <<"Nobmre dogk" << nbParticules <<std::endl;
    if (nbParticules < particules_bleues.size() + particules_rouges.size()) {
        std::cout << "Erreur dans les cellules" << std::endl;
        exit(1);
    }
}

void Univers :: assignParticule(const Particule3D& particule,int nCellsX) {
    // verifier si la particule appartient au domaine
    int cellX =(int) (particule.getPos().getX() / rCut);
    int cellY = (int) (particule.getPos().getY() / rCut);
    int index = cellX + cellY * nCellsX;
    if (index < cellules.size()) {
        cellules[index].addParticule(particule);
        nbParticules += 1;

    } else {
        std::cerr << "Computed cell index is out of bounds: " << index << std::endl;
    }

}

int Univers::getDimension() const {
    return dimension;
}

int Univers::getNbParticules() const {
    return nbParticules;
}

std::vector<Cellule> Univers::getCellules() {
    return cellules;
}

int Univers::getL1() const {
    return L1;
}

int Univers::getL2() const {
    return L2;
}

int Univers::getL3() const {
    return L3;
}

float Univers::getRCut() const {
    return rCut;
}

void Univers::setCellules(std::vector<Cellule> cellules) {
    this->cellules = cellules;
    // Mettre à jour le nombre de particules
    for (auto it = cellules.begin(); it != cellules.end(); it++) {
        this->nbParticules += it->getParticules().size();
    }
}

void Univers::writeVTKFile(std::string filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << " en écriture." << std::endl;
        exit(1);
    }
    file << "<?xml version=\"1.0\"?>" << std::endl;
    // Initialiser le fichier data_t0.vtu avec les positions, vitesses et masses des particules
    file << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">" << std::endl;
    file << "  <UnstructuredGrid>" << std::endl;
    file << "    <Piece NumberOfPoints=\"" << nbParticules << "\" NumberOfCells=\"0\">" << std::endl;
    file << "      <Points>" << std::endl;
    file << "        <DataArray name=\"Position\" type=\"Float32\" NumberOfComponents=\"" << dimension << "\" format=\"ascii\">" << std::endl;

    // Ajouter les positions des particules
    for (auto ite = cellules.begin(); ite != cellules.end(); ite++) {
        std::vector<Particule3D> particules = ite->getParticules();
        for (auto it = particules.begin(); it != particules.end(); it++) {
            Vector3D pos = it->getPos();
            if (dimension == 1) {
                file << pos.getX() << " ";
            } else if (dimension == 2) {
                file << pos.getX() << " " << pos.getY() << " ";
            } else {
                file << pos.getX() << " " << pos.getY() << " " << pos.getZ() << " ";
            }
        }
    }

    file << std::endl;
    file << "        </DataArray>" << std::endl;
    file << "      </Points>" << std::endl;
    file << "      <PointData Vectors=\"vector\">" << std::endl;
    file << "        <DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"" << dimension << "\" format=\"ascii\">" << std::endl;
    // Ajouter les vitesses des particules


    for (auto ite = cellules.begin(); ite != cellules.end(); ite++) {
        std::vector<Particule3D> particules = ite->getParticules();
        for (auto it = particules.begin(); it != particules.end(); it++) {
            Vector3D vit = it->getVit();
            if (dimension == 1) {
                file << vit.getX() << " ";
            } else if (dimension == 2) {
                file << vit.getX() << " " << vit.getY() << " ";
            } else {
                file << vit.getX() << " " << vit.getY() << " " << vit.getZ() << " ";
            }
        }
    }

    file << std::endl;
    file << "        </DataArray>" << std::endl;
    file << "        <DataArray type=\"Float32\" Name=\"Masse\" format=\"ascii\">" << std::endl;
    // Ajouter les masses des particules
    for (auto ite = cellules.begin(); ite != cellules.end(); ite++) {
        std::vector<Particule3D> particules = ite->getParticules();
        for (auto it = particules.begin(); it != particules.end(); it++) {
            float masse = it->getMasse();
            file << masse << " ";
        }
    }
    file << std::endl;
    file << "        </DataArray>" << std::endl;
    file << "      </PointData>" << std::endl;
    file << "      <Cells>" << std::endl;
    file << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">" << std::endl;
    file << "        </DataArray>" << std::endl;
    file << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">" << std::endl;
    file << "        </DataArray>" << std::endl;
    file << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">" << std::endl;
    file << "        </DataArray>" << std::endl;
    file << "      </Cells>" << std::endl;
    file << "    </Piece>" << std::endl;
    file << "  </UnstructuredGrid>" << std::endl;
    file << "</VTKFile>" << std::endl;

    file.close();
}
// The function to process cells and their neighbors
void Univers:: processCells(std::vector<Cellule> cellules, int gridWidth, int gridHeight) {
    std::vector<std::vector<int>> offsets = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (auto &cellule : cellules) {
        const int* id = cellule.getId();
        std::vector<Particule3D> particules_voisines = cellule.getParticules();

        for (auto& offset : offsets) {
            int neighborX = id[0] + offset[0];
            int neighborY = id[1] + offset[1];

            if (neighborX >= 0 && neighborX < gridWidth && neighborY >= 0 && neighborY < gridHeight) {
                auto& neighborCell = cellules[neighborX + gridWidth * neighborY];
                std::vector<Particule3D> neighborParticles = neighborCell.getParticules();
                particules_voisines.insert(particules_voisines.end(), neighborParticles.begin(), neighborParticles.end());
            }
        }

        // Output to demonstrate function
        std::cout << "Cell at (" << id[0] << ", " << id[1] << ") has " << particules_voisines.size() << " including neighbors." << std::endl;
    }
}



void Univers::calculForces(std::vector<Vector3D> & forcesOld) {
    //forcesOld.resize(nbParticules);
    std::cout << "nbParticules : " << nbParticules << std::endl;

    for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
          std::vector<Particule3D> particules = cellule->getParticules();
         for (auto p = particules.begin(); p != particules.end(); p++) {
             // stocker chaque force à la id-ième position de la liste des forcesOld

            int id = p->getId();
            if (id >= 0 && id < forcesOld.size()) {
                forcesOld[id] = p->getForce();

            } else {
                std::cerr << "Invalid particle ID: " << id << std::endl;
            }
        }
    }

    int gridWidth = L1 / rCut;
    int gridHeight = L2 / rCut;
    for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
        int* id = cellule->getId();

        std::vector<Particule3D> particules_voisines = cellule->getParticules(); // Start with current cell particles

        // Positions to check for neighboring cells
        //let (nx,ny) be the offset , if we look at (nx,ny) ,we don't need to look at (-nx,-ny)
        // this will solve the problem double counting interactions
        std::vector<std::vector<int>> offsets = {{1, 0}, {0, 1}, {1,1},{1,-1}};

        for (auto& offset : offsets) {
            int neighborX = id[0] + offset[0];
            int neighborY = id[1] + offset[1];

            // Check if the neighbor coordinates are within the grid bounds
            if (neighborX >= 0 && neighborX < gridWidth && neighborY >= 0 && neighborY < gridHeight) {
                    auto & neighborCell = cellules[neighborX + gridWidth*neighborY];
                    // Add all particles from this neighbor to the list of neighboring particles
                    std::vector<Particule3D> neighborParticles = neighborCell.getParticules();
                    particules_voisines.insert(particules_voisines.end(), neighborParticles.begin(), neighborParticles.end());

            }
        }


        std::vector<Particule3D> particules = cellule->getParticules();
        for (auto &p1 : particules) {

            Vector3D force_1 = p1.getForce();
            const double masse_i = p1.getMasse();
            const Vector3D pos_i = p1.getPos();

            for (auto &p2 : particules_voisines) {


                const double masse_j = p2.getMasse();
                const Vector3D pos_j = p2.getPos();
                Vector3D force_2 = p2.getForce();
                const Vector3D r = pos_i - pos_j;
                const double norme_r = r.norm();

                if (norme_r != 0.0 ) { // Avoid division by zero
                    //Force gravitationnelle
                    force_1 += r * (masse_i * masse_j / (norme_r * norme_r * norme_r));
                    force_2 += r * (masse_i * masse_j / (norme_r * norme_r * norme_r)) * (-1);
                    //Force d'intercation
                    double powTo6 = std::pow(sigma/norme_r,6);

                    force_1 += r * 24 * eps  * std::pow(1/norme_r,2) * powTo6  *(1-2*powTo6) ;
                    force_2 += r * 24 * eps  * std::pow(1/norme_r,2) * powTo6  *(1-2*powTo6)* (-1);

                    if (force_1.getX()>1e5 ) {
                        force_1.setX(1e5) ;
                    }
                    if (force_1.getY()>1e5) {
                        force_1.setY(1e5);
                    }
                    if (force_1.getX()<-1e5 ) {
                        force_1.setX(-1e5) ;
                    }
                    if (force_1.getY()<-1e5) {
                        force_1.setY(-1e5);
                    }
                    if (force_2.getX()>1e5 ) {
                        force_2.setX(1e5) ;
                    }
                    if (force_2.getY()>1e5) {
                        force_2.setY(1e5);
                    }
                    if (force_2.getX()<-1e5 ) {
                        force_2.setX(-1e5) ;
                    }
                    if (force_2.getY()<-1e5) {
                        force_2.setY(-1e5);
                    }

                }
                p2.setForce(force_2);
            }
            p1.setForce(force_1);

        }
        cellule->setParticules(particules);
    }




}

void Univers ::absorptionBC() {
    std ::cout <<"le sizeeeeeeee : "<<cellules.begin()->getParticules().size()<<std::endl;
        for (auto& cellule : cellules) {
            auto& particules =  cellule.getParticules();

            for (int i = (int)particules.size() - 1 ;i>=0 ;--i) {
                auto & p = particules[i];
                //Check the boundary conditions
                if (p.getPos().getX() < 0 || p.getPos().getX() > L1 ||
                    p.getPos().getY() < 0 || p.getPos().getY() > L2) {
                    std::cout << "Removing particle at index: " << i << std::endl;
                    cellule.removeParticule(i);
                    nbParticules--;

                }
            }

        }



}

void Univers::reassignCells() {
    // Collect all particles from all cells
    std::vector<Particule3D> allParticules;
    for (auto& cellule : cellules) {
        auto particules = cellule.getParticules();
        allParticules.insert(allParticules.end(), particules.begin(), particules.end());
    }

    // Clear all cells
    for (auto& cellule : cellules) {
        cellule.clearParticules();
    }


    // Reassign particles to the correct cells based on their positions

    for (auto& p : allParticules) {
            int cellX = (int)(p.getPos().getX() / rCut);
            int cellY = (int)(p.getPos().getY() / rCut);

            if (cellX >= 0 && cellX < cellules.size() && cellY >= 0 && cellY < cellules.size()) {
                int newCellIndex = cellX + cellY * (L1 / rCut);
                cellules[newCellIndex].addParticule(p);
            } else {
                std::cerr << "Particle out of bounds: " << p.getId() << std::endl;
            }
        }

}



void Univers::evolution() {


    std::string filename = "data_t0.vtu";
    writeVTKFile(filename);

    // Implémentation de l'algorithme de Verlet    
    // Stocker les forces des particules de la classe Univers dans une liste de vecteurs de forces
    std::vector<Vector3D> forcesOld;
    forcesOld.resize(nbParticules);
    std :: cout << " nombre de particules   :      " << nbParticules << std ::endl ;
    calculForces(forcesOld);

    // Initialisation du temps
    float t = 0;
    int file_index = 0;
    while (t < tmax) {
        std::cout << "t = " << t << std::endl;  
        file_index++;
        t += dt;

        // Mise à jour des positions
        for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
            std::vector<Particule3D> particules = cellule->getParticules();
            for (auto p = particules.begin(); p != particules.end(); p++) {
                Vector3D pos = p->getPos();
                // print the id and the position of the particle 0
                Vector3D vit = p->getVit();
                float masse = p->getMasse();
                Vector3D force = p->getForce();
                pos += (vit + force * dt * (0.5 / masse)) * dt;
                p->setPos(pos);

            }

            // à revoir si c'est utile
            cellule->setParticules(particules);
        }

        //Mise à jour des cellules
        //1. condition d'absorption
        absorptionBC();
        reassignCells();

        if (t == dt) {
            std ::cout <<"le sizeeeeeeee endddddddd : "<<cellules.begin()->getParticules().size()<<std::endl;

        }


    calculForces(forcesOld);

        for (auto& cellule : cellules) {
        std::vector<Particule3D> particules = cellule.getParticules();
        for (auto &p : particules) {
            Vector3D vit = p.getVit();
            float masse = p.getMasse();
            Vector3D force = p.getForce();
            Vector3D forceOld = forcesOld[p.getId()];
            vit = vit + (force + forceOld) * dt * (0.5 / masse);
            p.setVit(vit.getX(), vit.getY(), vit.getZ());
        }
         cellule.setParticules(particules);
    }




    // remplir le fichier data_t.vtu avec les nouvelles positions, vitesses et masses des particules
    std::string filename = "data_t" + std::to_string(file_index) + ".vtu";
    writeVTKFile(filename);
    // print le pourcentage de l'évolution
    std::cout << "Pourcentage de l'évolution : " << (t -dt) / tmax * 100 << "%" << std::endl;


    }

    std::cout << "Evolution terminée" << std::endl;
    }


