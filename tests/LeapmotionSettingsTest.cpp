// The Leap Motion settings read back from a document, including documents
// saved before the device had a serial.

#include <UltraLeap/LeapmotionSpecificSettings.hpp>

#include <score/serialization/JSONVisitor.hpp>

#include <catch2/catch_test_macros.hpp>
#include <score_test/App.hpp>

namespace
{
Protocols::LeapmotionSpecificSettings read(const char* text)
{
  rapidjson::Document doc;
  doc.Parse(text);
  REQUIRE(!doc.HasParseError());
  JSONWriter wrt{doc};
  Protocols::LeapmotionSpecificSettings s;
  wrt.write(s);
  return s;
}
}

TEST_CASE("Leap Motion settings without a serial", "[ultraleap]")
{
  score::test::run_in_app(
      [](const score::GUIApplicationContext&) { CHECK(read("{}").serial.isEmpty()); });
}

TEST_CASE("Leap Motion settings keep their serial", "[ultraleap]")
{
  score::test::run_in_app([](const score::GUIApplicationContext&) {
    CHECK(read(R"({"Serial": "LP12345"})").serial == "LP12345");

    Protocols::LeapmotionSpecificSettings s{.serial = "LP777"};
    JSONReader r;
    r.stream.StartObject();
    r.read(s);
    r.stream.EndObject();
    CHECK(read(r.toByteArray().constData()).serial == "LP777");
  });
}
