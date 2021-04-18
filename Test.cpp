#include "doctest.h"
#include "NumberWithUnits.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace ariel;
using namespace std;


TEST_CASE("simple check for operator+ and operator-"){
    NumberWithUnits a(2, "m");
    NumberWithUnits b(3, "m");
    NumberWithUnits c(1.1, "m");

    NumberWithUnits x = a+b;
    NumberWithUnits y = a-b;
    NumberWithUnits z = a+c;
    NumberWithUnits t = b-c;

    CHECK(x.get_num() == 3+2);
    CHECK(y.get_num() == 2-3);
    CHECK(z.get_num() == 2+1.1);
    CHECK(t.get_num() == 3-1.1);
}


TEST_CASE("check for operator+ and operator-, from diffrent type"){
    std::ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file);
    NumberWithUnits a(2, "m");
    NumberWithUnits b(30, "cm");

    NumberWithUnits x = a+b;
    NumberWithUnits y = b+a;

    CHECK(x.get_num() == 2.3);
    CHECK(y.get_num() == 230);
}


TEST_CASE("check for the unary operators: +() and -()"){
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file);
    NumberWithUnits a(2, "km");
    NumberWithUnits b(30, "min");

    NumberWithUnits x = -a;
    NumberWithUnits y = +b;

    CHECK(x.get_num() == -2);
    CHECK(x.get_type() == "km");
    CHECK(y.get_num() == 30);
    CHECK(y.get_type() == "min");
    CHECK((x+a).get_num() == 0);
}


TEST_CASE("simple check for friend operator*"){
    NumberWithUnits a(2, "m");
    NumberWithUnits b(3, "m");
    NumberWithUnits c(1.1, "m");

    NumberWithUnits x = 2*a;
    NumberWithUnits y = b*(-5);
    NumberWithUnits z = c*10;

    CHECK(x.get_num() == 4);
    CHECK(y.get_num() == -15);
    CHECK(z.get_num() == 11);
}


TEST_CASE("compare operators: == and !="){
    ifstream units_file{"units1.txt"};
    NumberWithUnits::read_units(units_file); 
    //  1 xx = 1000 yy
    //  1 yy = 100 zz )
    NumberWithUnits a(2, "xx");
    NumberWithUnits b(2, "yy");
    NumberWithUnits c(20, "yy");
    NumberWithUnits d(200, "zz");
    NumberWithUnits e(2000, "yy");


    CHECK(a == a);
    CHECK(a != b);
    CHECK(a != c);
    CHECK(a == e);
    CHECK(b == d);
}


TEST_CASE("compare operators: >, =>, <, <="){
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file); 
    NumberWithUnits a(1, "min");
    NumberWithUnits b(5, "sec");
    NumberWithUnits c(60, "sec");

    NumberWithUnits x(1, "USD");
    NumberWithUnits y(1, "ILS");

    CHECK(a > b);
    CHECK(a >= c);
    CHECK(a <= c);
    CHECK(y < x);
}


TEST_CASE("placement operators: +=, -="){
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file); 
    NumberWithUnits a(2, "hour");
    NumberWithUnits b(30, "min");
    NumberWithUnits c(60, "sec");
    a += b;
    b -= c; 

    CHECK(a.get_num() == 2.5);
    CHECK(b.get_num() == 29);
}


TEST_CASE("check for operators: ++ and -- , prefix and postfix"){
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file); 
    NumberWithUnits a(1, "ton");
    NumberWithUnits b(1, "min");
    NumberWithUnits c(1, "ILS");
    NumberWithUnits d(1, "m");
    
    NumberWithUnits x = a++;    // postfix
    NumberWithUnits y = ++b;    // prefix
    NumberWithUnits z = c--;    // postfix
    NumberWithUnits t = --d;    // prefix

    CHECK(a.get_num() == 2);
    CHECK(x.get_num() == 1);

    CHECK(b.get_num() == 2);
    CHECK(y.get_num() == 2);

    CHECK(c.get_num() == 0);
    CHECK(z.get_num() == 1);

    CHECK(d.get_num() == 0);
    CHECK(t.get_num() == 0);
}


TEST_CASE("check for operators<<"){
    ifstream units_file{"units2.txt"};
    NumberWithUnits::read_units(units_file);
    // 1 smile = 10 happiness
    // 1 happiness = 100 health
    NumberWithUnits a(1, "smile");
    NumberWithUnits b(1, "happiness");
    NumberWithUnits c(1, "health");

    std::stringstream s1, s2, s3;
    s1 << a;    //  a = 1[smile]
    s2 << b;    // b = 1[happiness]
    s3 << c;    // c = 1[health]

    CHECK(s1.str() == "1[smile]");
    CHECK(s2.str() == "1[happiness]");
    CHECK(s3.str() == "1[health]");
}


TEST_CASE("check for operators>>"){
    istringstream sample_input1{"700 [ m ]"};   // with spaces
    NumberWithUnits a(0, "m");
    NumberWithUnits a_before_change = a;
    sample_input1 >> a;
    NumberWithUnits like_a_after_change(700, "m");
    CHECK(a != a_before_change);
    CHECK(a == like_a_after_change);

    istringstream sample_input2{"5[m]"};    // without spaces
    sample_input2 >> a;
    cout << a.get_num() <<", "<<a.get_type()<<endl;
    CHECK(a != like_a_after_change);
}


TEST_CASE("unlegal convertions"){
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file); 
    NumberWithUnits ton(1, "ton");
    NumberWithUnits minute(1, "min");
    NumberWithUnits monney(1, "ILS");
    NumberWithUnits meter(1, "m");

    CHECK_THROWS(ton + minute);
    CHECK_THROWS(minute - monney);
    bool a;
    CHECK_THROWS(a=(monney == meter));
    CHECK_THROWS(a=(monney != ton));
}

