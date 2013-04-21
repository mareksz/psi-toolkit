add_executable(extract-rules
    ExtractedRule.cpp
    extract-rules-main.cpp
    HoleCollection.cpp
    InputFileStream.cpp
    OutputFileStream.cpp
    SentenceAlignment.cpp
    SentenceAlignmentWithSyntax.cpp
    SyntaxTree.cpp
    tables-core.cpp
    XmlTree.cpp
)

add_executable(score
    score-main.cpp
    domain.cpp
    ScoreFeature.cpp
    InputFileStream.cpp
    OutputFileStream.cpp
    PhraseAlignment.cpp
    util/exception.cc
    tables-core.cpp
)

add_executable(consolidate
    consolidate-main.cpp
    domain.cpp
    InputFileStream.cpp
    OutputFileStream.cpp
    util/exception.cc
    tables-core.cpp
)

add_executable(symal
    symal.cpp
    cmd.c
)

target_link_libraries(extract-rules boost_iostreams)
target_link_libraries(score boost_iostreams)
target_link_libraries(consolidate boost_iostreams)
