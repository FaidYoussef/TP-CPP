#include "Univers.hxx"
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "Cellule.hxx"
#include "Particule3D.hxx"

// Helper function for error logging
void logError(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
}

/**
 * @brief Constructs a Univers object.
 *
 * @param dimension The dimension of the simulation (1, 2, or 3).
 * @param L1 The size of the simulation box in the x-direction.
 * @param L2 The size of the simulation box in the y-direction.
 * @param L3 The size of the simulation box in the z-direction.
 * @param rCut The cutoff radius for interactions.
 * @param dt The time step for the simulation.
 * @param tmax The maximum simulation time.
 */
Univers::Univers(int dimension, int L1, int L2, int L3, float rCut, float dt, float tmax) {
    if (dimension < 1 || dimension > 3) {
        throw std::invalid_argument("Invalid dimension: Dimension must be 1, 2, or 3.");
    }
    if (L1 <= 0 || L2 < 0 || L3 < 0 || rCut <= 0 || dt <= 0 || tmax <= 0) {
        throw std::invalid_argument("Invalid parameters: Sizes and time values must be positive.");
    }
    this->dimension = dimension;
    this->cellules = std::vector<Cellule>();
    this->rCut = rCut;
    this->L1 = L1;
    this->L2 = (dimension >= 2) ? L2 : 0;
    this->L3 = (dimension == 3) ? L3 : 0;
    this->nbParticules = 0;
    this->dt = dt;
    this->tmax = tmax;
    this->eps = 0;
    this->sigma = 0;
}

/**
 * @brief Constructs a Univers object with Lennard-Jones parameters.
 *
 * @param dimension The dimension of the simulation (1, 2, or 3).
 * @param L1 The size of the simulation box in the x-direction.
 * @param L2 The size of the simulation box in the y-direction.
 * @param L3 The size of the simulation box in the z-direction.
 * @param eps The depth of the Lennard-Jones potential well.
 * @param sigma The finite distance at which the inter-particle potential is zero.
 * @param rCut The cutoff radius for interactions.
 * @param dt The time step for the simulation.
 * @param tmax The maximum simulation time.
 */
Univers::Univers(int dimension, int L1, int L2, int L3, int eps, int sigma, float rCut, float dt, float tmax) {
    if (dimension < 1 || dimension > 3) {
        throw std::invalid_argument("Invalid dimension: Dimension must be 1, 2, or 3.");
    }
    if (L1 <= 0 || L2 < 0 || L3 < 0 || eps <= 0 || sigma <= 0 || rCut <= 0 || dt <= 0 || tmax <= 0) {
        throw std::invalid_argument("Invalid parameters: Sizes, potentials, and time values must be positive.");
    }
    this->dimension = dimension;
    this->cellules = std::vector<Cellule>();
    this->rCut = rCut;
    this->L1 = L1;
    this->L2 = (dimension >= 2) ? L2 : 0;
    this->L3 = (dimension == 3) ? L3 : 0;
    this->nbParticules = 0;
    this->dt = dt;
    this->tmax = tmax;
    this->sigma = sigma;
    this->eps = eps;
}

/**
 * @brief Constructs a Univers object with Lennard-Jones parameters and additional physical parameters.
 *
 * @param dimension The dimension of the simulation (1, 2, or 3).
 * @param L1 The size of the simulation box in the x-direction.
 * @param L2 The size of the simulation box in the y-direction.
 * @param L3 The size of the simulation box in the z-direction.
 * @param eps The depth of the Lennard-Jones potential well.
 * @param sigma The finite distance at which the inter-particle potential is zero.
 * @param rCut The cutoff radius for interactions.
 * @param dt The time step for the simulation.
 * @param tmax The maximum simulation time.
 * @param boundaryCond The type of boundary conditions to apply.
 * @param G The gravitational constant.
 * @param scaleType the type of scaling to apply : 0 using for maxForce ; 1 using kinetic energy
 */
