#include "TiledArray/variable_list.h"
#include "TiledArray/permutation.h"
#include "unit_test_config.h"

//using namespace TiledArray;
using TiledArray::Permutation;
using TiledArray::expressions::VariableList;

struct VariableListFixture {
  VariableListFixture() : v("a,b,c,d"), v_aib("a,i,b"), v_xiy("x,i,y"),
      v_ai("a,i"), v_xi("x,i"), v_ia("i,a"), v_ix("i,x"), v_i("i"), v_0("") { }

  VariableList v;
  const VariableList v_aib;
  const VariableList v_xiy;
  const VariableList v_ai;
  const VariableList v_xi;
  const VariableList v_ia;
  const VariableList v_ix;
  const VariableList v_i;
  const VariableList v_0;
};

BOOST_FIXTURE_TEST_SUITE( variable_list_suite , VariableListFixture )

BOOST_AUTO_TEST_CASE( valid_chars )
{
  // Check for valid characters in string input

  // Check letters 'a' - 'z'
  BOOST_CHECK_NO_THROW(VariableList v1("a"));
  BOOST_CHECK_NO_THROW(VariableList v1("b"));
  BOOST_CHECK_NO_THROW(VariableList v1("c"));
  BOOST_CHECK_NO_THROW(VariableList v1("d"));
  BOOST_CHECK_NO_THROW(VariableList v1("e"));
  BOOST_CHECK_NO_THROW(VariableList v1("f"));
  BOOST_CHECK_NO_THROW(VariableList v1("g"));
  BOOST_CHECK_NO_THROW(VariableList v1("h"));
  BOOST_CHECK_NO_THROW(VariableList v1("i"));
  BOOST_CHECK_NO_THROW(VariableList v1("j"));
  BOOST_CHECK_NO_THROW(VariableList v1("k"));
  BOOST_CHECK_NO_THROW(VariableList v1("l"));
  BOOST_CHECK_NO_THROW(VariableList v1("m"));
  BOOST_CHECK_NO_THROW(VariableList v1("n"));
  BOOST_CHECK_NO_THROW(VariableList v1("o"));
  BOOST_CHECK_NO_THROW(VariableList v1("p"));
  BOOST_CHECK_NO_THROW(VariableList v1("q"));
  BOOST_CHECK_NO_THROW(VariableList v1("r"));
  BOOST_CHECK_NO_THROW(VariableList v1("s"));
  BOOST_CHECK_NO_THROW(VariableList v1("t"));
  BOOST_CHECK_NO_THROW(VariableList v1("u"));
  BOOST_CHECK_NO_THROW(VariableList v1("v"));
  BOOST_CHECK_NO_THROW(VariableList v1("w"));
  BOOST_CHECK_NO_THROW(VariableList v1("x"));
  BOOST_CHECK_NO_THROW(VariableList v1("y"));
  BOOST_CHECK_NO_THROW(VariableList v1("z"));

  // Check letters 'A' - 'Z'
  BOOST_CHECK_NO_THROW(VariableList v1("A"));
  BOOST_CHECK_NO_THROW(VariableList v1("B"));
  BOOST_CHECK_NO_THROW(VariableList v1("C"));
  BOOST_CHECK_NO_THROW(VariableList v1("D"));
  BOOST_CHECK_NO_THROW(VariableList v1("E"));
  BOOST_CHECK_NO_THROW(VariableList v1("F"));
  BOOST_CHECK_NO_THROW(VariableList v1("G"));
  BOOST_CHECK_NO_THROW(VariableList v1("H"));
  BOOST_CHECK_NO_THROW(VariableList v1("I"));
  BOOST_CHECK_NO_THROW(VariableList v1("J"));
  BOOST_CHECK_NO_THROW(VariableList v1("K"));
  BOOST_CHECK_NO_THROW(VariableList v1("L"));
  BOOST_CHECK_NO_THROW(VariableList v1("M"));
  BOOST_CHECK_NO_THROW(VariableList v1("N"));
  BOOST_CHECK_NO_THROW(VariableList v1("O"));
  BOOST_CHECK_NO_THROW(VariableList v1("P"));
  BOOST_CHECK_NO_THROW(VariableList v1("Q"));
  BOOST_CHECK_NO_THROW(VariableList v1("R"));
  BOOST_CHECK_NO_THROW(VariableList v1("S"));
  BOOST_CHECK_NO_THROW(VariableList v1("T"));
  BOOST_CHECK_NO_THROW(VariableList v1("U"));
  BOOST_CHECK_NO_THROW(VariableList v1("V"));
  BOOST_CHECK_NO_THROW(VariableList v1("W"));
  BOOST_CHECK_NO_THROW(VariableList v1("X"));
  BOOST_CHECK_NO_THROW(VariableList v1("Y"));
  BOOST_CHECK_NO_THROW(VariableList v1("Z"));

  // Check characters '0' - '9'
  BOOST_CHECK_NO_THROW(VariableList v1("0"));
  BOOST_CHECK_NO_THROW(VariableList v1("1"));
  BOOST_CHECK_NO_THROW(VariableList v1("2"));
  BOOST_CHECK_NO_THROW(VariableList v1("3"));
  BOOST_CHECK_NO_THROW(VariableList v1("4"));
  BOOST_CHECK_NO_THROW(VariableList v1("5"));
  BOOST_CHECK_NO_THROW(VariableList v1("5"));
  BOOST_CHECK_NO_THROW(VariableList v1("6"));
  BOOST_CHECK_NO_THROW(VariableList v1("7"));
  BOOST_CHECK_NO_THROW(VariableList v1("8"));
  BOOST_CHECK_NO_THROW(VariableList v1("9"));

  // Check characters ',', ' ', and '\0'
  BOOST_CHECK_NO_THROW(VariableList v1("a,b"));
  BOOST_CHECK_NO_THROW(VariableList v1("a ,b"));
  BOOST_CHECK_NO_THROW(VariableList v1(""));
}

