get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-normalizers-niema-tests
    niema_tests.cpp
)

target_link_libraries(tools-normalizers-niema-tests
    tools-normalizers-niema
)