Univers::Univers(int dimension, int L1, int L2, int L3, int eps, int sigma, float rCut, float dt, float tmax, int boundaryCond, float G, int scaleType) {
    if (dimension < 1 || dimension > 3) {
        throw std::invalid_argument("Invalid dimension: Dimension must be 1, 2, or 3.");
    }
    if (L1 <= 0 || L2 < 0 || L3 < 0 || eps <= 0 || sigma <= 0 || rCut <= 0 || dt <= 0 || tmax <= 0 || boundaryCond < 0 || boundaryCond > 2) {
        throw std::invalid_argument("Invalid parameters: Sizes, potentials, time values, and boundary conditions must be positive.");
    }
    this->dimension = dimension;
    this->cellules = std::vector<Cellule>();
    this->rCut = rCut;
    this->L1 = L1;
    this->L2 = (dimension >= 2) ? L2 : 0;
    this->L3 = (dimension == 3) ? L3 : 0;
    this->nbParticules = 0;
    this->dt = dt;
    this->tmax = tmax;
    this->sigma = sigma;
    this->eps = eps;
    this->boundaryCond = boundaryCond;
    this->G = G;
    this->scaleType = scaleType;
}

/**
 * @brief Default constructor for Univers class.
 */
Univers::Univers() : dimension(0), nbParticules(0), cellules(std::vector<Cellule>()) {}

/**
 * @brief Initializes the simulation with specific dimensions and velocities.
 *
 * @param dim1_rouge The first dimension of the red particles grid.
 * @param dim2_rouge The second dimension of the red particles grid.
 * @param dim1_bleue The first dimension of the blue particles grid.
 * @param dim2_bleue The second dimension of the blue particles grid.
 * @param vitesse_bleue The initial velocity of the blue particles.
 * @param vitesse_rouge The initial velocity of the red particles.
 */