BOOST_AUTO_TEST_CASE( accessors )
{
  BOOST_CHECK_EQUAL(v.dim(), 4u); // check for variable count
  BOOST_CHECK_EQUAL(v.at(0), "a"); // check 1st variable access
  BOOST_CHECK_EQUAL(v.at(3), "d"); // check last variable access
  BOOST_CHECK_EQUAL(v[0], "a"); // check 1st variable access
  BOOST_CHECK_EQUAL(v[3], "d"); // check last variable access
  BOOST_CHECK_THROW(v.at(4), std::out_of_range); // check for out of range throw.
}

BOOST_AUTO_TEST_CASE( constructor )
{
  BOOST_REQUIRE_NO_THROW(VariableList v0); // Check default constructor
  VariableList v0;
  BOOST_CHECK_EQUAL(v0.dim(), 0u);

  BOOST_REQUIRE_NO_THROW(VariableList v1("a,b,c,d")); // check string constructor
  VariableList v1("a,b,c,d");
  BOOST_CHECK_EQUAL(v1.dim(), 4u);
  BOOST_CHECK_EQUAL(v1.at(0), "a");  // check for corret data
  BOOST_CHECK_EQUAL(v1.at(1), "b");
  BOOST_CHECK_EQUAL(v1.at(2), "c");
  BOOST_CHECK_EQUAL(v1.at(3), "d");

  BOOST_REQUIRE_NO_THROW(VariableList v2(v)); // check string constructor
  VariableList v2(v);
  BOOST_CHECK_EQUAL(v2.dim(), 4u);
  BOOST_CHECK_EQUAL(v2.at(0), "a");  // check for corret data
  BOOST_CHECK_EQUAL(v2.at(1), "b");
  BOOST_CHECK_EQUAL(v2.at(2), "c");
  BOOST_CHECK_EQUAL(v2.at(3), "d");

  boost::array<std::string, 4> a10 = {{"a", "b", "c", "d"}};
  BOOST_REQUIRE_NO_THROW(VariableList v10(a10.begin(), a10.end())); // check iterator constructor
  VariableList v10(a10.begin(), a10.end());
  BOOST_CHECK_EQUAL(v10.dim(), 4u);
  BOOST_CHECK_EQUAL(v10.at(0), "a");  // check for corret data
  BOOST_CHECK_EQUAL(v10.at(1), "b");
  BOOST_CHECK_EQUAL(v10.at(2), "c");
  BOOST_CHECK_EQUAL(v10.at(3), "d");

  BOOST_CHECK_THROW(VariableList v3(",a,b,c"), std::runtime_error); // check invalid input
  BOOST_CHECK_THROW(VariableList v4("a,,b,c"), std::runtime_error);
  BOOST_CHECK_THROW(VariableList v5(" ,a,b"), std::runtime_error);
  BOOST_CHECK_THROW(VariableList v6("a,  b,   , c"), std::runtime_error);
  BOOST_CHECK_THROW(VariableList v8("a,b,a,c"), std::runtime_error);
  BOOST_CHECK_THROW(VariableList v9("a,a,b,c"), std::runtime_error);

  VariableList v7(" a , b, c, d , e e ,f f, g10,h, i "); // check input with various spacings.
  BOOST_CHECK_EQUAL(v7.at(0), "a");
  BOOST_CHECK_EQUAL(v7.at(1), "b");
  BOOST_CHECK_EQUAL(v7.at(2), "c");
  BOOST_CHECK_EQUAL(v7.at(3), "d");
  BOOST_CHECK_EQUAL(v7.at(4), "ee");
  BOOST_CHECK_EQUAL(v7.at(5), "ff");
  BOOST_CHECK_EQUAL(v7.at(6), "g10");
  BOOST_CHECK_EQUAL(v7.at(7), "h");
  BOOST_CHECK_EQUAL(v7.at(8), "i");

  BOOST_REQUIRE_NO_THROW(VariableList v11("")); // Check zero length constructor
  VariableList v11("");
  BOOST_CHECK_EQUAL(v11.dim(), 0u);

}

