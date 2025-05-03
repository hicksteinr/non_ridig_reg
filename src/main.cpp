#include <igl/arap.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include "vtk_reader.hpp"

igl::ARAPData arap_data;

int main(int argc, char* argv[]) {

    // read points
    vtkSmartPointer<vtkPolyData> mesh = readers::ReadPolyData(
        "/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/src/non_ridig_reg/src/"
        // "demodata/018_eam_018_rigid.vtk");
        "demodata/022.vtk");
    vtkSmartPointer<vtkPoints> points = mesh->GetPoints();
    Eigen::MatrixXd V = Eigen::MatrixXd(static_cast<int>(points->GetNumberOfPoints()), 3);
    vtkSmartPointer<vtkCellArray> polys = mesh->GetPolys();
    int numFaces = static_cast<int>(polys->GetNumberOfCells());
    Eigen::MatrixXi F(numFaces, 3);
    readers::fill_eigen_matrices(points, polys, V, F);

    // redobj part
    // Eigen::MatrixXi F;
    // Eigen::MatrixXd V;
    // igl::readOBJ(
    //     "/Users/richardhickstein/Documents/Development/Projects/LaFire/lafire_analysis_24/src/non_ridig_reg/src/"
    //     "demodata/bunny.obj",
    //     V, F);

    // Plotting
    igl::opengl::glfw::Viewer viewer;

    // Attach a menu plugin
    igl::opengl::glfw::imgui::ImGuiPlugin plugin;
    viewer.plugins.push_back(&plugin);
    igl::opengl::glfw::imgui::ImGuiMenu menu;
    plugin.widgets.push_back(&menu);
    // Customize the menu
    double doubleVariable = 0.1f;  // Shared between two menus

    // Add content to the default menu window
    menu.callback_draw_viewer_menu = [&]() {
        // Draw parent menu content
        menu.draw_viewer_menu();

        // Add new group
        if (ImGui::CollapsingHeader("Custom-Menu", ImGuiTreeNodeFlags_DefaultOpen)) {
            // // Expose variable directly ...
            // ImGui::InputDouble("double", &doubleVariable, 0, 0, "%.4f");

            // ... or using a custom callback
            // static bool boolVariable = true;
            // if (ImGui::Checkbox("bool", &boolVariable)) {
            //     // do something
            //     std::cout << "boolVariable: " << std::boolalpha << boolVariable << std::endl;
            // }

            // // Expose an enumeration type
            // enum Orientation { Up = 0, Down, Left, Right };
            // static Orientation dir = Up;
            // ImGui::Combo("Direction", (int*)(&dir), "Up\0Down\0Left\0Right\0\0");

            // // We can also use a std::vector<std::string> defined dynamically
            // static int num_choices = 3;
            // static std::vector<std::string> choices;
            // static int idx_choice = 0;
            // if (ImGui::InputInt("Num letters", &num_choices)) {
            //     num_choices = std::max(1, std::min(26, num_choices));
            // }
            // if (num_choices != (int)choices.size()) {
            //     choices.resize(num_choices);
            //     for (int i = 0; i < num_choices; ++i) choices[i] = std::string(1, 'A' + i);
            //     if (idx_choice >= num_choices) idx_choice = num_choices - 1;
            // }
            // ImGui::Combo("Letter", &idx_choice, choices);

            // Add a button
            if (ImGui::Button("Load Mesh", ImVec2(-1, 0))) {
                std::cout << "Hello\n";
            }
            if (ImGui::Button("Run ARAP", ImVec2(-1, 0))) {
                // Simple 3D ARAP example with one fixed point
                Eigen::MatrixXd V_deformed = V;

                // Constraint: fix vertex 0 and move vertex 10 upward
                Eigen::VectorXi b(60);

                Eigen::MatrixXd bc(60, 3);
                for (int i = 0; i < 60; ++i) {
                    b(i) = i;
                    bc.row(i) = V.row(i) + Eigen::RowVector3d(0, 0.5, 0);  // fixed point
                    // bc.row(1) = V.row(10) + Eigen::RowVector3d(0, 0.5, 0);  // move point 10 upward}
                }
                // Initialize ARAP (3 = 3D)
                arap_data.with_dynamics = false;
                igl::arap_precomputation(V, F, 3, b, arap_data);

                // Solve ARAP
                igl::arap_solve(bc, arap_data, V_deformed);

                // Update viewer
                viewer.data().clear();
                viewer.data().set_mesh(V_deformed, F);
                viewer.data().set_face_based(true);
            }
        }
    };

    // Plot the mesh
    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);
    viewer.launch();
}
