add_subdirectory(t)

add_library(tools-translators-transferer SHARED
    numconversion.cpp
    transferer.cpp
    trules.cpp
    test_results_recorder.cpp)
