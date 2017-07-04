add_library(tools-normalizers-openfst-plugin SHARED
	openfst_adapter_impl.cpp
	fst_path_finder.cpp
)

target_link_libraries(tools-normalizers-openfst-plugin
    ${OPENFST_LIBRARIES}
    dl
)

install(TARGETS tools-normalizers-openfst-plugin DESTINATION lib/)

