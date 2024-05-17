
#include <list>
#include "Cellule.hxx"

#ifndef UNIVERS_HXX
#define UNIVERS_HXX


class Univers {
    private:
        int dimension;
        int nbParticules;
        // une liste de particules
        //Tableau à une dimension , performante en termes de mémoire et d'accès
        std::vector<Cellule> cellules;
        std::vector<Particule3D> particules;
        int L1;
        int L2;
        int L3;
        float rCut;
        float t0;
        float dt;
        float tmax;

    public:
        Univers(int dimension, int L1, int L2, int L3, float rCut, float dt, float tmax);
        Univers();
    
    int getDimension() const;
    int getNbParticules() const;
    std::vector<Cellule> getCellules();
    int getL1() const;
    int getL2() const;
    int getL3() const;
    float getRCut() const;

    void setCellules(std::vector<Cellule> cellules);

    void initialiser();

    void initialiser2();

    void evolution();

    void assignParticule(const Particule3D&,int nCellX);
};

#endif // UNIVERS_HXX