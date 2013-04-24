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

add_executable(binfsa
    binfsa.cpp
    ../../rules/CompressedDAG.cpp
    ../../rules/SimpleDAG.cpp
)

add_executable(minfsa
    minfsa.cpp
    Register.cpp
    IncrementalFSA.cpp
)

add_executable(txt2stringvector
    txt2stringvector.cpp
    ../../rules/ThrowingFwrite.cpp
)

add_executable(txt2monotonic
    txt2monotonic.cpp
    ../../rules/ThrowingFwrite.cpp
)


target_link_libraries(extract-rules boost_iostreams)
target_link_libraries(score boost_iostreams)
target_link_libraries(consolidate boost_iostreams)
target_link_libraries(minfsa boost_regex boost_serialization boost_program_options)
target_link_libraries(binfsa boost_regex boost_program_options)
