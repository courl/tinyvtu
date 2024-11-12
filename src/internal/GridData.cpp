#include "internal/GridData.hpp"

#include <bit>

#include "XMLWriter.hpp"

namespace {
std::string endian()
{
    if constexpr (std::endian::native == std::endian::little)
    {
        return "LittleEndian";
    }
    return "BigEndian";
}
}  // namespace

namespace tinyvtu::internal {
void GridData::write(const std::filesystem::path &file_path) const
{
    using Attr = XMLWriter::Attribute;
    XMLWriter writer(file_path);
    if (!compression_.active)
        writer.openXMLElement("VTKFile",
                              {Attr{"type", "UnstructuredGrid"}, Attr{"version", "0.1"}, Attr{"byte_order", endian()}});
    else
        writer.openXMLElement("VTKFile", {Attr{"type", "UnstructuredGrid"}, Attr{"version", "0.1"},
                                          Attr{"byte_order", endian()}, Attr{"compressor", "vtkZLibDataCompressor"}});

    writer.openXMLElement("UnstructuredGrid");
    {
        writer.openXMLElement("Piece", {Attr{"NumberOfPoints", std::to_string(point_count_)},
                                        Attr{"NumberOfCells", std::to_string(cell_count_)}});
        {
            std::uint32_t currentOffset = 0;
            writer.openXMLElement("Points");
            writer.writeDataArrayElement(points_, currentOffset);
            currentOffset += static_cast<std::uint32_t>(points_.block.size());
            writer.endXMLElement();

            writer.openXMLElement("Cells");
            writer.writeDataArrayElement(cell_connectivity_, currentOffset);
            currentOffset += static_cast<std::uint32_t>(cell_connectivity_.block.size());

            writer.writeDataArrayElement(cell_offsets_, currentOffset);
            currentOffset += static_cast<std::uint32_t>(cell_offsets_.block.size());

            writer.writeDataArrayElement(cell_types_, currentOffset);
            currentOffset += static_cast<std::uint32_t>(cell_types_.block.size());
            writer.endXMLElement();

            writer.openXMLElement("PointData");
            for (const auto &data : point_data_)
            {
                writer.writeDataArrayElement(data, currentOffset);
                currentOffset += static_cast<std::uint32_t>(data.block.size());
            }
            writer.endXMLElement();

            writer.openXMLElement("CellData");
            for (const auto &data : cell_data_)
            {
                writer.writeDataArrayElement(data, currentOffset);
                currentOffset += static_cast<std::uint32_t>(data.block.size());
            }
            writer.endXMLElement();
        }
        writer.endXMLElement();
    }
    writer.endXMLElement();

    writer.openXMLElement("AppendedData", {Attr{"encoding", "raw"}});
    {
        writer.writeContent("_", true, false);
        writer.writeData(points_.block);
        writer.writeData(cell_connectivity_.block);
        writer.writeData(cell_offsets_.block);
        writer.writeData(cell_types_.block);

        for (const auto &data : point_data_)
        {
            writer.writeData(data.block);
        }

        for (const auto &data : cell_data_)
        {
            writer.writeData(data.block);
        }

        writer.writeContent("", false, true);
    }
    writer.endXMLElement();

    writer.endXMLElement();
}
}  // namespace tinyvtu::internal
