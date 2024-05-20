#include <vector>
#include "Cellule.hxx"
#include "Particule3D.hxx"
#include "Vector3D.hxx"
#include <string>

#ifndef UNIVERS_HXX
#define UNIVERS_HXX

/**
 * @brief Class representing a universe containing particles and cells.
 */
class Univers {
private:
    int dimension; ///< Dimension of the universe
    int nbParticules; ///< Number of particles
    std::vector<Cellule> cellules; ///< List of cells in the universe
    std::vector<Particule3D> particules; ///< List of particles in the universe
    int L1; ///< Length of the universe in x direction
    int L2; ///< Length of the universe in y direction
    int L3; ///< Length of the universe in z direction
    int eps; ///< Epsilon
    int sigma; ///< Sigma
    float rCut; ///< Cutoff radius
    float dt; ///< Time step
    float tmax; ///< Maximum time
    int boundaryCond = 0; ///< Boundary condition: 0 = absorption, 1 = periodic, 2 = reflection
    float G = 0; ///< Gravitational constant
    int scaleType = 0; ///< Scale type: 0 = scale by max force, 1 = using kinetic energy

public:
    /**
     * @brief Constructor with basic parameters.
     *
     * @param dimension Dimension of the universe
     * @param L1 Length of the universe in x direction
     * @param L2 Length of the universe in y direction
     * @param L3 Length of the universe in z direction
     * @param rCut Cutoff radius
     * @param dt Time step
     * @param tmax Maximum time
     */
    Univers(int dimension, int L1, int L2, int L3, float rCut, float dt, float tmax);

    /**
     * @brief Constructor with extended parameters.
     *
     * @param dimension Dimension of the universe
     * @param L1 Length of the universe in x direction
     * @param L2 Length of the universe in y direction
     * @param L3 Length of the universe in z direction
     * @param eps Epsilon value
     * @param sigma Sigma value
     * @param rCut Cutoff radius
     * @param dt Time step
     * @param tmax Maximum time
     */
    Univers(int dimension, int L1, int L2, int L3, int eps, int sigma, float rCut, float dt, float tmax);

    /**
     * @brief Full constructor.
     *
     * @param dimension Dimension of the universe
     * @param L1 Length of the universe in x direction
     * @param L2 Length of the universe in y direction
     * @param L3 Length of the universe in z direction
     * @param eps Epsilon value
     * @param sigma Sigma value
     * @param rCut Cutoff radius
     * @param dt Time step
     * @param tmax Maximum time
     * @param boundaryCond Boundary condition
     * @param G Gravitational constant
     * @param scaleType Scale type
     */
    Univers(int dimension, int L1, int L2, int L3, int eps, int sigma, float rCut, float dt, float tmax, int boundaryCond, float G, int scaleType);

    /**
     * @brief Default constructor.
     */
    Univers();

    /**
     * @brief Gets the dimension of the universe.
     *
     * @return int Dimension of the universe
     */
    int getDimension() const;

    /**
     * @brief Gets the number of particles.
     *
     * @return int Number of particles
     */
    int getNbParticules() const;

    /**
     * @brief Gets the list of cells in the universe.
     *
     * @return std::vector<Cellule> List of cells
     */
    std::vector<Cellule> getCellules();

    /**
     * @brief Gets the length of the universe in the x direction.
     *
     * @return int Length in x direction
     */
    int getL1() const;

    /**
     * @brief Gets the length of the universe in the y direction.
     *
     * @return int Length in y direction
     */
    int getL2() const;

    /**
     * @brief Gets the length of the universe in the z direction.
     *
     * @return int Length in z direction
     */
    int getL3() const;

    /**
     * @brief Gets the cutoff radius.
     *
     * @return float Cutoff radius
     */
    float getRCut() const;

    /**
     * @brief Sets the list of cells in the universe.
     *
     * @param cellules New list of cells
     */
    void setCellules(std::vector<Cellule> cellules);


    /**
     * @brief Writes a VTK file.
     *
     * @param filename Name of the file
     */
    void writeVTKFile(std::string filename);

    /**
     * @brief Calculates forces in 3D.
     */
    void calculForces3D();

    /**
     * @brief Initializes a demo in a circle.
     *
     * @param dim1_bleue Dimension 1 of the blue particles
     * @param dim2_bleue Dimension 2 of the blue particles
     * @param rayon_rouge Radius of the red particles
     * @param vitesse_bleue Speed of the blue particles
     * @param vitesse_rouge Speed of the red particles
     */
    void initialiserDemoCercle(int dim1_bleue, int dim2_bleue, float rayon_rouge, const Vector3D& vitesse_bleue, const Vector3D& vitesse_rouge);

    /**
     * @brief Calculates forces.
     *
     * @param forcesOld Previous forces
     */
    void calculForces();

    /**
     * @brief Initializes particles.
     *
     * @param dim1_rouge Dimension 1 of the red particles
     * @param dim2_rouge Dimension 2 of the red particles
     * @param dim1_bleue Dimension 1 of the blue particles
     * @param dim2_bleue Dimension 2 of the blue particles
     * @param vitesseRouge Speed of the red particles
     * @param vitesseBleue Speed of the blue particles
     */
    void initialiser(int dim1_rouge, int dim2_rouge, int dim1_bleue, int dim2_bleue, const Vector3D& vitesse_rouge, const Vector3D& vitesse_bleue) ;



    /**
     * @brief Starts the evolution of the universe.
     */
    void evolution();

    /**
     * @brief Assigns a particle to a cell.
     *
     * @param particule Particle to assign
     * @param nCellX Index of the cell
     */
    void assignParticule(const Particule3D& particule, int nCellX);

    /**
     * @brief Applies absorbing boundary conditions.
     */
    void absorptionBC();

    /**
     * @brief Prints the particles.
     */
    void printParticules();

    /**
     * @brief Reassigns particles to cells.
     */
    void reassignCells();

    /**
     * @brief Process cells for computation.
     *
     * @param cellules List of cells
     * @param gridWidth Width of the grid
     * @param gridHeight Height of the grid
     */
    static void processCells(std::vector<Cellule> cellules, int gridWidth, int gridHeight);

    /**
     * @brief Applies periodic boundary conditions.
     */
    void periodicBC();

    /**
     * @brief Starts the evolution in 2D.
     */
    void evolution2D();

    /**
     * @brief Starts the evolution in 3D.
     */
    void evolution3D();

    /**
     * @brief Calculates kinetic energy.
     *
     * @return double Kinetic energy
     */
    double energieCinetique();

    /**
     * @brief Reassigns particles to cells in 3D.
     */
    void reassignCells3D();
};

#endif // UNIVERS_HXX
