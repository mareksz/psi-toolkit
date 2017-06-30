add_library(tools-normalizers-openfst-plugin SHARED
	openfst_adapter_impl.cpp
)

target_link_libraries(tools-normalizers-openfst-plugin
    ${OPENFST_LIBRARIES}
)

install(TARGETS tools-normalizers-openfst-plugin DESTINATION lib/)