BOOST_AUTO_TEST_CASE( iterator )
{
  boost::array<std::string, 4> a1 = {{"a", "b", "c", "d"}};

  BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), a1.begin(), a1.end()); // check that all values are equal
  boost::array<std::string, 4>::const_iterator it_a = a1.begin();
  for(VariableList::const_iterator it = v.begin(); it != v.end(); ++it, ++it_a) // check basic iterator functionality.
    BOOST_CHECK_EQUAL(*it, *it_a);

}

BOOST_AUTO_TEST_CASE( comparison )
{
  VariableList v1("a,b,c,d");
  VariableList v2("d,b,c,a");

  BOOST_CHECK(v1 == v);    // check variable list comparison operators
  BOOST_CHECK(!(v2 == v));
  BOOST_CHECK(v2 != v);
  BOOST_CHECK(!(v1 != v));
}

BOOST_AUTO_TEST_CASE( assignment )
{
  VariableList v1;
  v1 = v;
  BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v.begin(), v.end());

  VariableList v2;
  v2 = "a,b,c,d";
  BOOST_CHECK_EQUAL_COLLECTIONS(v2.begin(), v2.end(), v.begin(), v.end());
}

BOOST_AUTO_TEST_CASE( permutation )
{
  Permutation<4> p(1,2,3,0);
  VariableList v1(v);
  VariableList v2 = (p ^ v1);
  BOOST_CHECK_EQUAL(v2[0], "d");
  BOOST_CHECK_EQUAL(v2[1], "a");
  BOOST_CHECK_EQUAL(v2[2], "b");
  BOOST_CHECK_EQUAL(v2[3], "c");

  VariableList v3(v);
  v3 ^= p;
  BOOST_CHECK_EQUAL_COLLECTIONS(v3.begin(), v3.end(), v2.begin(), v2.end());
}
/*
BOOST_AUTO_TEST_CASE( implicit_permutation )
{
  Permutation<4> p1(1,2,3,0);
  VariableList v1 = p1 ^ v;
  std::vector<std::size_t> p = v.permutation(v1);

  BOOST_CHECK_EQUAL_COLLECTIONS(p.begin(), p.end(), p1.begin(), p1.end());
}
*/
BOOST_AUTO_TEST_CASE( ostream )
{
  boost::test_tools::output_test_stream output;
  output << v;
  BOOST_CHECK( !output.is_empty( false ) ); // check for correct output.
  BOOST_CHECK( output.check_length( 12, false ) );
  BOOST_CHECK( output.is_equal("(a, b, c, d)") );
}

BOOST_AUTO_TEST_SUITE_END()
