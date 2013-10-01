add_library(tools-taggers-inflector SHARED
    inflector.cpp
)
target_link_libraries(tools-taggers-inflector common maxent)

#add_subdirectory(t)

install(TARGETS tools-taggers-inflector DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/inflector)
