add_library(tools-converters-antiselector SHARED
    antiselector.cpp
)

target_link_libraries(tools-converters-antiselector)

install(TARGETS tools-converters-antiselector DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/antiselector)
