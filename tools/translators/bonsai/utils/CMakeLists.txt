include_directories(binaries)
include_directories(../rules)
add_subdirectory(binaries)

file(COPY scripts DESTINATION ${psi-toolkit_BINARY_DIR}/tools/translators/bonsai/utils)
