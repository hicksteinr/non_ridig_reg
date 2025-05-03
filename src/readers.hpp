#pragma once

// #include <igl/opengl/glfw/Viewer.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <string>

namespace readers {
vtkSmartPointer<vtkPolyData> ReadPolyData(const char *filename);
void fill_eigen_matrices(vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkCellArray> polys, Eigen::MatrixXd &V,
                         Eigen::MatrixXi &F);
int load_target_indices(const std::string &filename, Eigen::VectorXi &indices);
int load_target_points(const std::string &filename, Eigen::MatrixXd &points);
}  // namespace readers