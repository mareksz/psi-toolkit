include_directories("${psi-toolkit_SOURCE_DIR}/tools/normalizers/iayko")

add_library(tools-normalizers-iayko SHARED
    iayko.cpp
    openfst_plugin.cpp
)

target_link_libraries(tools-normalizers-iayko
    common
)

add_subdirectory(t)
add_subdirectory(plugin)

install(TARGETS tools-normalizers-iayko DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/iayko)

