#include "vtk_reader.hpp"

vtkSmartPointer<vtkPolyData> readers::ReadPolyData(const char *fileName) {
    vtkSmartPointer<vtkPolyData> polyData;
    vtkNew<vtkPolyDataReader> reader;
    reader->SetFileName(fileName);
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