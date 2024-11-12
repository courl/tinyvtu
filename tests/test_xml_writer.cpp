#include <catch2/catch_test_macros.hpp>

#include "internal/XMLWriter.hpp"

using namespace tinyvtu::internal;

namespace {

std::string getContents(const std::filesystem::path& path)
{
    const std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

}  // namespace

TEST_CASE("XMLWriter writes elements correctly", "[XMLWriter]")
{
    static const auto file_path = std::filesystem::path("test.xml");

    SECTION("Write element")
    {
        {
            XMLWriter writer(file_path);
            writer.openXMLElement("test");
            writer.endXMLElement();
        }
        REQUIRE(getContents(file_path) == "<test>\n</test>\n");
        {
            XMLWriter writer(file_path);
            writer.openXMLElement("test", {{.name = "attr1", .attribute = "123"}});
            writer.endXMLElement();
        }
        REQUIRE(getContents(file_path) == "<test attr1=\"123\">\n</test>\n");
        {
            XMLWriter writer(file_path);
            writer.openXMLElement("more",
                                  {{.name = "attr1", .attribute = "123"}, {.name = "attr2", .attribute = "678"}});
            writer.endXMLElement();
        }
        REQUIRE(getContents(file_path) == "<more attr1=\"123\" attr2=\"678\">\n</more>\n");
    }

    SECTION("Write nested elements")
    {
        {
            XMLWriter writer(file_path);
            writer.openXMLElement("root");
            writer.openXMLElement("child");
            writer.endXMLElement();
            writer.endXMLElement();
        }
        REQUIRE(getContents(file_path) == "<root>\n  <child>\n  </child>\n</root>\n");
    }

    SECTION("Write element with multiple attributes")
    {
        {
            XMLWriter writer(file_path);
            writer.openXMLElement("element",
                                  {{.name = "attr1", .attribute = "value1"}, {.name = "attr2", .attribute = "value2"}});
            writer.endXMLElement();
        }
        REQUIRE(getContents(file_path) == "<element attr1=\"value1\" attr2=\"value2\">\n</element>\n");
    }

    SECTION("Write inline element")
    {
        {
            XMLWriter writer(file_path);
            writer.inlineXMLElement("inline");
        }
        REQUIRE(getContents(file_path) == "<inline/>\n");

        {
            XMLWriter writer(file_path);
            writer.inlineXMLElement("inline", {{.name = "attr1", .attribute = "123"}});
        }
        REQUIRE(getContents(file_path) == "<inline attr1=\"123\"/>\n");

        {
            XMLWriter writer(file_path);
            writer.inlineXMLElement("inline",
                                    {{.name = "attr1", .attribute = "123"}, {.name = "attr2", .attribute = "678"}});
        }
        REQUIRE(getContents(file_path) == "<inline attr1=\"123\" attr2=\"678\"/>\n");
    }

    SECTION("Write content")
    {
        {
            XMLWriter writer(file_path);
            writer.openXMLElement("test");
            writer.writeContent("Hello, World!", true, true);
            writer.endXMLElement();
        }
        REQUIRE(getContents(file_path) == "<test>\n  Hello, World!\n</test>\n");

        {
            XMLWriter writer(file_path);
            writer.openXMLElement("test");
            writer.writeContent("Hello, World!", false, false);
            writer.endXMLElement();
        }
        REQUIRE(getContents(file_path) == "<test>\nHello, World!</test>\n");
    }

    SECTION("Write data")
    {
        {
            XMLWriter writer(file_path);
            writer.writeData(Data{});
        }
        REQUIRE(getContents(file_path).empty());
        {
            const auto data = Data{'H', 'e', 'l', 'l', 'o'};
            XMLWriter writer(file_path);
            writer.writeData(data);
        }
        REQUIRE(getContents(file_path) == "Hello");
    }

    SECTION("Write data block")
    {
        {
            DataBlock dataBlock{DataBlock::Type::Float32, "float32_block", 3, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 10);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"float32_block\" NumberOfComponents=\"3\" type=\"Float32\" format=\"appended\" "
                "offset=\"10\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::Float64, "float64_block", 2, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 20);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"float64_block\" NumberOfComponents=\"2\" type=\"Float64\" format=\"appended\" "
                "offset=\"20\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::Int8, "int8_block", 4, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 30);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"int8_block\" NumberOfComponents=\"4\" type=\"Int8\" format=\"appended\" "
                "offset=\"30\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::UInt8, "uint8_block", 2, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 40);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"uint8_block\" NumberOfComponents=\"2\" type=\"UInt8\" format=\"appended\" "
                "offset=\"40\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::Int16, "int16_block", 3, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 50);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"int16_block\" NumberOfComponents=\"3\" type=\"Int16\" format=\"appended\" "
                "offset=\"50\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::UInt16, "uint16_block", 2, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 60);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"uint16_block\" NumberOfComponents=\"2\" type=\"UInt16\" format=\"appended\" "
                "offset=\"60\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::Int32, "int32_block", 5, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 70);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"int32_block\" NumberOfComponents=\"5\" type=\"Int32\" format=\"appended\" "
                "offset=\"70\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::UInt32, "uint32_block", 2, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 80);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"uint32_block\" NumberOfComponents=\"2\" type=\"UInt32\" format=\"appended\" "
                "offset=\"80\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::Int64, "int64_block", 3, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 90);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"int64_block\" NumberOfComponents=\"3\" type=\"Int64\" format=\"appended\" "
                "offset=\"90\"/>\n");

        {
            DataBlock dataBlock{DataBlock::Type::UInt64, "uint64_block", 2, Data{}};
            XMLWriter writer(file_path);
            writer.writeDataArrayElement(dataBlock, 100);
        }
        REQUIRE(getContents(file_path) ==
                "<DataArray Name=\"uint64_block\" NumberOfComponents=\"2\" type=\"UInt64\" format=\"appended\" "
                "offset=\"100\"/>\n");
    }
}
