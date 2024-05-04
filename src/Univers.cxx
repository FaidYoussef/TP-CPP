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
}

Univers::Univers() : dimension(0), nbParticules(0), cellules(std::vector<Cellule>()) {}

void Univers::initialiser() {
    int dim_rouge = 4;
    int dim1_bleue = 4;
    int dim2_bleue = 16;

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
    for (auto& cellule : cellules) {
        nbParticules += cellule.getNbParticules();
    }
    if (nbParticules < particules_bleues.size() + particules_rouges.size()) {
        std::cout << "Erreur dans les cellules" << std::endl;
        exit(1);
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
    std::vector<Vector3D> forces;
    forces.reserve(nbParticules);

    // parcourir les particules de chaque cellule de la classe Univers
    for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
        std::vector<Particule3D> particules = cellule->getParticules();
        for (auto p = particules.begin(); p != particules.end(); p++) {
            // stocker chaque force à la id-ième position de la liste des forcesOld
            forcesOld.insert(forcesOld.begin() + p->getId(), p->getForce());
        }
    }

    // Calcul des forces
    for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
        int* id = cellule->getId();
        std::vector<Particule3D> particules_voisines = cellule->getParticules();
        // parcourir les cellules de la classe Univers ( A REVOIR!!!!!!!!!)
        for (auto autre_cellule = cellules.begin(); autre_cellule != cellules.end(); autre_cellule++) {
            int* id2 = autre_cellule->getId();
            if (id[0] == id2[0] + 1 || id[0] == id2[0] -1 || id[1] == id2[1] + 1 || id[1] == id2[1] - 1) {
                // inserer les particules de it_l dans la liste des particules voisines
                std::vector<Particule3D> particules2 = autre_cellule->getParticules();
                for (auto it_m = particules2.begin(); it_m != particules2.end(); it_m++) {
                    particules_voisines.push_back(*it_m);
                }
            }
        }

        std::vector<Particule3D> particules = cellule->getParticules();
        for (auto p1 = particules.begin(); p1 != particules.end(); p1++) {
            
            Vector3D force_ij;
            const double masse_i = p1->getMasse();
            const Vector3D pos_i = p1->getPos();

            for (auto p2 = particules_voisines.begin(); p2 != particules_voisines.end(); p2++) {
                const double masse_j = p2->getMasse();
                const Vector3D pos_j = p2->getPos();
                const Vector3D r = pos_i - pos_j;
                const double norme_r = r.norm(); 

                    if (norme_r != 0.0) { // Avoid division by zero
                        force_ij += r * (masse_i * masse_j / (norme_r * norme_r * norme_r));
                    }
                }
            forces.insert(forces.begin() + p1->getId(), force_ij);
            }
        }
    
    // Initialisation du temps
    float t = 0.;
    int file_index = 0;
    while (t < tmax) {
        file_index++;
        t += dt;
        for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
            std::vector<Particule3D> particules = cellule->getParticules();
            for (auto p = particules.begin(); p != particules.end(); p++) {
                Vector3D pos = p->getPos();
                // print the id and the position of the particle 0
                Vector3D vit = p->getVit();
                float masse = p->getMasse();
                Vector3D force = *(std::next(forces.begin(), p->getId()));
                pos += (vit + force * dt * (0.5 / masse)) * dt;
                p->setPos(pos);
            }
            // à revoir si c'est utile
            //ite->setParticules(particules);
        }

        // Mise à jour des cellules
        for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
            std::vector<Particule3D> particules = cellule->getParticules();
            for (auto p = particules.begin(); p != particules.end(); p++) {
                Vector3D pos = p->getPos();
                Vector3D centre = cellule->getCentre();

                if (std::abs(pos.getX() - centre.getX()) > rCut / 2. || std::abs(pos.getY() - centre.getY()) > rCut / 2. || std::abs(pos.getZ() - centre.getZ()) > rCut / 2.) {
                    // Supprimer la particule de la cellule
                    cellule->removeParticule(*p);
                    // Ajouter la particule à la cellule correspondante
                    // Calculer le centre le plus proche
                    //Vector3D centreProche = Vector3D(std::floor(pos.getX() / rCut) * rCut + 0.5*rCut, std::floor(pos.getY() / rCut) * rCut + 0.5*rCut, std::round(pos.getZ() / rCut) * rCut + 0.5*rCut);
                    //for (auto& cellule2 : cellules) {
                      //  if (cellule2.getCentre() == centreProche) {
                     //       cellule2.addParticule(*it);
                    //        break;
                    //    }
                    //}
                    if (pos.getX()>=0 && pos.getX()<=L1 && pos.getY()>=0 && pos.getX()<=L2) {
                       int index1 = std::ceil(pos.getX() / rCut);
                       int index2 = std::ceil(pos.getY() / rCut);
                       cellules[index1 + index2 * (L1 / rCut)].addParticule(*p);
                    }


                }
            }
        }

        // stocker forces dans forcesOld
        forcesOld = forces;

        // Calcul des forces
        // forces.clear(); // Clear la liste des forces
            // parcourir les particules de chaque cellule de la classe Univers
    //for (auto it = cellules.begin(); it != cellules.end(); it++) {
     //   std::vector<Particule3D> particules = it->getParticules();
      //  for (auto it = particules.begin(); it != particules.end(); it++) {
      //      // stocker chaque force à la id-ième position de la liste des forcesOld
       //     forcesOld[it->getId()] = it->getForce();
       // }
    //}

    //std::vector<Vector3D> forces;
        forces.clear();
        // Calcul des forces
        for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
            int* id = cellule->getId();
            std::vector<Particule3D> particules_voisines = cellule->getParticules();
            // parcourir les cellules de la classe Univers ( A REVOIR!!!!!!!!!)
            for (auto autre_cellule = cellules.begin(); autre_cellule != cellules.end(); autre_cellule++) {
                int* id2 = autre_cellule->getId();
                if (id[0] == id2[0] + 1 || id[0] == id2[0] -1 || id[1] == id2[1] + 1 || id[1] == id2[1] - 1) {
                    // inserer les particules de it_l dans la liste des particules voisines
                    std::vector<Particule3D> particules2 = autre_cellule->getParticules();
                    for (auto it_m = particules2.begin(); it_m != particules2.end(); it_m++) {
                        particules_voisines.push_back(*it_m);
                    }
                }
            }

            std::vector<Particule3D> particules = cellule->getParticules();
            for (auto p1 = particules.begin(); p1 != particules.end(); p1++) {
                
                Vector3D force_ij;
                const double masse_i = p1->getMasse();
                const Vector3D pos_i = p1->getPos();

                for (auto p2 = particules_voisines.begin(); p2 != particules_voisines.end(); p2++) {
                    const double masse_j = p2->getMasse();
                    const Vector3D pos_j = p2->getPos();
                    const Vector3D r = pos_i - pos_j;
                    const double norme_r = r.norm(); 

                        if (norme_r != 0.0) { // Avoid division by zero
                            force_ij += r * (masse_i * masse_j / (norme_r * norme_r * norme_r));
                        }
                    }
                forces.insert(forces.begin() + p1->getId(), force_ij);
                }
            }
            

        for (auto cellule = cellules.begin(); cellule != cellules.end(); cellule++) {
            std::vector<Particule3D> particules = cellule->getParticules();
            for (auto p = particules.begin(); p != particules.end(); p++) {
                Vector3D vit = p->getVit();
                float masse = p->getMasse();
                Vector3D force = *(std::next(forces.begin(), p->getId()));
                Vector3D forceOld = *(std::next(forcesOld.begin(), p->getId()));
                vit = vit + (force + forceOld) * dt * (0.5 / masse);
                p->setVit(vit.getX(), vit.getY(), vit.getZ());
            }
            // cellule->setParticules(particules);
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