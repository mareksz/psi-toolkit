add_library(tools-normalizers-simplenorm SHARED
        simplenorm_normalizer.cpp
)

add_subdirectory(t)

install(TARGETS tools-normalizers-simplenorm DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/simplenorm-normalizer)
