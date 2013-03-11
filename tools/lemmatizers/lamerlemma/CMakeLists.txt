include_directories("${psi-toolkit_SOURCE_DIR}/tools/datastructures/lexica")
include_directories("${psi-toolkit_SOURCE_DIR}/utf8case")

add_library(tools-lemmatizers-lamerlemma SHARED
    lamerlemma.cpp
    dictionary.cpp
)

target_link_libraries(tools-lemmatizers-lamerlemma common cmph)
install(TARGETS tools-lemmatizers-lamerlemma DESTINATION lib/)