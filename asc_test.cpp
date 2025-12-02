#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main()
{
    std::ifstream file("rastert_dem_241.asc"); // <-- file is in repo root
    if (!file)
    {
        std::cerr << "Could not open rastert_dem_241.asc\n";
        return 1;
    }

    std::string label;
    int ncols = 0, nrows = 0;
    double xll = 0.0, yll = 0.0, cellsize = 0.0, nodata = 0.0;

    file >> label >> ncols;
    file >> label >> nrows;
    file >> label >> xll;
    file >> label >> yll;
    file >> label >> cellsize;
    file >> label >> nodata;

    std::cout << "Header:\n";
    std::cout << "  ncols   = " << ncols << "\n";
    std::cout << "  nrows   = " << nrows << "\n";
    std::cout << "  xll     = " << xll << "\n";
    std::cout << "  yll     = " << yll << "\n";
    std::cout << "  cellsize= " << cellsize << "\n";
    std::cout << "  NODATA  = " << nodata << "\n";

    std::vector<float> heights;
    heights.resize(static_cast<std::size_t>(ncols) * nrows);

    std::size_t nonNoDataCount = 0;

    for (int r = 0; r < nrows; ++r)
    {
        for (int c = 0; c < ncols; ++c)
        {
            double v;
            file >> v;
            if (!file)
            {
                std::cerr << "Unexpected end of file at row " << r
                          << " col " << c << "\n";
                return 1;
            }
            if (v == nodata)
            {
                heights[r * ncols + c] = 0.0f; // or keep as 0 for now
            }
            else
            {
                heights[r * ncols + c] = static_cast<float>(v);
                ++nonNoDataCount;
            }
        }
    }

    std::cout << "\nGrid stats:\n";
    std::cout << "  non-NODATA cells = " << nonNoDataCount << "\n";

    // print a couple of sample cells
    auto at = [&](int row, int col) -> float
    {
        return heights[row * ncols + col];
    };

    std::cout << "  heights[0][0]         = " << at(0, 0) << "\n";
    std::cout << "  heights[nrows/2][...]= "
              << at(nrows / 2, ncols / 2) << "\n";
    std::cout << "  heights[last][last]   = "
              << at(nrows - 1, ncols - 1) << "\n";

    return 0;
}
