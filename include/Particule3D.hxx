/**
 * @class Particule3D
 * @brief Classe représentant une particule en 3D avec des propriétés physiques.
 *
 * La classe Particule3D gère les attributs et les méthodes associés à une particule dans un espace tridimensionnel,
 * incluant la position, la vitesse, la force, la masse, l'identifiant et la catégorie.
 */

#ifndef Particule3D_HXX
#define Particule3D_HXX

#include "Vector3D.hxx" // Inclure le fichier d'en-tête Vector3D

class Particule3D {
    private:
        int id; ///< Identifiant unique de la particule
        float masse; ///< Masse de la particule
        int catégorie; ///< Catégorie de la particule
        Vector3D force; ///< Force agissant sur la particule
        Vector3D position; ///< Position de la particule
        Vector3D vitesse; ///< Vitesse de la particule

    public:
        /**
         * @brief Constructeur avec paramètres.
         *
         * @param id Identifiant unique de la particule.
         * @param masse Masse de la particule.
         * @param catégorie Catégorie de la particule.
         * @param force Force agissant sur la particule.
         * @param position Position de la particule.
         * @param vitesse Vitesse de la particule.
         */
        Particule3D(int id, float masse, int catégorie, Vector3D force, Vector3D position, Vector3D vitesse);

        /**
         * @brief Constructeur par défaut.
         */
        Particule3D();

        /**
         * @brief Destructeur.
         */
        ~Particule3D();

        /**
         * @brief Constructeur de copie.
         *
         * @param other L'autre particule à copier.
         */
        Particule3D(const Particule3D &other);

        /**
         * @brief Opérateur d'affectation par copie.
         *
         * @param other L'autre particule à copier.
         * @return Une référence à la particule assignée.
         */
        Particule3D& operator=(const Particule3D &other);

        /**
         * @brief Définit la position de la particule.
         *
         * @param pos La nouvelle position.
         */
        void setPos(Vector3D pos);

        /**
         * @brief Définit la vitesse de la particule.
         *
         * @param v La nouvelle vitesse.
         */
        void setVit(Vector3D v);

        /**
         * @brief Définit la force agissant sur la particule.
         *
         * @param f La nouvelle force.
         */
        void setForce(Vector3D f);

        /**
         * @brief Définit la masse de la particule.
         *
         * @param m La nouvelle masse.
         */
        void setMasse(float m);

        /**
         * @brief Définit la catégorie de la particule.
         *
         * @param c La nouvelle catégorie.
         */
        void setCategorie(int c);

        /**
         * @brief Définit l'identifiant de la particule.
         *
         * @param i Le nouvel identifiant.
         */
        void setId(int i);

        /**
         * @brief Obtient la position de la particule.
         *
         * @return La position actuelle.
         */
        Vector3D getPos() const;

        /**
         * @brief Obtient la vitesse de la particule.
         *
         * @return La vitesse actuelle.
         */
        Vector3D getVit();

        /**
         * @brief Obtient la force agissant sur la particule.
         *
         * @return La force actuelle.
         */
        Vector3D getForce();

        /**
         * @brief Obtient la masse de la particule.
         *
         * @return La masse actuelle.
         */
        float getMasse() const;

        /**
         * @brief Obtient la catégorie de la particule.
         *
         * @return La catégorie actuelle.
         */
        int getCategorie() const;

        /**
         * @brief Obtient l'identifiant de la particule.
         *
         * @return L'identifiant actuel.
         */
        int getId() const;

        /**
         * @brief Opérateur de comparaison pour trier les particules.
         *
         * @param other L'autre particule à comparer.
         * @return True si cette particule est considérée comme étant "moins" que l'autre.
         */
        bool operator<(const Particule3D& other) const;

        /**
         * @brief Opérateur d'égalité pour comparer les particules.
         *
         * @param other L'autre particule à comparer.
         * @return True si les deux particules sont égales.
         */
        bool operator==(const Particule3D &other) const;
};

#endif // Particule3D_HXX
