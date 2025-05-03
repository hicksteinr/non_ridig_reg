#pragma once

// #include <igl/opengl/glfw/Viewer.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <Eigen/Geometry>
#include <string>

namespace readers {
vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName);
void fill_eigen_matrices(vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkCellArray> polys, Eigen::MatrixXd &V,
                         Eigen::MatrixXi &F);
}  // namespace readers