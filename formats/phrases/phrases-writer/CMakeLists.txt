add_library(formats-phrases SHARED
    phrases_lattice_writer.cpp
)

install(TARGETS formats-phrases DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/phrases-lattice-writer)
