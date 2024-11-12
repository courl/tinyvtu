#pragma once

#include <cstdint>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

#include "internal/DataBlock.hpp"

namespace tinyvtu::internal {
/**
 * @class XMLWriter
 * @brief A utility class for generating XML content from data structures.
 *
 * The XMLWriter class provides methods to construct and write XML strings.
 * It supports creating elements, attributes, and handling nested hierarchies.
 *
 * Typical usage includes creating an instance of XMLWriter, adding elements,
 * setting attributes, and finally generating the XML string.
 */
class XMLWriter final
{
public:
    struct Attribute
    {
        std::string name;
        std::string attribute;
    };

    explicit XMLWriter(const std::filesystem::path &file_path);

    ~XMLWriter();

    XMLWriter(const XMLWriter &) = delete;

    XMLWriter(XMLWriter &&) noexcept = default;

    XMLWriter &operator=(const XMLWriter &) = delete;

    XMLWriter &operator=(XMLWriter &&) noexcept = default;

    void openXMLElement(const std::string &name, const std::vector<Attribute> &attributes = {});

    void endXMLElement();

    void inlineXMLElement(const std::string &name, const std::vector<Attribute> &attributes = {});

    void writeContent(const std::string &content, bool useIndent, bool addNewline);

    void writeData(const Data &content);

    void writeDataBlock(const DataBlock &dataBlock, std::uint32_t offset);

private:
    [[nodiscard]] std::string indent() const;

    std::ofstream file_;
    std::stack<std::string> elements_;
};
}  // namespace tinyvtu::internal
