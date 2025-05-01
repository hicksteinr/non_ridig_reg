#pragma once

#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <string>

namespace readers {
vtkSmartPointer<vtkPolyData> ReadPolyData(const char* fileName);
}