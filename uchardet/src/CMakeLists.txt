add_library(uchardet SHARED
	src/CharDistribution.cpp
	src/JpCntx.cpp
	src/LangBulgarianModel.cpp
	src/LangCyrillicModel.cpp
	src/LangGreekModel.cpp
	src/LangHungarianModel.cpp
	src/LangHebrewModel.cpp
	src/LangThaiModel.cpp
	src/nsHebrewProber.cpp
	src/nsCharSetProber.cpp
	src/nsBig5Prober.cpp
	src/nsEUCJPProber.cpp
	src/nsEUCKRProber.cpp
	src/nsEUCTWProber.cpp
	src/nsEscCharsetProber.cpp
	src/nsEscSM.cpp
	src/nsGB2312Prober.cpp
	src/nsMBCSGroupProber.cpp
	src/nsMBCSSM.cpp
	src/nsSBCSGroupProber.cpp
	src/nsSBCharSetProber.cpp
	src/nsSJISProber.cpp
	src/nsUTF8Prober.cpp
	src/nsLatin1Prober.cpp
	src/nsUniversalDetector.cpp
	src/uchardet.cpp
)

include_directories("${psi-toolkit_SOURCE_DIR}/uchardet/src")
set_target_properties(
	uchardet
	PROPERTIES
		LINKER_LANGUAGE
			CXX
		OUTPUT_NAME
			uchardet
		VERSION
			0.0.0
		SOVERSION
			0
)

install(TARGETS uchardet DESTINATION lib/)
