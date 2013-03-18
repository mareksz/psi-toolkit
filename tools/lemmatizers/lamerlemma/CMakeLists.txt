include_directories("${psi-toolkit_SOURCE_DIR}/tools/datastructures/psi_query")

add_library(tools-lemmatizers-lamerlemma SHARED
    lamerlemma.cpp
    dictionary.cpp
    utility_functions.cpp
)

target_link_libraries(tools-lemmatizers-lamerlemma tools-datastructures-psi_query common z boost_iostreams)

install(TARGETS tools-lemmatizers-lamerlemma DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/lamerlemma)
