add_library(tools-converters-selector SHARED
    selector.cpp
)

target_link_libraries(tools-converters-selector)

install(TARGETS tools-converters-selector DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/selector)
