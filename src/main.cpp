#include <igl/arap.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <vector>
#include "readers.hpp"

void run_arap(Eigen::MatrixXd &V, Eigen::MatrixXd &V_deformed, Eigen::MatrixXi &F, std::string &pseudo,
              Eigen::VectorXi &indices, Eigen::MatrixXd &target_points, igl::ARAPData &arap_data) {

    // Initialize ARAP (3 = 3D)
    arap_data.with_dynamics = false;
    igl::arap_precomputation(V, F, 3, indices, arap_data);

    // Solve ARAP
    igl::arap_solve(target_points, arap_data, V_deformed);

    igl::writeOBJ(
        "/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/data/output/"
        "tests/" +
            pseudo + ".obj",
        V_deformed, F);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "call with only single pseudo!" << std::endl;
        return 1;
    }
    std::string pseudo = std::string(argv[1]);
    igl::ARAPData arap_data;

    // read source mesh
    vtkSmartPointer<vtkPolyData> mesh =
        readers::ReadPolyData(("/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/"
                               "data/max_iir_meshes_clipped/"
                               "Baseline/" +
                               pseudo + ".vtk")
                                  .c_str());
    vtkSmartPointer<vtkPoints> points = mesh->GetPoints();
    Eigen::MatrixXd V = Eigen::MatrixXd(static_cast<int>(points->GetNumberOfPoints()), 3);
    vtkSmartPointer<vtkCellArray> cells = mesh->GetPolys();
    int numFaces = static_cast<int>(cells->GetNumberOfCells());
    Eigen::MatrixXi F(numFaces, 3);
    readers::fill_eigen_matrices(points, cells, V, F);

    Eigen::VectorXi indices;
    Eigen::MatrixXd target_points;

    readers::load_target_indices(
        "/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/data/output/"
        "correspondence_sets/geo_clip/" +
            pseudo + "_lge_idxs.txt",
        indices);
    readers::load_target_points(
        "/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/data/output/"
        "correspondence_sets/geo_clip/" +
            pseudo + "_eam_pts.txt",
        target_points);
    std::cout << "processing case: " << pseudo << std::endl;

    Eigen::MatrixXd source_points(indices.size(), 3);
    for (int i = 0; i < indices.size(); ++i) {
        source_points.row(i) = V.row(indices[i]);
        // std::cout << "Index == " << indices[i] << std::endl << "V.row = " << V.row(indices[i]) << std::endl;
    }

    // Plotting
    igl::opengl::glfw::Viewer viewer;

    // // Attach a menu plugin
    igl::opengl::glfw::imgui::ImGuiPlugin plugin;
    viewer.plugins.push_back(&plugin);
    igl::opengl::glfw::imgui::ImGuiMenu menu;
    plugin.widgets.push_back(&menu);
    // Customize the menu
    double doubleVariable = 0.1f;  // Shared between two menus

    // // Add content to the default menu window
    menu.callback_draw_viewer_menu = [&]() {
        // Draw parent menu content
        menu.draw_viewer_menu();

        // Add new group
        if (ImGui::CollapsingHeader("Custom-Menu", ImGuiTreeNodeFlags_DefaultOpen)) {

            if (ImGui::Button("Run ARAP", ImVec2(-1, 0))) {
                Eigen::MatrixXd V_deformed = V;
                run_arap(V, V_deformed, F, pseudo, indices, target_points, arap_data);

                // Update viewer
                viewer.data().clear();
                viewer.data().set_mesh(V_deformed, F);
                viewer.data().set_face_based(true);
                igl::writeOBJ(
                    "/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/data/output/"
                    "tests/" +
                        pseudo + ".obj",
                    V_deformed, F);
            }
        }
    };

    // // Plot the mesh
    viewer.data().set_mesh(V, F);
    viewer.data().add_points(source_points, Eigen::RowVector3d(0, 0, 1));
    viewer.data().add_points(target_points, Eigen::RowVector3d(1, 0, 0));
    viewer.data().set_face_based(true);
    viewer.launch();
}
