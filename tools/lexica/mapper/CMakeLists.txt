include_directories("${psi-toolkit_SOURCE_DIR}/tools/datastructures/lexica")

add_library(tools-lexica-mapper SHARED
    mapper.cpp
)

target_link_libraries(tools-lexica-mapper tools-datastructures-lexica)

add_subdirectory(t)

install(TARGETS tools-lexica-mapper DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/mapper)
