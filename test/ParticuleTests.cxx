#include <gtest/gtest.h>
#include "Particule3D.hxx"
#include "Vector3D.hxx"

// Test the default constructor
TEST(Particule3D, DefaultConstructor) {
    Particule3D p;
    EXPECT_EQ(p.getId(), 0);
    EXPECT_EQ(p.getMasse(), 0.0f);
    EXPECT_EQ(p.getCategorie(), 0);
    EXPECT_EQ(p.getForce(), Vector3D());
    EXPECT_EQ(p.getPos(), Vector3D());
    EXPECT_EQ(p.getVit(), Vector3D());
}

// Test the parameterized constructor
TEST(Particule3D, ParameterizedConstructor) {
    Vector3D force(1.0, 2.0, 3.0);
    Vector3D position(4.0, 5.0, 6.0);
    Vector3D velocity(7.0, 8.0, 9.0);
    Particule3D p(1, 10.0f, 2, force, position, velocity);
    EXPECT_EQ(p.getId(), 1);
    EXPECT_EQ(p.getMasse(), 10.0f);
    EXPECT_EQ(p.getCategorie(), 2);
    EXPECT_EQ(p.getForce(), force);
    EXPECT_EQ(p.getPos(), position);
    EXPECT_EQ(p.getVit(), velocity);
}

// Test the setters and getters
TEST(Particule3D, SettersAndGetters) {
    Particule3D p;
    p.setId(2);
    p.setMasse(5.0f);
    p.setCategorie(3);
    Vector3D force(1.0, 2.0, 3.0);
    Vector3D position(4.0, 5.0, 6.0);
    Vector3D velocity(7.0, 8.0, 9.0);
    p.setForce(force);
    p.setPos(position);
    p.setVit(velocity);

    EXPECT_EQ(p.getId(), 2);
    EXPECT_EQ(p.getMasse(), 5.0f);
    EXPECT_EQ(p.getCategorie(), 3);
    EXPECT_EQ(p.getForce(), force);
    EXPECT_EQ(p.getPos(), position);
    EXPECT_EQ(p.getVit(), velocity);
}

// Test the copy constructor
TEST(Particule3D, CopyConstructor) {
    Particule3D original(1, 10.0f, 2, Vector3D(1.0, 2.0, 3.0), Vector3D(4.0, 5.0, 6.0), Vector3D(7.0, 8.0, 9.0));
    Particule3D copy(original);
    EXPECT_EQ(copy.getId(), 1);
    EXPECT_EQ(copy.getMasse(), 10.0f);
    EXPECT_EQ(copy.getCategorie(), 2);
    EXPECT_EQ(copy.getForce(), original.getForce());
    EXPECT_EQ(copy.getPos(), original.getPos());
    EXPECT_EQ(copy.getVit(), original.getVit());
}

// Test the copy assignment operator
TEST(Particule3D, CopyAssignmentOperator) {
    Particule3D original(1, 10.0f, 2, Vector3D(1.0, 2.0, 3.0), Vector3D(4.0, 5.0, 6.0), Vector3D(7.0, 8.0, 9.0));
    Particule3D copy;
    copy = original;
    EXPECT_EQ(copy.getId(), 1);
    EXPECT_EQ(copy.getMasse(), 10.0f);
    EXPECT_EQ(copy.getCategorie(), 2);
    EXPECT_EQ(copy.getForce(), original.getForce());
    EXPECT_EQ(copy.getPos(), original.getPos());
    EXPECT_EQ(copy.getVit(), original.getVit());
}

#include <gtest/gtest.h>
#include "Particule3D.hxx"
#include "Vector3D.hxx"

// Test the operator== method
TEST(Particule3D, EqualityOperator) {
    Particule3D p1(1, 10.0f, 2, Vector3D(1.0, 2.0, 3.0), Vector3D(4.0, 5.0, 6.0), Vector3D(7.0, 8.0, 9.0));
    Particule3D p2(1, 10.0f, 2, Vector3D(1.0, 2.0, 3.0), Vector3D(4.0, 5.0, 6.0), Vector3D(7.0, 8.0, 9.0));
    Particule3D p3(2, 20.0f, 3, Vector3D(2.0, 3.0, 4.0), Vector3D(5.0, 6.0, 7.0), Vector3D(8.0, 9.0, 10.0));
    
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

// Test the operator< method
TEST(Particule3D, LessThanOperator) {
    Particule3D p1(1, 10.0f, 2, Vector3D(1.0, 2.0, 3.0), Vector3D(4.0, 5.0, 6.0), Vector3D(7.0, 8.0, 9.0));
    Particule3D p2(2, 20.0f, 3, Vector3D(2.0, 3.0, 4.0), Vector3D(5.0, 6.0, 7.0), Vector3D(8.0, 9.0, 10.0));
    
    EXPECT_TRUE(p1 < p2);
    EXPECT_FALSE(p2 < p1);
}
