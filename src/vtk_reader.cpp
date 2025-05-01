#include "vtk_reader.hpp"

vtkSmartPointer<vtkPolyData> readers::ReadPolyData(const char* fileName) {
    vtkSmartPointer<vtkPolyData> polyData;
    vtkNew<vtkPolyDataReader> reader;
    reader->SetFileName(fileName);
    reader->Update();
    polyData = reader->GetOutput();
    return polyData;
}