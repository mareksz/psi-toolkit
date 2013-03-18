add_definitions(-Wno-sign-compare -Wno-unused-function)

find_package(BISON)
find_package(FLEX)

add_subdirectory(t)

BISON_TARGET(trparser trparser.y ${CMAKE_CURRENT_BINARY_DIR}/trparser.tab.cpp COMPILE_FLAGS "-p trparser")
FLEX_TARGET(trlexer lex.trlex.lpp ${CMAKE_CURRENT_BINARY_DIR}/lex.trlex.cpp)
#ADD_FLEX_BISON_DEPENDENCY(trparser trlexer)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(tools-translators-transferer SHARED
    edge2zsyntree.cpp
    numconversion.cpp
    transferer.cpp
    transferer_runner.cpp
    trules.cpp
    test_results_recorder.cpp
    tscript_from_file.cpp
    ${BISON_trparser_OUTPUTS}
    ${FLEX_trlexer_OUTPUTS})

add_executable(trunner trunner.cpp)

include_directories("${psi-toolkit_SOURCE_DIR}/utf8case")

target_link_libraries(tools-translators-transferer common ${Boost_LIBRARIES} ${FLEX_LIBRARIES} utf8case)

target_link_libraries(trunner ${Boost_LIBRARIES} tools-translators-transferer)

install(TARGETS tools-translators-transferer DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/transferer)
