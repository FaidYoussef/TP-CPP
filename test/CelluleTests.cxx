#include <gtest/gtest.h>
#include "Cellule.hxx"
#include "Vector3D.hxx"
#include "Particule3D.hxx"

// Test the default constructor
TEST(Cellule, DefaultConstructor) {
    Cellule c;
    EXPECT_EQ(c.getNbParticules(), 0);
    EXPECT_EQ((int)c.getParticules().size(), 0);
}

// Test the parameterized constructor
TEST(Cellule, ParameterizedConstructor) {
    Vector3D centre(1.0, 2.0, 3.0);
    Cellule c(1, 2, centre);
    int* id = c.getId();
    EXPECT_EQ(id[0], 1); // Corrected from long unsigned int to int
    EXPECT_EQ(id[1], 2); // Corrected from long unsigned int to int
    EXPECT_EQ(c.getCentre(), centre);
    EXPECT_EQ(c.getNbParticules(), 0);
}


// Test the parameterized constructor with particules
// Test the parameterized constructor with particules
TEST(Cellule, ConstructorWithParticules) {
    Vector3D centre(1.0, 2.0, 3.0);
    std::vector<Particule3D> expected_particules = {Particule3D(1, 1.0, 1, Vector3D(), Vector3D(), Vector3D()),
                                                    Particule3D(2, 1.0, 1, Vector3D(), Vector3D(), Vector3D())};
    Cellule c(1, 2, centre, expected_particules);
    int* id = c.getId();
    EXPECT_EQ(id[0], 1);
    EXPECT_EQ(id[1], 2);
    EXPECT_EQ(c.getCentre(), centre);
    EXPECT_EQ(c.getNbParticules(), (int)expected_particules.size());
    EXPECT_EQ(c.getParticules(), expected_particules);
}


// Test the copy constructor
// Test the copy constructor
TEST(Cellule, CopyConstructor) {
    Vector3D centre(1.0, 2.0, 3.0);
    std::vector<Particule3D> particules = {Particule3D(1, 1.0, 1, Vector3D(), Vector3D(), Vector3D()),
                                           Particule3D(2, 1.0, 1, Vector3D(), Vector3D(), Vector3D())};
    Cellule c1(1, 2, centre, particules);
    Cellule c2(c1);
    EXPECT_EQ(c2.getId()[0], 1);
    EXPECT_EQ(c2.getId()[1], 2);
    EXPECT_EQ(c2.getCentre(), centre);
    EXPECT_EQ(c2.getNbParticules(),(int) particules.size());
    EXPECT_EQ(c2.getParticules(), particules);
}


// Test the copy assignment operator
// Test the copy assignment operator
TEST(Cellule, CopyAssignmentOperator) {
    Vector3D centre(1.0, 2.0, 3.0);
    std::vector<Particule3D> particules = {Particule3D(1, 1.0, 1, Vector3D(), Vector3D(), Vector3D()),
                                           Particule3D(2, 1.0, 1, Vector3D(), Vector3D(), Vector3D())};
    Cellule c1(1, 2, centre, particules);
    Cellule c2;
    c2 = c1;
    EXPECT_EQ(c2.getId()[0], 1);
    EXPECT_EQ(c2.getId()[1], 2);
    EXPECT_EQ(c2.getCentre(), centre);
    EXPECT_EQ(c2.getNbParticules(),(int) particules.size());
    EXPECT_EQ(c2.getParticules(), particules);
}

// Test the addParticule method
TEST(Cellule, AddParticule) {
    Cellule c;
    Particule3D p(1, 1.0, 1, Vector3D(), Vector3D(), Vector3D());
    c.addParticule(p);
    EXPECT_EQ(c.getNbParticules(), 1);
    EXPECT_EQ((int)c.getParticules().size(), 1);
    EXPECT_EQ(c.getParticules()[0], p);
}

// Test the removeParticule method
// TEST(Cellule, RemoveParticule) {
//     Cellule c;
//     Particule3D p1(1, 1.0, 1, Vector3D(), Vector3D(), Vector3D());
//     Particule3D p2(2, 1.0, 1, Vector3D(), Vector3D(), Vector3D());
//     c.addParticule(p1);
//     c.addParticule(p2);
//     c.removeParticule(p1);
//     EXPECT_EQ(c.getNbParticules(), 1);
//     EXPECT_EQ(c.getParticules().size(), 1);
//     EXPECT_EQ(c.getParticules()[0].getId(), p2.getId());
// }

// Test the setParticules method
TEST(Cellule, SetParticules) {
    Cellule c;
    std::vector<Particule3D> particules = {Particule3D(1, 1.0, 1, Vector3D(), Vector3D(), Vector3D()),
                                           Particule3D(2, 1.0, 1, Vector3D(), Vector3D(), Vector3D())};
    c.setParticules(particules);
    // print Cellule c to see if the particules are set
    std::cout << c.getParticules().size() << std::endl;
    std::vector<Particule3D> particules2 = c.getParticules();
    for (int i = 0; i <(int) particules.size(); i++) {
        std::cout << particules2[i].getId() << std::endl;
    }
    EXPECT_EQ(c.getParticules(), particules);
}
