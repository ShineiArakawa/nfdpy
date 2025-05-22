#include <pybind11/pybind11.h>

#include <iostream>
#include <nfd.hpp>
#include <string>

std::string open_file_dialog() {
  NFD::Guard nfdGuard;

  // auto-freeing memory
  NFD::UniquePath outPath;

  // prepare filters for the dialog
  nfdfilteritem_t filterItem[2] = {{"Source code", "c,cpp,cc"}, {"Headers", "h,hpp"}};

  // show the dialog
  nfdresult_t result = NFD::OpenDialog(outPath,
                                       filterItem, 2);

  if (result == NFD_OKAY) {
    std::cout << "Success!" << std::endl
              << outPath.get() << std::endl;
    return outPath.get();
  } else if (result == NFD_CANCEL) {
    std::cout << "User pressed cancel." << std::endl;
  } else {
    std::cout << "Error: " << NFD::GetError() << std::endl;
  }

  // NFD::Guard will automatically quit NFD.
  return "";
}

PYBIND11_MODULE(_nfdpy, m) {
  m.def("open_file_dialog", &open_file_dialog, "A function that adds two numbers");
}
