#include "catch.hpp"

#include <cstring>
#include "strand.hpp"

TEST_CASE("String class", "[string]"){
    strand s[14];
    s[0] = strand("testString");
    s[1] = strand("a test string");
    s[2] = strand("");
    s[3] = strand("THIS IS AN UPPERCASE STRING");
    s[4] = strand("this is an uppercase string");
    s[5] = strand("\n");
    s[6] = strand("");
    s[7] = strand("  split  split  split  ");
    s[8] = strand("                          ");
    s[9] = strand("testString");
    s[10] = strand('x');
    s[11] = strand('\0');
    s[12] = strand("100");
    s[13] = strand(10);

    SECTION("Equality operators"){
        REQUIRE(s[0] == strand("testString"));
        REQUIRE(s[0] == s[9]);
        REQUIRE(s[2] == "");
        REQUIRE(s[1] == "a test string");
        REQUIRE(!(s[3] == s[4]));
    }
    SECTION("Assignment operators"){
        strand str;
        str = "a test string";
        REQUIRE(str == s[1]);
        str = strand("testString");
        REQUIRE(str == s[0]);
        str = "";
        REQUIRE(str == s[6]);
        str = strand("\n");
        REQUIRE(str == s[5]);
    }
    SECTION("+= operators"){
        strand str;
        str = "a test ";
        REQUIRE((str += "string") == s[1]);
        str = strand("test");
        strand temp("String");
        REQUIRE((str += temp) == s[0]);
        str = "";
        REQUIRE((str += "") == s[6]);
        str = "";
        REQUIRE((str += "\n") == s[5]);
    }
    SECTION("Addition operator"){
        REQUIRE(strand("testStringtestString") == s[0]+s[9]);
        REQUIRE(s[6] + s[6] == "");
        REQUIRE(s[5] + s[6] == strand("\n"));
        REQUIRE(s[0] + s[1] + s[2] == "testStringa test string");
    }
    SECTION("[] Operator"){
        REQUIRE(s[0][1] == 'e');
        REQUIRE(s[4][4] == ' ');
        REQUIRE(s[10][0] == 'x');
        REQUIRE(s[11][0] == '\0');
    }
    SECTION("size function"){
        REQUIRE(s[9].size() == 10);
        REQUIRE(s[2].size() == 0);
        REQUIRE(s[8].size() == 26);
        REQUIRE(s[3].size() == 27);
        REQUIRE(s[13].size() == 0);
    }
    SECTION("capacity function"){
        REQUIRE(s[9].capacity() == 10);
        REQUIRE(s[2].capacity() == 0);
        REQUIRE(s[8].capacity() == 26);
        REQUIRE(s[3].capacity() == 27);
        REQUIRE(s[13].capacity() == 10);
    }
    SECTION("capacity function"){
        REQUIRE(s[9].find("tS", 0) == 3);
        REQUIRE(s[9].find("ts", 2) == 3);
        REQUIRE(s[2].find(s[3], 4) == -1);
    }
    SECTION("Substring function"){
        REQUIRE(s[0].substr(0, 5) == "testS");
        REQUIRE(s[4].substr(0, 4) == "this");
        REQUIRE(s[4].substr(1, 3) == "his");
    }
    SECTION("c_str function"){
        REQUIRE(strcmp(s[0].c_str(), "testString") == 0);
        REQUIRE(strcmp(s[9].c_str(), s[0].c_str()) == 0);
        REQUIRE(strcmp(s[2].c_str(), "") == 0);
    }
}