#include "XMLWriter.hpp"

#include <stdexcept>
#include <string>

namespace tinyvtu::internal {
	XMLWriter::XMLWriter(const std::filesystem::path &file_path)
		: file_(file_path, std::ios::binary) {
		if (!file_.is_open())
			throw std::runtime_error(std::string("[Writer::Writer] could not open file '") + file_path.string() + "'");
	}

	XMLWriter::~XMLWriter() {
		file_.close();
	}

	void XMLWriter::openXMLElement(const std::string &name, const std::vector<Attribute> &attributes) {
		file_ << indent() << "<" << name;
		for (const auto &[name, attribute]: attributes)
			file_ << " " << name << "=\"" << attribute << "\"";
		file_ << ">\n";

		elements_.push(name);
	}

	void XMLWriter::endXMLElement() {
		const auto name = elements_.top();
		elements_.pop();

		file_ << indent() << "</" << name << ">\n";
	}

	void XMLWriter::inlineXMLElement(const std::string &name, const std::vector<Attribute> &attributes) {
		file_ << indent() << "<" << name;
		for (auto &attr: attributes)
			file_ << " " << attr.name << "=\"" << attr.attribute << "\"";
		file_ << "/>\n";
	}

	void XMLWriter::writeContent(const std::string &content, bool useIndent, bool addNewline) {
		if (useIndent)
			file_ << indent();
		file_ << content;
		if (addNewline)
			file_ << "\n";
	}

	void XMLWriter::writeData(const Data &content) {
		file_.write(reinterpret_cast<const char *>(content.data()), static_cast<std::streamsize>(content.size()));
	}


	void XMLWriter::writeDataBlock(const DataBlock &dataBlock, std::uint32_t offset) {
		constexpr char TypeToName[][10] = {
			"Float32", "Float64",
			"Int8", "UInt8",
			"Int16", "UInt16",
			"Int32", "UInt32",
			"Int64", "UInt64"
		};

		auto attributes = std::vector<Attribute>();
		if (!dataBlock.name.empty())
			attributes.emplace_back(Attribute{"Name", dataBlock.name});
		if (dataBlock.number_of_components != 0)
			attributes.emplace_back(Attribute{"NumberOfComponents", std::to_string(dataBlock.number_of_components)});
		attributes.emplace_back(Attribute{"type", TypeToName[dataBlock.type]});
		attributes.emplace_back(Attribute{"format", "appended"});
		attributes.emplace_back(Attribute{"offset", std::to_string(offset)});

		inlineXMLElement("DataArray", attributes);
	}

	std::string XMLWriter::indent() const {
		return std::string(elements_.size() * 2, ' ');
	}
}
