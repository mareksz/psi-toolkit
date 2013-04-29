add_library(formats-bracketing SHARED
    bracketing_lattice_writer.cpp
    bracketing_quoter.cpp
)

add_subdirectory(t)

install(TARGETS formats-bracketing DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/bracketing-lattice-writer)