void Univers::initialiser(int dim1_rouge, int dim2_rouge, int dim1_bleue, int dim2_bleue, const Vector3D& vitesse_rouge, const Vector3D& vitesse_bleue) {
    int nCellsX = L1 / rCut;
    int nCellsY = L2 / rCut;

    try {
        cellules.reserve(nCellsX * nCellsY);

        // Create all cells
        for (int j = 0; j < nCellsY; j++) {
            for (int i = 0; i < nCellsX; i++) {
                Vector3D centre((i + 0.5) * rCut, (j + 0.5) * rCut, 0);
                Cellule cellule(i, j, centre);
                cellules.push_back(cellule);
            }
        }

        std::vector<Particule3D> particules_bleues;
        particules_bleues.reserve(dim1_bleue * dim2_bleue);
        std::vector<Particule3D> particules_rouges;
        particules_rouges.reserve(dim1_rouge * dim2_rouge);

        float distance = std::pow(2, 1.0 / 6.0);

        // Initialize red particles in a grid
        for (int j = 0; j < dim1_rouge; j++) {
            for (int i = 0; i < dim2_rouge; i++) {
                Particule3D particule_rouge = Particule3D(j * dim2_rouge + i, 1, 1, Vector3D(0, 0, 0), Vector3D(i * distance + dim1_bleue * distance, j * distance + dim1_bleue * distance + 5, 0), vitesse_rouge);
                particules_rouges.push_back(particule_rouge);
            }
        }

        // Initialize blue particles in a grid
        for (int j = 0; j < dim1_bleue; j++) {
            for (int i = 0; i < dim2_bleue; i++) {
                Particule3D particule_bleue = Particule3D(j * dim2_bleue + i + dim1_rouge * dim2_rouge, 1, 0, Vector3D(0, 0, 0), Vector3D(i * distance, j * distance, 0), vitesse_bleue);
                particules_bleues.push_back(particule_bleue);
            }
        }

        // Assign particles to cells
        for (const auto& particule : particules_rouges) {
            assignParticule(particule, nCellsY);
        }

        for (const auto& particule : particules_bleues) {
            assignParticule(particule, nCellsY);
        }


        if (nbParticules < (int)(particules_bleues.size() + particules_rouges.size())) {
            throw std::runtime_error("Error in cells: Number of particles is less than expected.");
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Initializes the simulation with a rectangular grid of blue particles and a circular cluster of red particles.
 *
 * @param dim1_bleue The first dimension of the blue particles grid.
 * @param dim2_bleue The second dimension of the blue particles grid.
 * @param rayon_rouge The radius of the red particles cluster.
 * @param vitesse_bleue The initial velocity of the blue particles.
 * @param vitesse_rouge The initial velocity of the red particles.
 */
void Univers::initialiserDemoCercle(int dim1_bleue, int dim2_bleue, float rayon_rouge, const Vector3D& vitesse_bleue, const Vector3D & vitesse_rouge) {
    int nCellsX = L1 / rCut;
    int nCellsY = L2 / rCut;

    try {
        cellules.reserve(nCellsX * nCellsY);

        // Create all cells
        for (int j = 0; j < nCellsY; j++) {
            for (int i = 0; i < nCellsY; i++) {
                Vector3D centre((i + 0.5) * rCut, (j + 0.5) * rCut, 0);
                Cellule cellule(i, j, centre);
                cellules.push_back(cellule);
            }
        }

        std::vector<Particule3D> particules_bleues;
        particules_bleues.reserve(dim1_bleue * dim2_bleue);
        std::vector<Particule3D> particules_rouges;

        float distance = std::pow(2, 1.0 / 6.0);

        // Initialize red particles in a disk
        int num_particules_rouges = static_cast<int>(M_PI * std::pow(rayon_rouge / distance, 2));

        particules_rouges.reserve(num_particules_rouges);

        for (int i = 0; i < num_particules_rouges; i++) {
            float r = rayon_rouge * std::sqrt(static_cast<float>(rand()) / RAND_MAX);
            float theta = 2 * M_PI * static_cast<float>(rand()) / RAND_MAX;
            float x = r * cos(theta) + distance * dim2_bleue / 2;
            float y = r * sin(theta) + distance * dim1_bleue + 2 * r;
            Particule3D particule_rouge = Particule3D(i + dim1_bleue * dim2_bleue, 1, 1, Vector3D(0, 0, 0), Vector3D(x, y, 0), vitesse_rouge);
            particules_rouges.push_back(particule_rouge);
        }

        // Initialize blue particles in a rectangle
        for (int j = 0; j < dim1_bleue; j++) {
            for (int i = 0; i < dim2_bleue; i++) {
                Particule3D particule_bleue = Particule3D(i + j * dim2_bleue, 1, 0, Vector3D(0, 0, 0), Vector3D(i * distance, j * distance, 0), vitesse_bleue);
                particules_bleues.push_back(particule_bleue);
            }
        }

        // Assign particles to cells
        for (const auto& particule : particules_bleues) {
            assignParticule(particule, nCellsX);
        }

        for (const auto& particule : particules_rouges) {
            assignParticule(particule, nCellsX);
        }

        std::cout << "nCd1 : " << nCellsX << std::endl;
        std::cout << "nCd2 : " << nCellsY << std::endl;
        std::cout << "nCellsX * nCd2 : " << nCellsX * nCellsY << std::endl;
        std::cout << "Number of particles: " << nbParticules << std::endl;
        if (nbParticules < (int)(particules_bleues.size() + particules_rouges.size())) {
            throw std::runtime_error("Error in cells: Number of particles is less than expected.");
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Gets the dimension of the simulation.
 *
 * @return The dimension of the simulation.
 */
int Univers::getDimension() const {
    return dimension;
}

/**
 * @brief Gets the number of particles in the simulation.
 *
 * @return The number of particles.
 */
int Univers::getNbParticules() const {
    return nbParticules;
}

/**
 * @brief Gets the cells in the simulation.
 *
 * @return A vector of cells.
 */
std::vector<Cellule> Univers::getCellules() {
    return cellules;
}

/**
 * @brief Gets the size of the simulation box in the x-direction.
 *
 * @return The size in the x-direction.
 */
int Univers::getL1() const {
    return L1;
}

/**
 * @brief Gets the size of the simulation box in the y-direction.
 *
 * @return The size in the y-direction.
 */
int Univers::getL2() const {
    return L2;
}

/**
 * @brief Gets the size of the simulation box in the z-direction.
 *
 * @return The size in the z-direction.
 */
int Univers::getL3() const {
    return L3;
}

/**
 * @brief Gets the cutoff radius for interactions.
 *
 * @return The cutoff radius.
 */
float Univers::getRCut() const {
    return rCut;
}

/**
 * @brief Sets the cells in the simulation and updates the particle count.
 *
 * @param cellules A vector of cells.
 */
void Univers::setCellules(std::vector<Cellule> cellules) {
    this->cellules = cellules;
    this->nbParticules = 0;
    // Update the number of particles
    for (auto it = cellules.begin(); it != cellules.end(); it++) {
        this->nbParticules += it->getParticules().size();
    }
}

/**
 * @brief Assigns a particle to a cell based on its position.
 *
 * @param particule The particle to assign.
 * @param nCellsX The number of cells in the x-direction.
 */
void Univers::assignParticule(const Particule3D& particule, int nCellsX) {
    try {
        int cellX = (int)(particule.getPos().getX() / rCut);
        int cellY = (int)(particule.getPos().getY() / rCut);
        int index = cellX + cellY * nCellsX;
        if (index >= 0 && index < (int)cellules.size()) {
            cellules[index].addParticule(particule);
            nbParticules += 1;
        } else {
            std::ostringstream oss;
            oss << "Computed cell index is out of bounds: " << index << " for particle ID: " << particule.getId();
            throw std::out_of_range(oss.str());
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Writes the current state of the simulation to a VTK file.
 *
 * @param filename The name of the file to write.
 */
void Univers::writeVTKFile(std::string filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file " + filename + " for writing.");
        }
        file << "<?xml version=\"1.0\"?>" << std::endl;
        file << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">" << std::endl;
        file << "  <UnstructuredGrid>" << std::endl;
        file << "    <Piece NumberOfPoints=\"" << nbParticules << "\" NumberOfCells=\"0\">" << std::endl;
        file << "      <Points>" << std::endl;
        file << "        <DataArray name=\"Position\" type=\"Float32\" NumberOfComponents=\"" << dimension << "\" format=\"ascii\">" << std::endl;

        // Add particle positions
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
        // Add particle velocities
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
        file << "        <DataArray type=\"Float32\" Name=\"Category\" format=\"ascii\">" << std::endl;

        // Add particle categories
        for (auto ite = cellules.begin(); ite != cellules.end(); ite++) {
            std::vector<Particule3D> particules = ite->getParticules();
            for (auto it = particules.begin(); it != particules.end(); it++) {
                float categorie = it->getCategorie();
                file << categorie << " ";
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
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Calculates the forces on each particle using the Lennard-Jones potential and gravitational forces.
 *
 */
void Univers::calculForces() {
    try {
        int gridWidth = L1 / rCut;
        int gridHeight = L2 / rCut;

        for (auto &cellule : cellules) {
            int *id = cellule.getId();
            int x = id[0];
            int y = id[1];

            // List of neighboring particles
            std::vector<Particule3D> particules_voisines;
            std::vector<Particule3D> &part = cellule.getParticules();

            // Interactions with the neighboring cells
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int neighborX = x + dx;
                    int neighborY = y + dy;

                    // Check if the neighbor coordinates are within the grid bounds
                    if (neighborX >= 0 && neighborX < gridWidth && neighborY >= 0 && neighborY < gridHeight) {
                        auto &neighborCell = cellules[neighborX + neighborY * gridWidth];
                        // Add all particles from this neighbor to the list of neighboring particles
                        std::vector<Particule3D> neighborParticles = neighborCell.getParticules();
                        particules_voisines.insert(particules_voisines.end(), neighborParticles.begin(), neighborParticles.end());
                    }
                }
            }

            for (auto &p1 : part) {
                p1.setForce(Vector3D(0, 0, 0)); // Reset force to zero

                const Vector3D pos_i = p1.getPos();
                const double masse_i = p1.getMasse();

                for (auto &p2 : particules_voisines) {
                    if (&p1 == &p2) continue; // Skip self-interaction

                    const Vector3D pos_j = p2.getPos();
                    Vector3D r = pos_j - pos_i;
                    double norme_r = r.norm();

                    if (norme_r != 0.0 && norme_r < rCut) { // Avoid division by zero and skip particles outside the cutoff
                        double powTo6 = std::pow(sigma / norme_r, 6);
                        Vector3D force = r * (24 * eps * std::pow(1 / norme_r, 2) * powTo6 * (1 - 2 * powTo6));
                        force += r * (masse_i * 1 / (norme_r * norme_r * norme_r)); // Gravitational force
                        // Cap the forces to avoid numerical instabilities
                        if (scaleType == 0) {
                            if (force.getX() > 1e5) {
                                force.setX(1e5);
                            }
                            if (force.getY() > 1e5) {
                                force.setY(1e5);
                            }
                            if (force.getX() < -1e5) {
                                force.setX(-1e5);
                            }
                            if (force.getY() < -1e5) {
                                force.setY(-1e5);
                            }
                        }

                        p1.setForce(p1.getForce() + force);
                    }
                }

                // Add gravitational force if G is non-zero
                if (G != 0) {
                    p1.getForce().setY(p1.getForce().getY() + p1.getMasse() * G);
                }
            }

            cellule.setParticules(part);
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Calculates the forces on each particle using the Lennard-Jones potential and gravitational forces in 3D.
 *
 */
void Univers::calculForces3D() {
    try {
        int gridWidth = L1 / rCut;
        int gridHeight = L2 / rCut;
        int gridDepth = L3 / rCut;

        for (auto &cellule : cellules) {
            int *id = cellule.getId();
            int x = id[0];
            int y = id[1];
            int z = id[2];

            // List of neighboring particles
            std::vector<Particule3D> particules_voisines;
            std::vector<Particule3D> &part = cellule.getParticules();

            // Interactions with the neighboring cells
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dz = -1; dz <= 1; dz++) {
                        int neighborX = x + dx;
                        int neighborY = y + dy;
                        int neighborZ = z + dz;

                        // Check if the neighbor coordinates are within the grid bounds
                        if (neighborX >= 0 && neighborX < gridWidth && neighborY >= 0 && neighborY < gridHeight && neighborZ >= 0 && neighborZ < gridDepth) {
                            auto &neighborCell = cellules[neighborX + neighborY * gridWidth + neighborZ * gridWidth * gridHeight];
                            // Add all particles from this neighbor to the list of neighboring particles
                            std::vector<Particule3D> neighborParticles = neighborCell.getParticules();
                            particules_voisines.insert(particules_voisines.end(), neighborParticles.begin(), neighborParticles.end());
                        }
                    }
                }
            }

            for (auto &p1 : part) {
                p1.setForce(Vector3D(0, 0, 0)); // Reset force to zero

                const Vector3D pos_i = p1.getPos();
                const double masse_i = p1.getMasse();

                for (auto &p2 : particules_voisines) {
                    if (&p1 == &p2) continue; // Skip self-interaction

                    const Vector3D pos_j = p2.getPos();
                    Vector3D r = pos_j - pos_i;
                    double norme_r = r.norm();

                    if (norme_r != 0.0 && norme_r < rCut) { // Avoid division by zero and skip particles outside the cutoff
                        double powTo6 = std::pow(sigma / norme_r, 6);
                        Vector3D force = r * (24 * eps * std::pow(1 / norme_r, 2) * powTo6 * (1 - 2 * powTo6));
                        force += r * (masse_i * 1 / (norme_r * norme_r * norme_r)); // Gravitational force
                        // Cap the forces to avoid numerical instabilities
                        if (force.getX() > 1e5) force.setX(1e5);
                        if (force.getY() > 1e5) force.setY(1e5);
                        if (force.getZ() > 1e5) force.setZ(1e5);
                        if (force.getX() < -1e5) force.setX(-1e5);
                        if (force.getY() < -1e5) force.setY(-1e5);
                        if (force.getZ() < -1e5) force.setZ(-1e5);
                        p1.setForce(p1.getForce() + force);
                    }
                }

                // Add gravitational force if G is non-zero
                if (G != 0) {
                    p1.getForce().setY(p1.getForce().getY() + p1.getMasse() * G);
                }
            }

            cellule.setParticules(part);
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Applies periodic boundary conditions to the simulation.
 *
 * This function ensures that particles that move out of the simulation domain on one side
 * re-enter on the opposite side, maintaining the continuity of the simulation space.
 * The function iterates through each cell and each particle, checks if a particle's position
 * is outside the domain boundaries in 2D or 3D, and if so, wraps the position around to the opposite side.
 */
void Univers::periodicBC() {
    try {
        std::vector<Cellule> &cells = cellules;
        for (auto &cellule : cells) {
            // Verify if the cell is in the domain boundary
            std::vector<Particule3D> &part = cellule.getParticules();
            // Check if the particle is out of the domain
            for (auto &p : part) {
                Vector3D pos = p.getPos();
                Vector3D oldPos = p.getPos();
                double posOldX = pos.getX();
                double posOldY = pos.getY();
                double posOldZ = pos.getZ();

                // Apply periodic boundary conditions for X coordinate
                if (posOldX < 0) {
                    pos.setX(L1 - std::abs(fmod(posOldX, L1)));
                }
                if (posOldX >= L1) {
                    pos.setX(fmod(posOldX, L1));
                }

                // Apply periodic boundary conditions for Y coordinate
                if (posOldY < 0) {
                    pos.setY(L2 - std::abs(fmod(posOldY, L2)));
                }
                if (posOldY >= L2) {
                    pos.setY(fmod(posOldY, L2));
                }

                // Apply periodic boundary conditions for Z coordinate
                if (posOldZ < 0) {
                    pos.setZ(L3 - std::abs(fmod(posOldZ, L3)));
                }
                if (posOldZ >= L3 && L3 != 0) {
                    pos.setZ(fmod(posOldZ, L3));
                }

                p.setPos(pos);
            }

            cellule.setParticules(part);
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Calculates the total kinetic energy of the system.
 *
 * This function computes the kinetic energy of all particles in the system.
 * The kinetic energy for each particle is calculated as 0.5 * mass * velocity^2.
 *
 * @return The total kinetic energy of the system.
 */
double Univers::energieCinetique() {
    try {
        double energieCinetique = 0;

        for (auto &cellule : cellules) {
            std::vector<Particule3D> &part = cellule.getParticules();

            // Iterate over each particle in the cell
            for (auto &p : part) {
                Vector3D vit = p.getVit();

                energieCinetique += p.getMasse() * (vit * vit);
            }
        }

        return 0.5 * energieCinetique;
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Applies absorption boundary conditions to the simulation.
 *
 * This function removes particles that move out of the simulation domain.
 * If a particle's position is outside the domain boundaries, it is removed from the simulation.
 * The function iterates through each cell and each particle, checks if a particle's position
 * is outside the domain boundaries, and if so, removes the particle from the cell.
 */
void Univers::absorptionBC() {
    try {
        // Iterate over each cell in the simulation
        for (auto &cellule : cellules) {
            auto &part = cellule.getParticules();

            // Iterate over each particle in the cell in reverse order
            // (to avoid issues with removing elements while iterating)
            for (int i = static_cast<int>(part.size()) - 1; i >= 0; --i) {
                auto &p = part[i];

                // Check if the particle is outside the domain boundaries
                if (p.getPos().getX() < 0 || p.getPos().getX() > L1 || p.getPos().getY() < 0 || p.getPos().getY() > L2 || p.getPos().getZ() < 0 || p.getPos().getZ() > L3) {
                    std::cout << "Removing particle at index: " << i << std::endl;
                    // Remove the particle from the cell
                    cellule.removeParticule(i);
                    // Decrease the total number of particles
                    nbParticules--;
                }
            }
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Reassigns particles to the correct cells based on their positions.
 *
 * This function collects all particles from all cells, clears the cells,
 * and then reassigns the particles to the correct cells based on their positions.
 * It ensures that particles are placed in the appropriate cell according to their new positions.
 */
void Univers::reassignCells() {
    try {
        // Collect all particles from all cells
        std::vector<Particule3D> allParticules;
        for (auto &cellule : cellules) {
            auto part = cellule.getParticules();
            allParticules.insert(allParticules.end(), part.begin(), part.end());
            cellule.clearParticules(); // Clear particles as we collect them
        }

        int nCellsX = L1 / rCut;
        int nCellsY = L2 / rCut;

        // Reassign particles to the correct cells based on their positions
        for (auto &p : allParticules) {
            int cellX = static_cast<int>(p.getPos().getX() / rCut);
            int cellY = static_cast<int>(p.getPos().getY() / rCut);

            // Ensure the particle is within the grid bounds
            if (cellX >= 0 && cellX <= nCellsX && cellY >= 0 && cellY <= nCellsY) {
                // Handle edge cases where the particle is on the boundary
                if (cellX == nCellsX) {
                    cellX--;
                }
                if (cellY == nCellsY) {
                    cellY--;
                }

                int newCellIndex = cellX + cellY * nCellsX;

                // Add the particle to the correct cell
                cellules[newCellIndex].addParticule(p);
            } else {
                // If a particle is out of bounds, print an error message and exit
                std::ostringstream oss;
                oss << "Particle out of bounds: ID=" << p.getId() << ", Position=(" << p.getPos().getX() << ", " << p.getPos().getY() << ")";
                throw std::out_of_range(oss.str());
            }
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Reassigns particles to the correct cells based on their positions in 3D.
 *
 * This function collects all particles from all cells, clears the cells,
 * and then reassigns the particles to the correct cells based on their positions.
 * It ensures that particles are placed in the appropriate cell according to their new positions.
 */
void Univers::reassignCells3D() {
    try {
        // Collect all particles from all cells
        std::vector<Particule3D> allParticules;
        for (auto &cellule : cellules) {
            auto part = cellule.getParticules();
            allParticules.insert(allParticules.end(), part.begin(), part.end());
            cellule.clearParticules(); // Clear particles as we collect them
        }

        int nCellsX = L1 / rCut;
        int nCellsY = L2 / rCut;
        int nCellsZ = L3 / rCut;

        // Reassign particles to the correct cells based on their positions
        for (auto &p : allParticules) {
            int cellX = static_cast<int>(p.getPos().getX() / rCut);
            int cellY = static_cast<int>(p.getPos().getY() / rCut);
            int cellZ = static_cast<int>(p.getPos().getZ() / rCut);

            // Ensure the particle is within the grid bounds
            if (cellX >= 0 && cellX <= nCellsX && cellY >= 0 && cellY <= nCellsY && cellZ >= 0 && cellZ <= nCellsZ) {
                // Handle edge cases where the particle is on the boundary
                if (cellX == nCellsX) {
                    cellX--;
                }
                if (cellY == nCellsY) {
                    cellY--;
                }
                if (cellZ == nCellsZ) {
                    cellZ--;
                }

                int newCellIndex = cellX + cellY * nCellsX + cellZ * nCellsX * nCellsY;

                // Add the particle to the correct cell
                cellules[newCellIndex].addParticule(p);
            } else {
                // If a particle is out of bounds, print an error message and exit
                std::ostringstream oss;
                oss << "Particle out of bounds: ID=" << p.getId() << ", Position=(" << p.getPos().getX() << ", " << p.getPos().getY() << ", " << p.getPos().getZ() << ")";
                throw std::out_of_range(oss.str());
            }
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Evolves the system over time using the Verlet integration algorithm.
 *
 * This function performs the time evolution of the system by iterating over time steps,
 * updating positions and velocities of particles, applying boundary conditions, and
 * recalculating forces.
 */
void Univers::evolution2D() {
    try {
        // Initial output to VTK file
        std::string filename = "data_t0.vtu";
        writeVTKFile(filename);

        // Initialize forces vector
        std::vector<Vector3D> forcesOld;
        forcesOld.resize(nbParticules);

        std::cout << "Number of particles: " << nbParticules << std::endl;

        // Calculate initial forces
        calculForces();

        // Time initialization
        int iter = 0;
        float t = 0;
        int file_index = 0;

        // Time evolution loop
        while (t < tmax) {
            // Scale speed using kinetic energy to compute coefficient Beta
            if (scaleType == 1) {
                double kinetic_energy = energieCinetique();
                std::cout << "Kinetic energy: " << kinetic_energy << std::endl;
                auto beta = static_cast<float>(std::sqrt(0.005 / kinetic_energy));

                if (iter % 1000 == 0) {
                    for (auto &cellule : cellules) {
                        std::vector<Particule3D> &part = cellule.getParticules();
                        for (auto &p : part) {
                            p.setVit(p.getVit() * beta);
                        }
                        cellule.setParticules(part);
                    }
                }
            }

            file_index++;
            t += dt;
            iter++;

            // Update positions
            for (auto &cellule : cellules) {
                std::vector<Particule3D> par = cellule.getParticules();
                for (auto &p : par) {
                    Vector3D pos = p.getPos();
                    Vector3D vit = p.getVit();
                    float masse = p.getMasse();
                    Vector3D force = p.getForce();

                    if (boundaryCond == 2) {
                        Vector3D v = pos;
                        v += (vit + force * dt * (0.5 / masse)) * dt;
                        // Reflection boundary conditions
                        if (v.getX() < 0 || v.getX() > L1) {
                            vit.setX(-vit.getX());
                        }
                        if (pos.getY() < 0 || pos.getY() > L2) {
                            vit.setY(-vit.getY());
                        }
                        p.setVit(vit);
                        pos += (p.getVit() + force * dt * (0.5 / masse)) * dt;
                        p.setPos(pos);
                        absorptionBC();
                    } else {
                        pos += (p.getVit() + force * dt * (0.5 / masse)) * dt;
                        p.setPos(pos);
                    }

                    int id = p.getId();
                    if (id >= 0 && id < (int)forcesOld.size()) {
                        forcesOld[id] = p.getForce();
                    }
                }
                cellule.setParticules(par);
            }

            // Apply boundary conditions
            if (boundaryCond == 0) {
                absorptionBC();
            } else if (boundaryCond == 1) {
                std::cout << "here" << std::endl;
                periodicBC();
            }

            // Reassign particles to their new cells
            reassignCells();

            // Calculate new forces
            calculForces();

            // Update velocities
            for (auto &cellule : cellules) {
                std::vector<Particule3D> &part = cellule.getParticules();
                for (auto &p : part) {
                    Vector3D vit = p.getVit();
                    float masse = p.getMasse();
                    Vector3D force = p.getForce();
                    Vector3D forceOld = forcesOld[p.getId()];
                    vit = vit + (force + forceOld) * dt * (0.5 / masse);
                    p.setVit(vit);
                }
                cellule.setParticules(part);
            }

            // Write to VTK file
            filename = "data_t" + std::to_string(file_index) + ".vtu";
            writeVTKFile(filename);
            std::cout << "Pourcentage de l'évolution : " << (t - dt) / tmax * 100 << "%" << std::endl;
        }

        std::cout << "Evolution completed" << std::endl;
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Evolves the system over time using the Verlet integration algorithm for 3D.
 */
void Univers::evolution3D() {
    try {
        // Initial output to VTK file
        std::string filename = "data_t0.vtu";
        writeVTKFile(filename);

        // Initialize forces vector
        std::vector<Vector3D> forcesOld;
        forcesOld.resize(nbParticules);

        std::cout << "Number of particles: " << nbParticules << std::endl;

        // Calculate initial forces
        calculForces3D();

        // Time initialization
        int iter = 0;
        float t = 0;
        int file_index = 0;

        // Time evolution loop
        while (t < tmax) {
            // Scale speed using kinetic energy to compute coefficient Beta
            if (scaleType == 1) {
                double kinetic_energy = energieCinetique();
                std::cout << "Kinetic energy: " << kinetic_energy << std::endl;
                auto beta = static_cast<float>(std::sqrt(0.005 / kinetic_energy));

                if (iter % 1000 == 0) {
                    for (auto &cellule : cellules) {
                        std::vector<Particule3D> &part = cellule.getParticules();
                        for (auto &p : part) {
                            p.setVit(p.getVit() * beta);
                        }
                        cellule.setParticules(part);
                    }
                }
            }

            file_index++;
            t += dt;
            iter++;

            // Update positions
            for (auto &cellule : cellules) {
                std::vector<Particule3D> par = cellule.getParticules();
                for (auto &p : par) {
                    Vector3D pos = p.getPos();
                    Vector3D vit = p.getVit();
                    float masse = p.getMasse();
                    Vector3D force = p.getForce();

                    if (boundaryCond == 2) {
                        Vector3D v = pos;
                        v += (vit + force * dt * (0.5 / masse)) * dt;
                        // Reflection boundary conditions
                        if (v.getX() < 0 || v.getX() > L1) {
                            vit.setX(-vit.getX());
                            pos = p.getPos() + (vit + force * dt * (0.5 / masse)) * dt;
                        }
                        if (pos.getY() < 0 || pos.getY() > L2) {
                            vit.setY(-vit.getY());
                            pos = p.getPos() + (vit + force * dt * (0.5 / masse)) * dt;
                        }
                        p.setVit(vit);
                    } else {
                        pos += (vit + force * dt * (0.5 / masse)) * dt;
                    }
                    p.setPos(pos);

                    int id = p.getId();
                    if (id >= 0 && id < (int)forcesOld.size()) {
                        forcesOld[id] = p.getForce();
                    }
                }
                cellule.setParticules(par);
            }

            // Apply boundary conditions
            if (boundaryCond == 0) {
                absorptionBC();
            } else if (boundaryCond == 1) {
                periodicBC();
            }

            // Reassign particles to their new cells
            reassignCells3D();

            // Calculate new forces
            calculForces3D();

            // Update velocities
            for (auto &cellule : cellules) {
                std::vector<Particule3D> &part = cellule.getParticules();
                for (auto &p : part) {
                    Vector3D vit = p.getVit();
                    float masse = p.getMasse();
                    Vector3D force = p.getForce();
                    Vector3D forceOld = forcesOld[p.getId()];
                    vit = vit + (force + forceOld) * dt * (0.5 / masse);
                    p.setVit(vit);
                }
                cellule.setParticules(part);
            }

            // Write to VTK file at specified intervals
            filename = "data_t" + std::to_string(file_index) + ".vtu";
            writeVTKFile(filename);
            // print le pourcentage de l'évolution
            std::cout << "Pourcentage de l'évolution : " << (t - dt) / tmax * 100 << "%" << std::endl;
        }

        std::cout << "Evolution in 3D completed" << std::endl;
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}

/**
 * @brief Evolves the system over time using the Verlet integration algorithm.
 *
 * This function selects the appropriate evolution function based on the dimensionality of the system.
 */
void Univers::evolution() {
    try {
        if (dimension == 2 || L3 == 0) {
            evolution2D();
        } else if (dimension == 3 && L3 != 0) {
            evolution3D();
        } else {
            throw std::runtime_error("Unsupported dimension: " + std::to_string(dimension));
        }
    } catch (const std::exception &e) {
        logError(e.what());
        throw;
    }
}
