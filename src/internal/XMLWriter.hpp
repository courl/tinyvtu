#pragma once

#include "internal/DataBlock.hpp"

#include <stack>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>

namespace tinyvtu::internal {
	class XMLWriter final {
	public:
		struct Attribute {
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
		std::string indent() const;

		std::ofstream file_;
		std::stack<std::string> elements_;
	};
}

