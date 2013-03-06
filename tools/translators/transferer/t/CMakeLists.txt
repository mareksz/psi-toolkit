get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

# commented for faster compilation
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-translators-transferer-tests
	transferer_tests.cpp
)

target_link_libraries(tools-translators-transferer-tests tools-translators-transferer common-tests)
