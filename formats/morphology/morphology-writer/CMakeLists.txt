add_library(formats-morphology SHARED
    morphology_lattice_writer.cpp
)

install(TARGETS formats-morphology DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/morphology-lattice-writer)
