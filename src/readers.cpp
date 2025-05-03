#include "readers.hpp"
#include <fstream>
#include <iostream>
#include <vector>

vtkSmartPointer<vtkPolyData> readers::ReadPolyData(const char *filename) {
    vtkSmartPointer<vtkPolyData> polyData;
    vtkNew<vtkPolyDataReader> reader;
    reader->SetFileName(filename);
    reader->Update();
    polyData = reader->GetOutput();
    return polyData;
}
void readers::fill_eigen_matrices(vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkCellArray> polys,
                                  Eigen::MatrixXd &V, Eigen::MatrixXi &F) {
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); ++i) {
        double p[3];
        points->GetPoint(i, p);
        V(i, 0) = p[0];
        V(i, 1) = p[1];
        V(i, 2) = p[2];
    }

    vtkIdType npts;
    vtkIdType const *pts;
    polys->InitTraversal();
    int faceIdx = 0;
    while (polys->GetNextCell(npts, pts)) {
        if (npts == 3) {  // triangle
            F(faceIdx, 0) = static_cast<int>(pts[0]);
            F(faceIdx, 1) = static_cast<int>(pts[1]);
            F(faceIdx, 2) = static_cast<int>(pts[2]);
            ++faceIdx;
        }
        // Skip non-triangle faces (should not be present anyways...)
    }
};

int count_lines(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "File: " << filename << "could not be read." << std::endl;
        return 1;
    }
    int count = 0;
    std::string line;
    while (std::getline(file, line)) ++count;
    file.close();
    return count;
}

int readers::load_target_indices(const std::string &filename, Eigen::VectorXi &indices) {
    int num_indices = count_lines(filename) - 1;
    indices.resize(num_indices);

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Index-File could not be read for: ." << filename << std::endl;
        return 1;
    }
    int index;

    for (int i = 0; i < num_indices; ++i) {
        infile >> index;
        indices(i, 0) = index;
    }

    infile.close();
    return 0;
}

int readers::load_target_points(const std::string &filename, Eigen::MatrixXd &points) {
    int num_points = count_lines(filename);
    points.resize(num_points, 3);

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Point-File: " << filename << "could not be read." << std::endl;
        return 1;
    }

    double x, y, z;
    for (int i = 0; i < num_points; ++i) {
        infile >> x >> y >> z;
        points(i, 0) = x;
        points(i, 1) = y;
        points(i, 2) = z;
    }

    infile.close();
    return 0;
}