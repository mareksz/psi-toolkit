include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-converters-unumsunt-tests
    unumsunt_tests.cpp
)

target_link_libraries(tools-converters-unumsunt-tests tools-converters-unumsunt)
