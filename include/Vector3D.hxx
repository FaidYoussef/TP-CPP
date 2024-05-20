/**
 * @class Vector3D
 * @brief Classe représentant un vecteur dans un espace tridimensionnel.
 *
 * La classe Vector3D gère les coordonnées x, y, et z d'un vecteur et fournit diverses méthodes pour manipuler ces vecteurs.
 */

#ifndef VECTOR3D_HXX
#define VECTOR3D_HXX

class Vector3D {
private:
    double x; ///< Coordonnée x du vecteur
    double y; ///< Coordonnée y du vecteur
    double z; ///< Coordonnée z du vecteur

public:
    /**
     * @brief Constructeur avec paramètres.
     *
     * @param x Coordonnée x du vecteur.
     * @param y Coordonnée y du vecteur.
     * @param z Coordonnée z du vecteur.
     */
    Vector3D(double x, double y, double z);

    /**
     * @brief Constructeur par défaut.
     */
    Vector3D();

    /**
     * @brief Destructeur.
     */
    ~Vector3D();

    /**
     * @brief Constructeur de copie.
     *
     * @param other L'autre vecteur à copier.
     */
    Vector3D(const Vector3D &other);

    /**
     * @brief Opérateur d'affectation par copie.
     *
     * @param other L'autre vecteur à copier.
     * @return Une référence au vecteur assigné.
     */
    Vector3D& operator=(const Vector3D &other);

    /**
     * @brief Obtient la coordonnée x du vecteur.
     *
     * @return La coordonnée x actuelle.
     */
    double getX() const;

    /**
     * @brief Obtient la coordonnée y du vecteur.
     *
     * @return La coordonnée y actuelle.
     */
    double getY() const;

    /**
     * @brief Obtient la coordonnée z du vecteur.
     *
     * @return La coordonnée z actuelle.
     */
    double getZ() const;

    /**
     * @brief Définit la coordonnée x du vecteur.
     *
     * @param x La nouvelle coordonnée x.
     */
    void setX(double x);

    /**
     * @brief Définit la coordonnée y du vecteur.
     *
     * @param y La nouvelle coordonnée y.
     */
    void setY(double y);

    /**
     * @brief Définit la coordonnée z du vecteur.
     *
     * @param z La nouvelle coordonnée z.
     */
    void setZ(double z);

    /**
     * @brief Soustraction de vecteurs.
     *
     * @param other L'autre vecteur à soustraire.
     * @return Un nouveau vecteur résultant de la soustraction.
     */
    Vector3D operator-(const Vector3D& other) const;

    /**
     * @brief Addition de vecteurs.
     *
     * @param other L'autre vecteur à ajouter.
     * @return Un nouveau vecteur résultant de l'addition.
     */
    Vector3D operator+(const Vector3D& other) const;

    /**
     * @brief Addition et affectation de vecteurs.
     *
     * @param other L'autre vecteur à ajouter.
     * @return Une référence au vecteur actuel après l'addition.
     */
    Vector3D operator+=(const Vector3D& other);

    /**
     * @brief Multiplication par un scalaire.
     *
     * @param scalar Le scalaire par lequel multiplier.
     * @return Un nouveau vecteur résultant de la multiplication.
     */
    Vector3D operator*(const double& scalar) const;

    /**
     * @brief Produit scalaire de deux vecteurs.
     *
     * @param other L'autre vecteur.
     * @return Le produit scalaire.
     */
    double operator*(const Vector3D& other) const;

    /**
     * @brief Opérateur de comparaison d'égalité.
     *
     * @param other L'autre vecteur à comparer.
     * @return True si les vecteurs sont égaux.
     */
    bool operator==(const Vector3D& other) const;

    /**
     * @brief Opérateur de comparaison d'inégalité.
     *
     * @param other L'autre vecteur à comparer.
     * @return True si les vecteurs sont différents.
     */
    bool operator!=(const Vector3D& other) const;

    /**
     * @brief Calcul de la norme du vecteur.
     *
     * @return La norme du vecteur.
     */
    double norm() const;
};

#endif // VECTOR3D_HXX
