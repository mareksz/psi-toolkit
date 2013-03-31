add_library(tools-datastructures-psi_query SHARED
    fsa_multi_store.cpp
    fsa_lexicon_base.cpp
)

target_link_libraries(tools-datastructures-psi_query z boost_iostreams)

add_subdirectory(t)

install(TARGETS tools-datastructures-psi_query DESTINATION lib/)
