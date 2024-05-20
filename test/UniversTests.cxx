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
TEST(Univers, Initialize2) {
    // Create the universe with 2 dimensions and specified parameters
    Univers u(2, 20, 20, 0, 2.5, 0.01, 1.0);

    // Initialize the universe with given particle parameters
    u.initialiser(4, 4, 4, 16, Vector3D(0, 0, 0), Vector3D(0, 0, 0));

    // Check if the number of cells is as expected
    EXPECT_EQ((int)u.getCellules().size(), 64); // 8x8 grid

    // Check if the number of particles is as expected
    EXPECT_EQ(u.getNbParticules(), 4 * 4 + 4 * 16); // Number of red and blue particles
}

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
    EXPECT_EQ((int)u.getCellules().size(), 1);
    EXPECT_EQ((int)u.getNbParticules(), 0); // No particles in the cell
}

// Test periodic boundary conditions
TEST(Univers, PeriodicBoundaryConditions) {
    // Create a universe with specific parameters
    Univers u(2, 3, 3, 0, 1.0, 0.01, 1.0);

    // Set up particles near the boundaries
    std::vector<Cellule> cellules = u.getCellules();
    cellules.resize(9);

    Cellule c1(0, 0, Vector3D(0, 0, 0));
    Cellule c2(0, 2, Vector3D(10, 0, 0));
    Cellule c3(2, 0, Vector3D(0, 10, 0));
    Cellule c4(2, 2, Vector3D(10, 10, 0));

    // Manually placing particles near the boundary
    Particule3D p1(1, 1.0, 1, Vector3D(0, 0, 0), Vector3D(-0.1, -0.1, 0.0), Vector3D(0, 0, 0));
    Particule3D p2(2, 1.0, 1, Vector3D(0, 0, 0), Vector3D(0.1, 3.1, 0.0), Vector3D(0, 0, 0));
    Particule3D p3(3, 1.0, 1, Vector3D(0, 0, 0), Vector3D(3.1, 0.1, 0.0), Vector3D(0, 0, 0));
    Particule3D p4(4, 1.0, 1, Vector3D(0, 0, 0), Vector3D(3.1, 3.1, 0.0), Vector3D(0, 0, 0));

    c1.addParticule(p1); // Top-left corner
    c2.addParticule(p2); // Top-right corner
    c3.addParticule(p3); // Bottom-left corner
    c4.addParticule(p4); // Bottom-right corner

    cellules[0] = c1;
    cellules[2] = c2;
    cellules[6] = c3;
    cellules[8] = c4;

    u.setCellules(cellules);

    // Apply periodic boundary conditions
    u.periodicBC();

    // Fetch the updated cellules and check positions of the particles
    cellules = u.getCellules();

    const double tolerance = 1e-6;

    // Check positions after applying periodic boundary conditions
    EXPECT_NEAR(cellules[0].getParticules()[0].getPos().getX(), 2.9, tolerance); // Wrapped from left to right
    EXPECT_NEAR(cellules[0].getParticules()[0].getPos().getY(), 2.9, tolerance);

    EXPECT_NEAR(cellules[2].getParticules()[0].getPos().getX(), 0.1, tolerance); // Wrapped from right to left
    EXPECT_NEAR(cellules[2].getParticules()[0].getPos().getY(), 0.1, tolerance);

    EXPECT_NEAR(cellules[6].getParticules()[0].getPos().getX(), 0.1, tolerance); // Wrapped from left to right
    EXPECT_NEAR(cellules[6].getParticules()[0].getPos().getY(), 0.1, tolerance); // Wrapped from bottom to top

    EXPECT_NEAR(cellules[8].getParticules()[0].getPos().getX(), 0.1, tolerance); // Wrapped from right to left
    EXPECT_NEAR(cellules[8].getParticules()[0].getPos().getY(), 0.1, tolerance); // Wrapped from bottom to top
}
