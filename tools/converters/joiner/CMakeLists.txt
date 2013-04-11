add_library(tools-converters-joiner SHARED
    joiner.cpp
)

#add_subdirectory("t")

target_link_libraries(tools-converters-joiner)

install(TARGETS tools-converters-joiner DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/joiner)
