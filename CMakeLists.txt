cmake_minimum_required(VERSION 3.15...3.26)

project(${SKBUILD_PROJECT_NAME} LANGUAGES C CXX)

set(NFDPY_MODULE_NAME _${PROJECT_NAME})
message(STATUS "Module name: ${NFDPY_MODULE_NAME}")

# ----------------------------------------------------------------------
# Add 'nativefiledialog-extended' as a submodule
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/nativefiledialog-extended)

# ----------------------------------------------------------------------

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

pybind11_add_module(
    ${NFDPY_MODULE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nfdpy/binding.cpp
)

target_link_libraries(
    ${NFDPY_MODULE_NAME}
    PRIVATE
    nfd
)

install(TARGETS ${NFDPY_MODULE_NAME} LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})
