# tinyvtu

This library allows easy creation of [vtu files](https://docs.vtk.org/en/latest/design_documents/VTKFileFormats.html),
i.e., unstructured grids. This format is especially useful to output simulations of two- or three-dimensional finite
element computations.

## Why another library?

Contrary to the vtk library, this one is "tiny". This means minimal dependencies (only one, zlib) and easy integration.
It can generate both compressed and uncompressed binary data in vtk's XML file format for unstructured grids. Hence, the
benefits of data compression are handled while the created files can be analysed with mighty visualization software like
Paraview.