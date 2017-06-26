
get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-normalizers-simplenorm-tests
        simplenorm_normalizer_tests.cpp
)

target_link_libraries(tools-normalizers-simplenorm-tests
        tools-normalizers-simplenorm
)

