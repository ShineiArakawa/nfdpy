#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <codecvt>
#include <cwchar>
#include <iostream>
#include <locale>
#include <nfd.hpp>
#include <string>

std::tuple<bool, std::string> open_file_dialog(const std::vector<std::vector<std::string>>& filter_list,
                                               const std::string& default_path) {
  NFD::Guard nfdGuard;

#if defined(_WIN32)
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
#endif

  // auto-freeing memory
  NFD::UniquePathN outPath;

  // prepare filters for the dialog
  const size_t filterCount = filter_list.size();
  nfdnfilteritem_t* filterItem = nullptr;

  if (filterCount > 0) {
    filterItem = (nfdnfilteritem_t*)malloc(sizeof(nfdnfilteritem_t) * filterCount);
    for (size_t i = 0; i < filterCount; ++i) {
#if defined(_WIN32)
      std::wstring name = converter.from_bytes(filter_list[i][0]);
      std::wstring spec = converter.from_bytes(filter_list[i][1]);

      filterItem[i].name = (const nfdnchar_t*)malloc(sizeof(nfdnchar_t) * (name.length() + 1));
      filterItem[i].spec = (const nfdnchar_t*)malloc(sizeof(nfdnchar_t) * (spec.length() + 1));
      std::wcscpy((wchar_t*)filterItem[i].name, name.c_str());
      std::wcscpy((wchar_t*)filterItem[i].spec, spec.c_str());
#else
      filterItem[i].name = strdup(filter_list[i][0].c_str());
      filterItem[i].spec = strdup(filter_list[i][1].c_str());
#endif
    }
  }

  // set the default path
#if defined(_WIN32)
  std::wstring defaultPath = converter.from_bytes(default_path);
  const nfdnchar_t* defaultPathPtr = default_path.empty() ? nullptr : (const nfdnchar_t*)malloc(sizeof(nfdnchar_t) * (defaultPath.length() + 1));
  std::wcscpy((wchar_t*)defaultPathPtr, defaultPath.c_str());
#else
  const nfdnchar_t* defaultPathPtr = default_path.empty() ? nullptr : default_path.c_str();
#endif

  // show the dialog
  const nfdresult_t result = NFD::OpenDialog(outPath,
                                             filterItem,
                                             filterCount,
                                             defaultPathPtr);

  // Clean up the filter item
  free(filterItem);

  std::string selectedPath;
  if (result == NFD_OKAY) {
#if defined(_WIN32)
    nfdnchar_t* outPathPtr = (nfdnchar_t*)outPath.get();
    std::wstring outPathW(outPathPtr);
    selectedPath = converter.to_bytes(outPathW);
#else
    selectedPath = std::string(outPath.get());
#endif
  }

  // NFD::Guard will automatically quit NFD.
  return {result == NFD_OKAY, selectedPath};
}

std::tuple<bool, std::vector<std::string>> open_multi_file_dialog(const std::vector<std::vector<std::string>>& filter_list,
                                                                  const std::string& default_path) {
  NFD::Guard nfdGuard;

#if defined(_WIN32)
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
#endif

  // auto-freeing memory
  NFD::UniquePathSet outPaths;

  // prepare filters for the dialog
  const size_t filterCount = filter_list.size();
  nfdnfilteritem_t* filterItem = nullptr;

  if (filterCount > 0) {
    filterItem = (nfdnfilteritem_t*)malloc(sizeof(nfdnfilteritem_t) * filterCount);
    for (size_t i = 0; i < filterCount; ++i) {
#if defined(_WIN32)
      std::wstring name = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(filter_list[i][0]);
      std::wstring spec = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(filter_list[i][1]);

      filterItem[i].name = (const nfdnchar_t*)malloc(sizeof(nfdnchar_t) * (name.length() + 1));
      filterItem[i].spec = (const nfdnchar_t*)malloc(sizeof(nfdnchar_t) * (spec.length() + 1));
      std::wcscpy((wchar_t*)filterItem[i].name, name.c_str());
      std::wcscpy((wchar_t*)filterItem[i].spec, spec.c_str());
#else
      filterItem[i].name = strdup(filter_list[i][0].c_str());
      filterItem[i].spec = strdup(filter_list[i][1].c_str());
#endif
    }
  }

// set the default path
#if defined(_WIN32)
  std::wstring defaultPath = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(default_path);
  const nfdnchar_t* defaultPathPtr = default_path.empty() ? nullptr : (const nfdnchar_t*)malloc(sizeof(nfdnchar_t) * (defaultPath.length() + 1));
  std::wcscpy((wchar_t*)defaultPathPtr, defaultPath.c_str());
#else
  const nfdnchar_t* defaultPathPtr = default_path.empty() ? nullptr : default_path.c_str();
#endif

  // show the dialog
  const nfdresult_t result = NFD::OpenDialogMultiple(outPaths,
                                                     filterItem,
                                                     filterCount,
                                                     defaultPathPtr);

  // Clean up the filter item
  free(filterItem);

  std::vector<std::string> selectedPaths;

  if (result == NFD_OKAY) {
    nfdpathsetsize_t numPaths;
    NFD::PathSet::Count(outPaths, numPaths);
    selectedPaths.resize(numPaths);

    for (nfdpathsetsize_t i = 0; i < numPaths; ++i) {
      NFD::UniquePathSetPathN path;
      NFD::PathSet::GetPath(outPaths, i, path);

#if defined(_WIN32)
      nfdnchar_t* outPathPtr = (nfdnchar_t*)path.get();
      std::wstring outPathW(outPathPtr);
      selectedPaths[i] = converter.to_bytes(outPathW);
#else
      selectedPaths[i] = std::string(path.get());
#endif
    }
  }

  // NFD::Guard will automatically quit NFD.
  return {result == NFD_OKAY, selectedPaths};
}

PYBIND11_MODULE(_nfdpy, m) {
  m.def("open_file_dialog", &open_file_dialog, "A function that adds two numbers");
  m.def("open_multi_file_dialog", &open_multi_file_dialog, "A function that adds two numbers");
}
