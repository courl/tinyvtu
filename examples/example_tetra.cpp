#include <cmath>

#include "tinyvtu.hpp"

int main()
{
    static std::vector<std::array<float, 3> > points{{0., 0., 0.}, {1., 0., 0.}, {0., 1., 0.}, {0., 0., 1.},
                                                     {1., 0., 1.}, {1., 1., 0.}, {0., 1., 1.}, {1., 1., 1.}};
    static std::vector<std::vector<std::int32_t> > cells{{0, 1, 5, 7}, {0, 1, 7, 4}, {5, 7, 6, 0},
                                                         {5, 0, 6, 2}, {0, 7, 3, 6}, {7, 3, 0, 4}};
    auto grid = tinyvtu::createGrid(points, tinyvtu::CellType::Tetra, cells);

    grid.addPointData("u", std::vector<float>{0.f, std::sqrt(1.f), std::sqrt(1.f), std::sqrt(1.f), std::sqrt(2.f),
                                              std::sqrt(2.f), std::sqrt(2.f), std::sqrt(3.f)});
    grid.addCellData("number", std::vector<std::uint8_t>{0, 1, 2, 3, 4, 5});

    grid.write("out.vtu");

    return 0;
}
