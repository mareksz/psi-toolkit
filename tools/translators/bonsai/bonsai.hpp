#ifndef BONSAI_HDR
#define BONSAI_HDR

#include "config.hpp"
#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include <boost/program_options.hpp>
#include <fstream>

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "TransferTypes.hpp"
#include "Translator.hpp"

class Bonsai {
    public:
        Bonsai(poleng::bonsai::TranslatorPtr translator, const std::string& langCode);
        void translate(Lattice &lattice, Lattice::EdgeDescriptor edge);

    private:
        poleng::bonsai::TranslatorPtr translator_;
        std::string langCode_;
};

#endif
