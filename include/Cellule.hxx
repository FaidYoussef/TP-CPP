
#include <vector>
#include "Particule3D.hxx"

#ifndef CELLULE_HXX
#define CELLULE_HXX


class Cellule {
    private:
        int nbParticules;
        // une liste de particules
        std::vector<Particule3D> particules;
        Vector3D centre;

    public:
        Cellule(Vector3D centre, std::vector<Particule3D> particules);
        Cellule();
    
    int getNbParticules() const;
    std::vector<Particule3D> getParticules();
    Vector3D getCentre() const;
    void setParticules(std::vector<Particule3D> particules);
    void addParticule(Particule3D particule);
    void removeParticule(Particule3D particule);
};

#endif // CELLULE_HXX