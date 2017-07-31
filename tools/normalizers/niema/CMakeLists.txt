include_directories("${psi-toolkit_SOURCE_DIR}/tools/normalizers/niema")

add_library(tools-normalizers-niema SHARED
    niema.cpp
)

target_link_libraries(tools-normalizers-niema
    common
)

add_subdirectory(t)

install(TARGETS tools-normalizers-niema DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/niema)

