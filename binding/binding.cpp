#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <nfd.hpp>
#include <string>

std::tuple<bool, std::string> open_file_dialog(const std::vector<std::vector<std::string>>& filter_list,
                                               const std::string& default_path) {
  NFD::Guard nfdGuard;

  // auto-freeing memory
  NFD::UniquePath outPath;

  // prepare filters for the dialog
  const size_t filterCount = filter_list.size();
  nfdfilteritem_t* filterItem = nullptr;

  if (filterCount > 0) {
    filterItem = (nfdfilteritem_t*)malloc(sizeof(nfdfilteritem_t) * filterCount);
    for (size_t i = 0; i < filterCount; ++i) {
      filterItem[i].name = strdup(filter_list[i][0].c_str());
      filterItem[i].spec = strdup(filter_list[i][1].c_str());
    }
  }

  // set the default path
  const nfdnchar_t* defaultPath = default_path.empty() ? nullptr : default_path.c_str();

  // show the dialog
  const nfdresult_t result = NFD::OpenDialog(outPath,
                                             filterItem,
                                             filterCount,
                                             defaultPath);

  // Clean up the filter item
  free(filterItem);

  std::string selectedPath;
  if (result == NFD_OKAY) {
    selectedPath = std::string(outPath.get());
  }

  // NFD::Guard will automatically quit NFD.
  return {result == NFD_OKAY, selectedPath};
}

std::tuple<bool, std::vector<std::string>> open_multi_file_dialog(const std::vector<std::vector<std::string>>& filter_list,
                                                                  const std::string& default_path) {
  NFD::Guard nfdGuard;

  // auto-freeing memory
  NFD::UniquePathSet outPaths;

  // prepare filters for the dialog
  const size_t filterCount = filter_list.size();
  nfdfilteritem_t* filterItem = nullptr;

  if (filterCount > 0) {
    filterItem = (nfdfilteritem_t*)malloc(sizeof(nfdfilteritem_t) * filterCount);
    for (size_t i = 0; i < filterCount; ++i) {
      filterItem[i].name = strdup(filter_list[i][0].c_str());
      filterItem[i].spec = strdup(filter_list[i][1].c_str());
    }
  }

  // set the default path
  const nfdnchar_t* defaultPath = default_path.empty() ? nullptr : default_path.c_str();

  // show the dialog
  const nfdresult_t result = NFD::OpenDialogMultiple(outPaths,
                                                     filterItem,
                                                     filterCount,
                                                     defaultPath);

  // Clean up the filter item
  free(filterItem);

  std::vector<std::string> selectedPaths;

  if (result == NFD_OKAY) {
    nfdpathsetsize_t numPaths;
    NFD::PathSet::Count(outPaths, numPaths);
    selectedPaths.resize(numPaths);

    for (nfdpathsetsize_t i = 0; i < numPaths; ++i) {
      NFD::UniquePathSetPath path;
      NFD::PathSet::GetPath(outPaths, i, path);

      selectedPaths[i] = std::string(path.get());
    }
  }

  // NFD::Guard will automatically quit NFD.
  return {result == NFD_OKAY, selectedPaths};
}

PYBIND11_MODULE(_nfdpy, m) {
  m.def("open_file_dialog", &open_file_dialog, "A function that adds two numbers");
  m.def("open_multi_file_dialog", &open_multi_file_dialog, "A function that adds two numbers");
}
