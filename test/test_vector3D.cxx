

// tester toutes les méthodes de la classe Vector3D

#include <iostream>
#include <cassert>
#include "Vector3D.hxx"

int main (){

    Vector3D v1(1, 2, 3);
    Vector3D v2(4, 5, 6);
    Vector3D v3;

    // test de la méthode setX
    v3.setX(7);
    assert(v3.getX() == 7);

    // test de la méthode setY
    v3.setY(8);
    assert(v3.getY() == 8);

    // test de la méthode setZ
    v3.setZ(9);
    assert(v3.getZ() == 9);

    // test de la méthode operator-
    Vector3D v4 = v1 - v2;
    assert(v4.getX() == -3);
    assert(v4.getY() == -3);
    assert(v4.getZ() == -3);

    // test de la méthode operator+
    Vector3D v5 = v1 + v2;
    assert(v5.getX() == 5);
    assert(v5.getY() == 7);
    assert(v5.getZ() == 9);

    // test de la méthode operator*
    Vector3D v6 = v1 * 2;
    assert(v6.getX() == 2);
    assert(v6.getY() == 4);
    assert(v6.getZ() == 6);

    // test de la méthode operator*
    float v7 = v1 * v2;
    assert(v7 == 32);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
