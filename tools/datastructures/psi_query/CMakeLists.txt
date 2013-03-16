add_library(tools-datastructures-fsa SHARED
    fsa_multi_store.cpp
)

target_link_libraries(tools-datastructures-fsa)

add_subdirectory(t)

install(TARGETS tools-datastructures-fsa DESTINATION lib/)
