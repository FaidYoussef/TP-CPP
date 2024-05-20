
#include <list>
#include "Cellule.hxx"
#include <string>

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
        int eps;
        int sigma;
        float rCut;
        float t0;
        float dt;
        float tmax;
        int boundaryCond = 0; // 0 :absorption , 1 : periodic , 2 :reflexion; absorption par défaut
        float G =0 ;
        int scaleType = 0;// 0 to scale by Max force , 1 using kinetic energy

    public:
        Univers(int dimension, int L1, int L2, int L3, float rCut, float dt, float tmax);

        Univers(int dimension, int L1, int L2, int L3,int eps,int sigma, float rCut, float dt, float tmax);

        Univers(int dimension, int L1, int L2, int L3,int eps,int sigma, float rCut, float dt, float tmax,int boundaryCond,float G, int scaleType);

        Univers();
    
    int getDimension() const;
    int getNbParticules() const;
    std::vector<Cellule> getCellules();
    int getL1() const;
    int getL2() const;
    int getL3() const;
    float getRCut() const;


    void setCellules(std::vector<Cellule> cellules);
    void addCellule(const Cellule& cell) { cellules.push_back(cell); }

    void initialiser();

    void writeVTKFile(std::string filename);

    void calculForces();
    void calculForces3D();

    void initialiser2(int dim1_rouge,int dim2_rouge, int dim1_bleue, int dim2_bleue, Vector3D vitesseRouge, Vector3D vitesseBleue);
    void initialiser3(int dim1_bleue, int dim2_bleue, float rayon_rouge, Vector3D vitesse_bleue, Vector3D vitesse_rouge) ;

    void assignParticule(const Particule3D&,int nCellsX);

    void absorptionBC();

    void reassignCells();
    void printParticules();

    void periodicBC();
    void evolution();

    void evolution2D();

    void evolution3D();



    double energieCinetique();

    void reassignCells3D();
};

#endif // UNIVERS_HXX