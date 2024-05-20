/**
 * @class Cellule
 * @brief Class representing a cell containing particles in a 3D space.
 *
 * This class manages the particles contained in a cell identified by coordinates and a center.
 */

#ifndef CELLULE_HXX
#define CELLULE_HXX

#include <vector>
#include "Particule3D.hxx"

class Cellule {
private:
    int id[2];  ///< Identifier of the cell (coordinates)
    int nbParticules;  ///< Number of particles in the cell
    std::vector<Particule3D> particules;  ///< List of particles in the cell
    Vector3D centre;  ///< Center of the cell

public:
    /**
     * @brief Constructor of the Cellule class.
     *
     * @param id_1 The first coordinate of the cell identifier.
     * @param id_2 The second coordinate of the cell identifier.
     * @param centre The center of the cell.
     * @param particules The initial particles of the cell.
     */
    Cellule(int id_1, int id_2, Vector3D centre, std::vector<Particule3D> particules);

    /**
     * @brief Constructor of the Cellule class.
     *
     * @param id_1 The first coordinate of the cell identifier.
     * @param id_2 The second coordinate of the cell identifier.
     * @param centre The center of the cell.
     */
    Cellule(int id_1, int id_2, Vector3D centre);

    /**
     * @brief Default constructor of the Cellule class.
     */
    Cellule();

    /**
     * @brief Destructor of the Cellule class.
     */
    ~Cellule();

    /**
     * @brief Copy constructor of the Cellule class.
     *
     * @param other The other cell to copy.
     */
    Cellule(const Cellule &other);

    /**
     * @brief Copy assignment operator.
     *
     * @param other The other cell to copy.
     * @return A reference to the assigned cell.
     */
    Cellule& operator=(const Cellule &other);

    /**
     * @brief Gets the number of particles in the cell.
     *
     * @return The number of particles.
     */
    int getNbParticules() const;

    /**
     * @brief Gets the identifier of the cell.
     *
     * @return A pointer to the identifier (array of two integers).
     */
    int* getId();

    /**
     * @brief Gets the list of particles in the cell.
     *
     * @return A reference to the vector of particles.
     */
    std::vector<Particule3D>& getParticules();

    /**
     * @brief Gets the center of the cell.
     *
     * @return The center of the cell.
     */
    Vector3D getCentre() const;

    /**
     * @brief Sets the list of particles in the cell.
     *
     * @param particules The new vector of particles.
     */
    void setParticules(std::vector<Particule3D> particules);

    /**
     * @brief Adds a particle to the cell.
     *
     * @param particule The particle to add.
     */
    void addParticule(Particule3D particule);

    /**
     * @brief Removes a particle from the cell by its index.
     *
     * @param index The index of the particle to remove.
     */
    void removeParticule(int index);

    /**
     * @brief Removes a specific particle from the cell.
     *
     * @param p The particle to remove.
     */
    void removeParticule(const Particule3D& p);

    /**
     * @brief Removes all particles from the cell.
     */
    void clearParticules();
};

#endif // CELLULE_HXX
