enable_language(C)

add_library(tiniconv SHARED
    tiniconv.c
    tiniconv_desc.c    
)

include_directories("${psi-toolkit_SOURCE_DIR}/tiniconv/encdec")
set_target_properties(tiniconv PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS tiniconv DESTINATION lib/)
