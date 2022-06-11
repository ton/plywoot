// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <plywood/plywood.hpp>

#include <fstream>

using namespace Catch::Matchers;

namespace
{
  class ExceptionMessageContainsMatcher final : public MatcherBase<std::exception>
  {
    std::string m_messagePart;

  public:
    ExceptionMessageContainsMatcher(const std::string &messagePart) : m_messagePart{messagePart} {}

    bool match(const std::exception &e) const override
    {
      return ::strstr(e.what(), m_messagePart.data()) != nullptr;
    }

    std::string describe() const override
    {
      return "exception message contains \"" + m_messagePart + '"';
    }
  };

  ExceptionMessageContainsMatcher MessageContains(const std::string &message) { return {message}; }
}

TEST_CASE("Input file does not exist")
{
  std::ifstream ifs{"test/input/missing.ply", std::ios::in};
  REQUIRE_THROWS_AS(plywood::PlyFile(ifs), plywood::UnexpectedToken);
}

TEST_CASE("Input file is not a PLY file")
{
  std::ifstream ifs{"test/input/invalid.ply"};
  REQUIRE_THROWS_AS(plywood::PlyFile(ifs), plywood::UnexpectedToken);
}

TEST_CASE("Input file does not contain a format definition", "[format]")
{
  std::ifstream ifs{"test/input/missing_format.ply"};
  REQUIRE_THROWS_AS(plywood::PlyFile(ifs), plywood::UnexpectedToken);
}

TEST_CASE("Input file does contain a format definition, but not in the right order", "[format]")
{
  std::ifstream ifs{"test/input/format_in_wrong_order.ply"};
  REQUIRE_THROWS_AS(plywood::PlyFile(ifs), plywood::UnexpectedToken);
}

TEST_CASE("Input file does contain a format definition, but for some invalid format", "[format]")
{
  std::ifstream ifs{"test/input/invalid_format.ply"};
  REQUIRE_THROWS_AS(plywood::PlyFile(ifs), plywood::InvalidFormat);
}

TEST_CASE(
    "Input file does contains a binary little endian format definition, which is not supported", "[format]")
{
  std::ifstream ifs{"test/input/binary_little_endian.ply"};
  REQUIRE_THROWS_AS(plywood::PlyFile(ifs), plywood::UnsupportedFormat);
}

TEST_CASE("Input file contains a binary little endian format definition, which is not supported", "[format]")
{
  std::ifstream ifs{"test/input/binary_big_endian.ply"};
  REQUIRE_THROWS_AS(plywood::PlyFile(ifs), plywood::UnsupportedFormat);
}

TEST_CASE("Element definition does not contain the number of elements", "[element]")
{
  std::ifstream ifs{"test/input/missing_element_size.ply"};
  REQUIRE_THROWS_MATCHES(
      plywood::PlyFile(ifs), plywood::UnexpectedToken,
      MessageContains("'end_header'") && MessageContains("'<number>'"));
}

TEST_CASE("A single element definition without properties is correctly parsed", "[element]")
{
  std::ifstream ifs{"test/input/single_element.ply"};
  const plywood::PlyFile plyFile{ifs};
  const std::vector<plywood::PlyElement> elements{plyFile.elements()};
  REQUIRE(elements.size() == 1);
  REQUIRE(elements.front().name == "vertex");
  REQUIRE(elements.front().size == 0);
}

TEST_CASE("Multiple element definitions without properties are correctly parsed", "[element]")
{
  std::ifstream ifs{"test/input/multiple_elements.ply"};
  const plywood::PlyFile plyFile{ifs};
  const std::vector<plywood::PlyElement> elements{plyFile.elements()};
  REQUIRE(elements.size() == 2);
  REQUIRE(elements.front().name == "vertex");
  REQUIRE(elements.front().size == 0);
  REQUIRE(elements.back().name == "face");
  REQUIRE(elements.back().size == 0);
}

TEST_CASE("A single element definition with properties is correctly parsed", "[element]")
{
  std::ifstream ifs{"test/input/single_element_with_properties.ply"};
  const plywood::PlyFile plyFile{ifs};
  const std::vector<plywood::PlyElement> elements{plyFile.elements()};
  REQUIRE(elements.size() == 1);

  const plywood::PlyElement &element{elements.front()};
  REQUIRE(element.name == "vertex");
  REQUIRE(element.size == 0);
  REQUIRE(element.properties.size() == 9);

  REQUIRE(element.properties[0].name == "a");
  REQUIRE_FALSE(element.properties[0].isList);
  REQUIRE(element.properties[0].type == plywood::PlyDataType::Char);

  REQUIRE(element.properties[1].name == "b");
  REQUIRE_FALSE(element.properties[1].isList);
  REQUIRE(element.properties[1].type == plywood::PlyDataType::UChar);

  REQUIRE(element.properties[2].name == "c");
  REQUIRE_FALSE(element.properties[2].isList);
  REQUIRE(element.properties[2].type == plywood::PlyDataType::Short);

  REQUIRE(element.properties[3].name == "d");
  REQUIRE_FALSE(element.properties[3].isList);
  REQUIRE(element.properties[3].type == plywood::PlyDataType::UShort);

  REQUIRE(element.properties[4].name == "e");
  REQUIRE_FALSE(element.properties[4].isList);
  REQUIRE(element.properties[4].type == plywood::PlyDataType::Int);

  REQUIRE(element.properties[5].name == "f");
  REQUIRE_FALSE(element.properties[5].isList);
  REQUIRE(element.properties[5].type == plywood::PlyDataType::UInt);

  REQUIRE(element.properties[6].name == "g");
  REQUIRE_FALSE(element.properties[6].isList);
  REQUIRE(element.properties[6].type == plywood::PlyDataType::Float);

  REQUIRE(element.properties[7].name == "h");
  REQUIRE_FALSE(element.properties[7].isList);
  REQUIRE(element.properties[7].type == plywood::PlyDataType::Double);

  REQUIRE(element.properties[8].name == "i");
  REQUIRE(element.properties[8].isList);
  REQUIRE(element.properties[8].type == plywood::PlyDataType::Int);
  REQUIRE(element.properties[8].sizeType == plywood::PlyDataType::UChar);
}
