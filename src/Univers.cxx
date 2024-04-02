#include "Univers.hxx"
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Cellule.hxx"



Univers::Univers(int dimension, std::vector<Cellule> cellules, int L1, int L2, int L3, float rCut, float dt, float tmax) {
    if (dimension < 1 || dimension > 3) {
        std::cerr << "Dimension invalide" << std::endl;
        exit(1);
    }
    this->dimension = dimension;
    this->cellules = cellules;
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
    // nbParticules est le nombre total de particules dans l'univers
    for (auto it = cellules.begin(); it != cellules.end(); it++) {
        this->nbParticules += it->getParticules().size();
    }

    this->dt = dt;
    this->tmax = tmax;
}

Univers::Univers() : dimension(0), nbParticules(0), cellules(std::vector<Cellule>()) {}

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

void Univers::evolution() {

    // Nombre maximal de fichiers à créer (data_t0.vtu jusqu'à data_tmax.vtu)
    int maxFiles = (tmax-t0) / dt; 


    // Créer les fichiers data_t0.vtu jusqu'à data_tmax.vtu
    for (int t = 0; t <= maxFiles; ++t) {
        // Nom du fichier à créer pour l'instant t
        std::string newFilename = "data_t" + std::to_string(t) + ".vtu";

        // Ouvrir le nouveau fichier en écriture
        std::ofstream newFile(newFilename);
        if (!newFile.is_open()) {
            std::cerr << "Erreur : Impossible de créer le fichier " << newFilename << " en écriture." << std::endl;
            exit(1);
        }

        // Fermer le nouveau fichier
        newFile.close();
    }

    std::string filename = "data_t0.vtu";
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



    std::cout << nbParticules << std::endl;

    // Implémentation de l'algorithme de Verlet
    

    // Stocker les forces des particules de la classe Univers dans une liste de vecteurs de forces
    std::vector<Vector3D> forcesOld;
    forcesOld.reserve(nbParticules);
    // parcourir les particules de chaque cellule de la classe Univers
    for (auto it = cellules.begin(); it != cellules.end(); it++) {
        std::vector<Particule3D> particules = it->getParticules();
        for (auto it = particules.begin(); it != particules.end(); it++) {
            // stocker chaque force à la id-ième position de la liste des forcesOld
            forcesOld[it->getId()] = it->getForce();
        }
    }
    std::vector<Vector3D> forces;
    forces.reserve(nbParticules);
    // Calcul des forces
    for (auto it = cellules.begin(); it != cellules.end(); it++) {
        std::vector<Particule3D> particules = it->getParticules();

        for (auto it_k = particules.begin(); it_k != particules.end(); it_k++) {
            std::vector<Particule3D> particules_voisines;
            Particule3D particule_k = *it_k;
            Vector3D pos = particule_k.getPos();
            // parcourir les cellules de la classe Univers
            for (auto it_l = cellules.begin(); it_l != cellules.end(); it_l++) {
                Vector3D centre = it_l->getCentre();
                Vector3D diff = pos - centre;
                double norme_diff = diff.norm();
                if (norme_diff < rCut) {
                    // inserer les particules de it_l dans la liste des particules voisines
                    std::vector<Particule3D> particules2 = it_l->getParticules();
                    for (auto it_m = particules2.begin(); it_m != particules2.end(); it_m++) {
                        particules_voisines.push_back(*it_m);
                    }
                }
            }
            
            Vector3D force_ij;
            for (auto it_n = particules_voisines.begin(); it_n != particules_voisines.end(); it_n++) {
                const double masse_i = it_n->getMasse();
                const Vector3D pos_i = it_n->getPos();

                for (auto it_j = std::next(it_n); it_j != particules_voisines.end(); it_j++) {
                    const double masse_j = it_j->getMasse();
                    const Vector3D pos_j = it_j->getPos();
                    const Vector3D r = pos_i - pos_j;
                    const double norme_r = r.norm(); 

                    if (norme_r != 0.0) { // Avoid division by zero
                        force_ij += r * (masse_i * masse_j / (norme_r * norme_r * norme_r));
                    }
                }
            }
            forces[it_k->getId()] = force_ij;
            // print force_ij
            // std::cout << "force_ij = " << force_ij.getX() << " " << force_ij.getY() << " " << force_ij.getZ() << std::endl;                
        }
    }
    
    // Initialisation du temps
    float t = 0.;
    int file_index = 0;
    while (t < tmax) {
        file_index++;
        t += dt;
        for (auto ite = cellules.begin(); ite != cellules.end(); ite++) {
            std::vector<Particule3D> particules = ite->getParticules();
            for (auto it = particules.begin(); it != particules.end(); it++) {
                Vector3D pos = it->getPos();
                // print the id and the position of the particle 0
                Vector3D vit = it->getVit();
                float masse = it->getMasse();
                Vector3D force = *(std::next(forces.begin(), it->getId()));
                pos += (vit + force * dt * (0.5 / masse)) * dt;
                it->setPos(pos);
            }
            ite->setParticules(particules);
        }

        // Mise à jour des cellules
        for (auto cellule : cellules) {
            std::vector<Particule3D> particules = cellule.getParticules();
            for (auto it = particules.begin(); it != particules.end(); it++) {
                Vector3D pos = it->getPos();
                Vector3D centre = cellule.getCentre();
                if (std::abs(pos.getX() - centre.getX()) > rCut / 2. || std::abs(pos.getY() - centre.getY()) > rCut / 2. || std::abs(pos.getZ() - centre.getZ()) > rCut / 2.) {
                    // Supprimer la particule de la cellule
                    cellule.removeParticule(*it);
                    // Ajouter la particule à la cellule correspondante
                    // Calculer le centre le plus proche
                    Vector3D centreProche = Vector3D(std::round(pos.getX() / rCut) * rCut, std::round(pos.getY() / rCut) * rCut, std::round(pos.getZ() / rCut) * rCut);
                    for (auto& cellule2 : cellules) {
                        if (cellule2.getCentre() == centreProche) {
                            cellule2.addParticule(*it);
                            break;
                        }
                    }
                }
            }
        }

        // stocker forces dans forcesOld
        forcesOld = forces;

        // Calcul des forces
        // forces.clear(); // Clear la liste des forces
        for (auto it = cellules.begin(); it != cellules.end(); it++) {
            std::vector<Particule3D> particules = it->getParticules();

            for (auto it_k = particules.begin(); it_k != particules.end(); it_k++) {
                std::vector<Particule3D> particules_voisines;
                Particule3D particule_k = *it_k;
                Vector3D pos = particule_k.getPos();
                // parcourir les cellules de la classe Univers
                for (auto it_l = cellules.begin(); it_l != cellules.end(); it_l++) {
                    Vector3D centre = it_l->getCentre();
                    Vector3D diff = pos - centre;
                    double norme_diff = diff.norm();
                    if (norme_diff < rCut) {
                        // inserer les particules de it_l dans la liste des particules voisines
                        std::vector<Particule3D> particules2 = it_l->getParticules();
                        for (auto it_m = particules2.begin(); it_m != particules2.end(); it_m++) {
                            particules_voisines.push_back(*it_m);
                        }
                    }
                }
                // parcourir les cellules voisines
                Vector3D force_ij;
                for (auto it_n = particules_voisines.begin(); it_n != particules_voisines.end(); it_n++) {
                    const double masse_i = it_n->getMasse();
                    const Vector3D pos_i = it_n->getPos();

                    for (auto it_j = std::next(it_n); it_j != particules_voisines.end(); it_j++) {
                        const double masse_j = it_j->getMasse();
                        const Vector3D pos_j = it_j->getPos();
                        const Vector3D r = pos_i - pos_j;
                        const double norme_r = r.norm(); 

                        if (norme_r != 0.0) { // Avoid division by zero
                            force_ij += r * (masse_i * masse_j / (norme_r * norme_r * norme_r));
                        }
                    }
                }    

            forces[it_k->getId()] = force_ij;
            // print I am here
            // std::cout << "I am here"<< std::endl;
            // print force_ij
            // std::cout << "force_ij = " << force_ij.getX() << " " << force_ij.getY() << " " << force_ij.getZ() << std::endl;                
        }
    }

        for (auto ite = cellules.begin(); ite != cellules.end(); ite++) {
            std::vector<Particule3D> particules = ite->getParticules();
            for (auto it = particules.begin(); it != particules.end(); it++) {
                Vector3D vit = it->getVit();
                float masse = it->getMasse();
                Vector3D force = *(std::next(forces.begin(), it->getId()));
                Vector3D forceOld = *(std::next(forcesOld.begin(), it->getId()));
                vit = vit + (force + forceOld) * dt * (0.5 / masse);
                it->setVit(vit.getX(), vit.getY(), vit.getZ());
            }
            ite->setParticules(particules);
        }

        // remplir le fichier data_t.vtu avec les nouvelles positions, vitesses et masses des particules
        std::string filename = "data_t" + std::to_string(file_index) + ".vtu";
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << " en écriture." << std::endl;
            exit(1);
        }
        // Initialiser le fichier data_t.vtu avec les positions, vitesses et masses des particules
        file << "<?xml version=\"1.0\"?>" << std::endl;
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

        // print le pourcentage de l'évolution
        std::cout << "Pourcentage de l'évolution : " << t / tmax * 100 << "%" << std::endl;
        }

        std::cout << "Evolution terminée" << std::endl;
    }