get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-normalizers-iayko-tests
    iayko_tests.cpp
)

target_link_libraries(tools-normalizers-iayko-tests
    tools-normalizers-iayko
)

