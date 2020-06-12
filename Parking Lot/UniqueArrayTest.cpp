#include "catch.hpp"
#include "UniqueArray.h"

typedef UniqueArray<int, std::equal_to<int>> intUA;

SCENARIO("UniqueArray insert", "[UniqueArray0]") {

    unsigned int size = 4;
    intUA ua(size);

    GIVEN("An empty UniqueArray of size " + std::to_string(size)) {
        REQUIRE(ua.getSize() == size);
        REQUIRE(ua.getCount() == 0);

        WHEN("inserting elements") {
            ua.insert(4);
            ua.insert(1);
            ua.insert(1);

            THEN("getCount changes") {
                REQUIRE(ua.getCount() == 2);
            }
            THEN("getSize does not change") {
                REQUIRE(ua.getSize() == size);
            }
        }
        WHEN("inserting same element twice") {
            unsigned int index = ua.insert(1);
            ua.insert(5);
            
            THEN("same index is returned") {
                REQUIRE(ua.insert(1) == index);
            }
        }
        WHEN("inserting too many elements") {
            ua.insert(4);
            ua.insert(1);
            ua.insert(5);
            ua.insert(2);

            THEN("exception is thrown") {
                REQUIRE_THROWS_AS(ua.insert(3), intUA::UniqueArrayIsFullException);
            }
        }
    }
}

SCENARIO("UniqueArray getIndex, operator[], Copy ctor, remove", "[UniqueArray1]") {
    unsigned int size = 3;
    intUA ua(size);
    ua.insert(4);
    ua.insert(1);
    unsigned int index = ua.insert(5);

    GIVEN("A UniqueArray with some elements of size " + std::to_string(size)) {
        REQUIRE(ua.getSize() == size);
        REQUIRE(ua.getCount() > 0);

        WHEN("Accessing an existing element with getIndex") {
            unsigned int retIndex;
            bool res = ua.getIndex(5, retIndex);

            THEN("Access is successful and returns correct index") {
                REQUIRE(res == true);
                REQUIRE(retIndex == index);
            }
        }

        WHEN("Accessing an existing element") {
            const int* elem_ptr = ua[4];

            THEN("Element pointer is not null") {
                REQUIRE(elem_ptr != nullptr);
            }
        }

        WHEN("Accessing a non-existing element") {
            const int* elem_ptr = ua[2];

            THEN("Element pointer is null") {
                REQUIRE(elem_ptr == nullptr);
            }
        }

        WHEN("Copying the UniqueArray") {
            intUA copyUa(ua);

            THEN("getCounts are equal") {
                REQUIRE(ua.getCount() == copyUa.getCount());
            }
            THEN("Elements reside in different memory") {
                REQUIRE(ua[1] != copyUa[1]);
            }
        }

        WHEN("Removing an existing element") {
            bool res = ua.remove(1);

            THEN("remove is successful") {
                REQUIRE(res == true);
            }
        }

        WHEN("Removing a non-existing element") {
            bool res = ua.remove(13);

            THEN("remove is unsuccessful") {
                REQUIRE(res == false);
            }
        }
    }
}

SCENARIO("UniqueArray filter", "[UniqueArray2]") {
    unsigned int size = 5;
    intUA ua(size);
    ua.insert(3);
    ua.insert(27);
    unsigned int index = ua.insert(17);
    ua.insert(9);
    ua.insert(35);
    class LessThan20 : public intUA::Filter {
    public:
        virtual bool operator() (const int& element) const override {
            return element < 20;
        }
    };

    GIVEN("A UniqueArray with some elements of size " + std::to_string(size)) {
        REQUIRE(ua.getSize() == size);
        REQUIRE(ua.getCount() > 0);

        WHEN("Filtering the UniqueArray") {
            intUA filtered = ua.filter(LessThan20());

            THEN("getCount is correct") {
                REQUIRE(filtered.getCount() == 3);
            }

            THEN("Accessing an existing element successful, and at correct index") {
                unsigned int index_filt;
                bool res = filtered.getIndex(17, index_filt);
                REQUIRE(res == true);
                REQUIRE(index_filt == index);
            }

            THEN("Accessing a non-existing element unsuccessful") {
                unsigned int index_filt;
                bool res = filtered.getIndex(35, index_filt);
                REQUIRE(res == false);
            }
        }
    }
}

SCENARIO("UniqueArray on class without default constructor", "[UniqueArray3]") {
    class Point {
        private:
            double x, y;
        public:
            Point(double x, double y) : x(x), y(y) {}
            bool operator==(const Point& point) const {
                return (point.x == this->x) && (point.y == this->y);
            }
    };
    
    GIVEN("Class without default constructor") {

        WHEN("Attempting to construct UniqueArray") {
            UniqueArray<Point, std::equal_to<Point>> pointUA(4);

            THEN("Construction successful") {
                REQUIRE(true);
            }
        }

    }
}
