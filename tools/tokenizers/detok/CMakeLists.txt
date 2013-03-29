add_library(tools-tokenizers-detok SHARED
	detok.cpp
)

target_link_libraries(
 tools-tokenizers-detok
 common
)

# add_subdirectory(t)

install(TARGETS tools-tokenizers-detok DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/detok)
