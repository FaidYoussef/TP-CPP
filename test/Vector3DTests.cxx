#include <gtest/gtest.h>
#include "Vector3D.hxx"

// Test the default constructor
TEST(Vector3D, DefaultConstructor) {
    Vector3D v;
    EXPECT_EQ(v.getX(), 0.0);
    EXPECT_EQ(v.getY(), 0.0);
    EXPECT_EQ(v.getZ(), 0.0);
}

// Test the parameterized constructor
TEST(Vector3D, ParameterizedConstructor) {
    Vector3D v(1.0, 2.0, 3.0);
    EXPECT_EQ(v.getX(), 1.0);
    EXPECT_EQ(v.getY(), 2.0);
    EXPECT_EQ(v.getZ(), 3.0);
}

// Test the copy constructor
TEST(Vector3D, CopyConstructor) {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(v1);
    EXPECT_EQ(v2.getX(), 1.0);
    EXPECT_EQ(v2.getY(), 2.0);
    EXPECT_EQ(v2.getZ(), 3.0);
}

// Test the copy assignment operator
TEST(Vector3D, CopyAssignmentOperator) {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2;
    v2 = v1;
    EXPECT_EQ(v2.getX(), 1.0);
    EXPECT_EQ(v2.getY(), 2.0);
    EXPECT_EQ(v2.getZ(), 3.0);
}

// Test the setX method
TEST(Vector3D, SetX) {
    Vector3D v;
    v.setX(5.0);
    EXPECT_EQ(v.getX(), 5.0);
}

// Test the setY method
TEST(Vector3D, SetY) {
    Vector3D v;
    v.setY(5.0);
    EXPECT_EQ(v.getY(), 5.0);
}

// Test the setZ method
TEST(Vector3D, SetZ) {
    Vector3D v;
    v.setZ(5.0);
    EXPECT_EQ(v.getZ(), 5.0);
}

// Test the + operator
TEST(Vector3D, AdditionOperator) {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    Vector3D result = v1 + v2;
    EXPECT_EQ(result.getX(), 5.0);
    EXPECT_EQ(result.getY(), 7.0);
    EXPECT_EQ(result.getZ(), 9.0);
}

// Test the - operator
TEST(Vector3D, SubtractionOperator) {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    Vector3D result = v2 - v1;
    EXPECT_EQ(result.getX(), 3.0);
    EXPECT_EQ(result.getY(), 3.0);
    EXPECT_EQ(result.getZ(), 3.0);
}

// Test the * operator for scalar multiplication
TEST(Vector3D, ScalarMultiplicationOperator) {
    Vector3D v(1.0, 2.0, 3.0);
    double scalar = 2.0;
    Vector3D result = v * scalar;
    EXPECT_EQ(result.getX(), 2.0);
    EXPECT_EQ(result.getY(), 4.0);
    EXPECT_EQ(result.getZ(), 6.0);
}

// Test the * operator for dot product
TEST(Vector3D, DotProductOperator) {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    double result = v1 * v2;
    EXPECT_EQ(result, 32.0); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
}

// Test the norm method
TEST(Vector3D, Norm) {
    Vector3D v(3.0, 4.0, 0.0); // This is a 3-4-5 right triangle
    double norm = v.norm();
    EXPECT_EQ(norm, 5.0); // Expected norm of a 3-4-5 triangle is 5
}

// Test the equality operator
TEST(Vector3D, EqualityOperator) {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(1.0, 2.0, 3.0);
    Vector3D v3(1.0, 2.0, 4.0);
    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
}

