add_library(tools-converters-unumsunt SHARED
    unumsunt.cpp
)

add_subdirectory("t")

target_link_libraries(tools-converters-unumsunt)

install(TARGETS tools-converters-unumsunt DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/unumsunt)
