#include <gtest/gtest.h>
#include "Univers.hxx"
#include "Cellule.hxx"
#include "Particule3D.hxx"
#include "Vector3D.hxx"

// Test the constructor
TEST(Univers, Constructor) {
    Univers u(2, 10, 10, 0, 2.5, 0.01, 1.0);
    EXPECT_EQ(u.getDimension(), 2);
    EXPECT_EQ(u.getL1(), 10);
    EXPECT_EQ(u.getL2(), 10);
    EXPECT_EQ(u.getL3(), 0);
    EXPECT_EQ(u.getRCut(), 2.5f);
    EXPECT_EQ(u.getNbParticules(), 0);
}

// Test the default constructor
TEST(Univers, DefaultConstructor) {
    Univers u;
    EXPECT_EQ(u.getDimension(), 0);
    EXPECT_EQ(u.getNbParticules(), 0);
}

// Test initialiser2 method
// TEST(Univers, Initialiser2) {
//     Univers u(2, 20, 20, 0, 2.5, 0.01, 1.0);
//     u.initialiser2();
//     EXPECT_EQ(u.getCellules().size(), 64); // 8x8 grid
//     EXPECT_EQ(u.getNbParticules(), 4*4 + 4*16); // Number of red and blue particles
// }

// Test assignParticule method
TEST(Univers, AssignParticule) {
    Univers u(2, 10, 10, 0, 2.5, 0.01, 1.0);
    Particule3D p(1, 1.0f, 1, Vector3D(), Vector3D(1.0, 1.0, 0.0), Vector3D());
    // u.initialiser2();
    std::vector<Cellule> Cells = {Cellule(1, 1, Vector3D(1.0, 1.0, 0.0))};
    u.setCellules(Cells);
    u.assignParticule(p, 4);
    EXPECT_EQ(u.getNbParticules(), 1);
    auto cells = u.getCellules();
    bool found = false;
    for (auto& cell : cells) {
        if (!cell.getParticules().empty()) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

// Test get and set methods
TEST(Univers, GetSetMethods) {
    Univers u(2, 10, 10, 0, 2.5, 0.01, 1.0);
    std::vector<Cellule> cells = {Cellule(1, 1, Vector3D(1.0, 1.0, 0.0))};
    u.setCellules(cells);
    EXPECT_EQ(u.getCellules().size(), 1);
    EXPECT_EQ(u.getNbParticules(), 0); // No particles in the cell
}

// // Test calculForces method
// TEST(Univers, CalculForces) {
//     Univers u(2, 10, 10, 0, 2.5, 0.01, 1.0);
//     u.initialiser2();
//     std::vector<Vector3D> forcesOld(20, Vector3D());
//     u.calculForces(forcesOld);
//     for (const auto& force : forcesOld) {
//         EXPECT_NE(force.getX(), 0);
//         EXPECT_NE(force.getY(), 0);
//         EXPECT_NE(force.getZ(), 0);
//     }
// }

