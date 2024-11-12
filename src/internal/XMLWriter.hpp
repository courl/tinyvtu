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
    /**
     * @struct Attribute
     * @brief Represents an attribute in an XML element.
     *
     * The Attribute struct contains a name and its corresponding value,
     * representing a name-value pair attribute in an XML element.
     */
public:
    struct Attribute
    {
        /**
         * @brief Name of the attribute.
         *
         * This member represents the name part of an attribute
         * within an XML element. It is used alongside the attribute
         * value to form name/value pairs in XML structures.
         */
        std::string name;
        /**
         * @brief Value of the attribute.
         *
         * This member represents the value part of an attribute within an XML element.
         * It is used alongside the attribute name to form name/value pairs in XML structures.
         */
        std::string value;
    };

    /**
     * @brief Constructs an XMLWriter object and opens the specified file.
     *
     * This constructor initializes an XMLWriter object and attempts to open the given
     * file path in binary mode. If the file cannot be opened, it throws a runtime error.
     *
     * @param file_path The path to the file to be opened and written to.
     * @return An initialized object of XMLWriter ready for writing XML content.
     */
    explicit XMLWriter(const std::filesystem::path &file_path);

    ~XMLWriter();

    XMLWriter(const XMLWriter &) = delete;

    XMLWriter(XMLWriter &&) noexcept = default;

    XMLWriter &operator=(const XMLWriter &) = delete;

    XMLWriter &operator=(XMLWriter &&) noexcept = default;

    /**
     * @brief Opens an XML element and writes it to the file with given attributes.
     *
     * This method constructs the opening tag for an XML element, including any specified attributes,
     * and writes it to the associated file stream. The element is then pushed onto the elements stack
     * to track nested elements.
     *
     * @param name The name of the XML element to open.
     * @param attributes A vector of Attribute structs, each representing a name-value pair to be added as attributes to
     * the element.
     */
    void openXMLElement(const std::string &name, const std::vector<Attribute> &attributes = {});

    /**
     * @brief Closes the most recently opened XML element.
     *
     * This method writes the closing tag for the most recently opened XML element
     * to the file. It ensures the correctness of nested XML structures by popping
     * the element from the elements stack and matching it with a corresponding
     * closing tag.
     */
    void endXMLElement();

    /**
     * @brief Writes an inline XML element with attributes.
     *
     * This method constructs a self-closing XML element with the specified name and attributes,
     * and writes it to the associated file stream.
     *
     * @param name The name of the XML element to write.
     * @param attributes A vector of Attribute structs, each representing a name-value pair to be added as attributes to
     * the element.
     */
    void inlineXMLElement(const std::string &name, const std::vector<Attribute> &attributes = {});

    /**
     * @brief Writes content to the file, with optional indentation and newline.
     *
     * This method writes the given content string to the associated file stream.
     * It can optionally add indentation before the content and a newline after the content.
     *
     * @param content The content string to be written.
     * @param useIndent If true, add indentation before the content.
     * @param addNewline If true, add a newline after the content.
     */
    void writeContent(const std::string &content, bool useIndent, bool addNewline);

    /**
     * @brief Writes binary data to the file.
     *
     * This method takes a Data object (vector of unsigned 8-bit integers) and writes its content
     * to the associated file stream in binary format.
     *
     * @param content The binary data to be written to the file.
     */
    void writeData(const Data &content);

    /**
     * @brief Writes a data block as an XML element with attributes.
     *
     * This method constructs and writes a "DataArray" XML element that represents a block of data.
     * It sets attributes such as the name, number of components, type, format, and offset based on
     * the provided DataBlock and offset values.
     *
     * @param dataBlock The data block containing information about the data to be written.
     * @param offset The offset value to be used in the "offset" attribute of the XML element.
     */
    void writeDataArrayElement(const DataBlock &dataBlock, std::uint32_t offset);

private:
    [[nodiscard]] std::string indent() const;

    std::ofstream file_;
    std::stack<std::string> elements_;
};
}  // namespace tinyvtu::internal
