#include <igl/opengl/glfw/Viewer.h>
#include "vtk_reader.hpp"

int main(int argc, char* argv[]) {
    vtkSmartPointer<vtkPolyData> mesh = readers::ReadPolyData(
        "/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/data/output/eams_rigid/"
        "008_eam_008_rigid.vtk");
    vtkSmartPointer<vtkPoints> points = mesh->GetPoints();
    Eigen::MatrixXd V = Eigen::MatrixXd(static_cast<int>(points->GetNumberOfPoints()), 3);
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); ++i) {
        double p[3];
        points->GetPoint(i, p);
        V(i, 0) = p[0];
        V(i, 1) = p[1];
        V(i, 2) = p[2];
    }
    vtkSmartPointer<vtkCellArray> polys = mesh->GetPolys();
    vtkIdType npts;
    vtkIdType const* pts;

    int numFaces = static_cast<int>(polys->GetNumberOfCells());
    Eigen::MatrixXi F(numFaces, 3);

    polys->InitTraversal();
    int faceIdx = 0;
    while (polys->GetNextCell(npts, pts)) {
        if (npts == 3) {  // triangle
            F(faceIdx, 0) = static_cast<int>(pts[0]);
            F(faceIdx, 1) = static_cast<int>(pts[1]);
            F(faceIdx, 2) = static_cast<int>(pts[2]);
            ++faceIdx;
        } else {
            // Skip non-triangle face (should not be present)
        }
    }

    // Plot the mesh
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);
    viewer.launch();
}
