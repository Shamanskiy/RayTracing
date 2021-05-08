#include <sstream>

#include "catch_utils.hpp"
#include "Vec3.h"

using namespace std;
using namespace Catch::literals;

SCENARIO("Inspecting a vector", "[Vec3]")
{
    GIVEN("A vector ") {
        Vec3 vec(3.0, 4.0, 0.0);

        WHEN("") {
            ostringstream stream;
            stream << vec;
            THEN("we can pass it to an output stream")      
                REQUIRE(stream.str() == "3 4 0");
        }

        WHEN("")
            THEN("we can access its elements as xyz") {
                REQUIRE(vec.x() == 3.0_a);
                REQUIRE(vec.y() == 4.0_a);
                REQUIRE(vec.z() == 0.0_a);
            }

        WHEN("")
            THEN("we can access its elements as rgb") {
                REQUIRE(vec.r() == 3.0_a);
                REQUIRE(vec.g() == 4.0_a);
                REQUIRE(vec.b() == 0.0_a);
            }

        WHEN("") {
            vec[0] = 1.0;
            vec[1] = 1.0;
            vec[2] = 1.0;
            THEN("we can access and modify its elements with brackets") {
                REQUIRE(vec[0] == 1.0_a);
                REQUIRE(vec[1] == 1.0_a);
                REQUIRE(vec[2] == 1.0_a);
            }
        }
    }
}

SCENARIO("Default vector", "[Vec3]")
{
    GIVEN("A default vector ") {
        Vec3 vec;

        WHEN("")
            THEN("it has zero elements")
                REQUIRE(vec == Vec3(0.0, 0.0, 0.0));
    }
}

SCENARIO("Vector length operations", "[Vec3]")
{
    GIVEN("A vector ") {
        Vec3 vec(3.0, 4.0, 0.0);

        WHEN("") 
            THEN("we can compute its length")
                REQUIRE(vec.length() == 5.0_a);

        WHEN("") 
            THEN("we can compute its squared length")
                REQUIRE(vec.length_sq() == 25.0_a);
  
        WHEN("we normalize it")
        {
            Vec3 unit_vec = vec.normalize();
            THEN("we get a new unit vector")
            {
                REQUIRE(vec.length() == 5.0_a);
                REQUIRE(unit_vec.length() == 1.0_a);
            }
        }

        WHEN("we normalize it in place")
        {
            vec.normalize_inplace();
            THEN("its length becomes 1")
                REQUIRE(vec.length() == 1.0_a);
        }

        WHEN("we negate it") {
            Vec3 vec_neg = -vec;
            THEN("we get a vector with negated components")
                REQUIRE(vec_neg == Vec3(-3.0, -4.0, 0.0));
        }   
    }
}

SCENARIO("Vector arithmetic in-place", "[Vec3]")
{
    GIVEN("Two vectors ") {
        Vec3 vecA(1.0, 2.0, 3.0);
        Vec3 vecB(2.0, 2.0, 2.0);

        WHEN("we add one to another in place") {
            vecA += vecB;
            THEN("the first one changes")
                REQUIRE(vecA == Vec3(3.0, 4.0, 5.0));
        }

        WHEN("we subtract one from another in place") {
            vecA -= vecB;
            THEN("the first one changes")
                REQUIRE(vecA == Vec3(-1.0, 0.0, 1.0));
        }

        WHEN("we multiply one by another in place") {
            vecA *= vecB;
            THEN("the first one changes")
                REQUIRE(vecA == Vec3(2.0, 4.0, 6.0));
        }

        WHEN("we divide one by another in place") {
            vecA /= vecB;
            THEN("the first one changes")
                REQUIRE(vecA == Vec3(0.5, 1.0, 1.5));
        }
    }

    GIVEN("A vector and a scalar ") {
        Vec3 vec(1.0, 2.0, 3.0);
        float scalar = 2.0;

        WHEN("we multiply the vector by the scalar in place") {
            vec *= scalar;
            THEN("the vector changes")
                REQUIRE(vec == Vec3(2.0, 4.0, 6.0));
        }

        WHEN("we divide the vector by the scalar in place") {
            vec /= scalar;
            THEN("the vector changes")
                REQUIRE(vec == Vec3(0.5, 1.0, 1.5));
        }
    }
}

SCENARIO("Vector arithmetic", "[Vec3]")
{
    GIVEN("Two vectors ") {
        Vec3 vecA(1.0, 2.0, 3.0);
        Vec3 vecB(2.0, 2.0, 2.0);

        WHEN("we add them") {
            Vec3 sum = vecA + vecB;
            THEN("we get an element-wise sum")
                REQUIRE(sum == Vec3(3.0, 4.0, 5.0));
        }

        WHEN("we subtract them") {
            Vec3 diff = vecA - vecB;
            THEN("we get an element-wise difference")
                REQUIRE(diff == Vec3(-1.0, 0.0, 1.0));
        }

        WHEN("we multiply them") {
            Vec3 product = vecA * vecB;
            THEN("we get an element-wise product")
                REQUIRE(product == Vec3(2.0, 4.0, 6.0));
        }

        WHEN("we divide them") {
            Vec3 ratio = vecA / vecB;
            THEN("we get an element-wise ratio")
                REQUIRE(ratio == Vec3(0.5, 1.0, 1.5));
        }

        WHEN("we apply dot") {
            float dot_product = dot(vecA,vecB);
            THEN("we get their dot product")
                REQUIRE(dot_product == 12.0_a);
        }
    }

    GIVEN("Two vectors ") {
        Vec3 vecA(1.0, 0.0, 0.0);
        Vec3 vecB(0.0, 1.0, 0.0);

        WHEN("we apply cross") {
            Vec3 cross_product = cross(vecA, vecB);
            THEN("we get their cross product")
                REQUIRE(cross_product == Vec3(0.0, 0.0, 1.0));
        }
    }

    GIVEN("A vector and a scalar ") {
        Vec3 vec(1.0, 2.0, 3.0);
        float scalar = 2.0;

        WHEN("") {
            Vec3 result = vec * scalar;
            THEN("we can multiply the vector by the scalar from the right")
                REQUIRE(result == Vec3(2.0, 4.0, 6.0));
        }

        WHEN("") {
            Vec3 result = scalar * vec;
            THEN("we can multiply the vector by the scalar from the left")
                REQUIRE(result == Vec3(2.0, 4.0, 6.0));
        }

        WHEN("") {
            Vec3 result = vec / scalar;
            THEN("we can divide the vector by the scalar")
                REQUIRE(result == Vec3(0.5, 1.0, 1.5));
        }
    }
}

SCENARIO("Vector linear interpolation", "[Vec3]")
{
    GIVEN("Two vectors ") {
        Vec3 vecA(1.0, 0.0, 0.0);
        Vec3 vecB(0.0, 1.0, 0.0);

        WHEN("")
            THEN("we can blend them equally")
            REQUIRE(lerp(vecA,vecB, 0.5) == Vec3(0.5, 0.5, 0.0));

        WHEN("")
            THEN("we can lerp to the first one")
            REQUIRE(lerp(vecA, vecB, 0.0) == Vec3(1.0, 0.0, 0.0));

        WHEN("")
            THEN("we can lerp to the last one")
            REQUIRE(lerp(vecA, vecB, 1.0) == Vec3(0.0, 1.0, 0.0));
    }
}
