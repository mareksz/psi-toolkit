#include "iayko.hpp"

#include <climits>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string/predicate.hpp>

#include "config.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include "psi_exception.hpp"


const std::string Iayko::Factory::DEFAULT_FAR_PATH
    = "%ITSDATA%/%LANG%/all.far";

const std::string Iayko::Factory::DEFAULT_FSTS_PATH
    = "%ITSDATA%/%LANG%/rules.txt";

const std::string Iayko::Factory::DEFAULT_EXCEPTIONS_PATH
    = "%ITSDATA%/%LANG%/exceptions.lst";

const std::list<std::string> Iayko::requiredTags = boost::assign::list_of
        ("lemma");

const std::list<std::string> Iayko::tagsToOperateOn = boost::assign::list_of
        ("token");

const std::list<std::string> Iayko::tagsToPut = boost::assign::list_of
        ("iayko")("normalization");


std::string Iayko::Factory::getRealFileName(std::string fileSpec, std::string lang) const
{
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    return fileFetcher.getOneFile(fileSpec).string();
}


Annotator* Iayko::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string lang = options["lang"].as<std::string>();
    std::string farFileSpec = options["far"].as<std::string>();
    std::string fst = options["fst"].as<std::string>();
    std::string fstsFileSpec = options["fsts"].as<std::string>();
    std::string grm = options["grm"].as<std::string>();
    std::string saveFar= options["save-far"].as<std::string>();
    std::string exceptionsFileSpec = options["exceptions"].as<std::string>();

    std::vector<std::string> exceptions;
    if (options.count("bypass-exceptions")) {
        std::string exceptionsFile = getRealFileName(exceptionsFileSpec, lang);

        std::ifstream fin(exceptionsFile.c_str());
        if (!fin.is_open()) {
            throw FileFormatException(
                    std::string("Cannot open file ") + exceptionsFile);
        }
        std::string exception;
        while (std::getline(fin, exception)) {
            if (!exception.empty()) {
                exceptions.push_back(exception);
            }
        }
        fin.close();
    }

    if (!fst.empty() && fstsFileSpec != DEFAULT_FSTS_PATH) {
        throw PsiException("Options --fst and --fsts must not be used together");
    }

    if (farFileSpec != DEFAULT_FAR_PATH && !grm.empty()) {
        throw PsiException("Options --far and --grm must not be used together");
    }

    std::vector< std::pair<std::string, std::string> > transducersSpec;
    if (options.count("spec")) {
        std::vector<std::string> spec = options["spec"].as< std::vector<std::string> >();
        std::vector<std::string>::iterator si = spec.begin();
        while (si != spec.end()) {
            std::string far = getRealFileName(*si, lang);
            ++si;
            if (si == spec.end()) {
                break;
            }
            std::string fst = *si;
            transducersSpec.push_back(std::make_pair(far, fst));
            ++si;
        }
    }

    if (!transducersSpec.empty()) {
        if (!fst.empty()) {
            throw PsiException("Options --fst and --spec must not be used together");
        }

        if (fstsFileSpec != DEFAULT_FSTS_PATH) {
            throw PsiException("Options --fsts and --spec must not be used together");
        }

        return new Iayko(lang, transducersSpec, exceptions);
    }

    std::string far = getRealFileName(farFileSpec, lang);
    std::string fsts = getRealFileName(fstsFileSpec, lang);

    if (!grm.empty()) {
        if (saveFar.empty()) {
            static char tmpFileNameTemplate[] = "far_XXXXXX";
            char tmpFileName[PATH_MAX];
            strcpy(tmpFileName, tmpFileNameTemplate);
            mkstemp(tmpFileName);
            saveFar = tmpFileName;
        }

        std::stringstream commandSs;
        commandSs << "thraxcompiler --input_grammar=" << grm
            << " --output_far=" << saveFar << " 1>&2";
        system(commandSs.str().c_str());

        far = saveFar;
    }

    if (fst.empty()) {
        std::ifstream fin(fsts.c_str());
        if (!fin.is_open()) {
            throw FileFormatException(std::string("Cannot open file ") + fsts);
        }
        std::string fstLine;
        while (std::getline(fin, fstLine)) {
            if (!fstLine.empty()) {
                transducersSpec.push_back(std::make_pair(far, fstLine));
            }
        }
        fin.close();

        return new Iayko(lang, transducersSpec, exceptions);
    }

    return new Iayko(lang, far, fst, exceptions);
}


void Iayko::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription)
{
    optionsDescription.add_options()
    ("far",
        boost::program_options::value<std::string>()
        ->default_value(DEFAULT_FAR_PATH),
        "far archive with rules")
    ("fst",
        boost::program_options::value<std::string>()
        ->default_value(std::string()),
        "fst name inside far")
    ("fsts",
        boost::program_options::value<std::string>()
        ->default_value(DEFAULT_FSTS_PATH),
        "file with fst names to be used as a cascade")
    ("spec",
        boost::program_options::value< std::vector<std::string> >()->multitoken(),
        "specification of more far:fst pairs to be used as cascade")
    ("grm",
        boost::program_options::value<std::string>()
        ->default_value(std::string()),
        "text file with rules written in Thrax")
    ("save-far",
        boost::program_options::value<std::string>()
        ->default_value(std::string()),
        "where to save the far archive compiled from grm file")
    ("bypass-exceptions",
        "bypass exceptions")
    ("exceptions",
        boost::program_options::value<std::string>()
        ->default_value(DEFAULT_EXCEPTIONS_PATH),
        "a text file with list of exceptions")
    ;
}


std::string Iayko::Factory::doGetName() const
{
    return "iayko";
}


std::string Iayko::Factory::doGetSubType() const
{
    return "normalizer";
}


std::list<std::string> Iayko::Factory::doGetAliases()
{
    return boost::assign::list_of<std::string>
        (std::string("iayko-normalizer"))
        (std::string("fst-normalizer"))
        (std::string("thrax-normalizer"));
}


boost::filesystem::path Iayko::Factory::doGetFile() const
{
    return __FILE__;
}


std::list<std::list<std::string> > Iayko::Factory::doRequiredLayerTags()
{
    return boost::assign::list_of(Iayko::requiredTags);
}


std::list<std::list<std::string> > Iayko::Factory::doOptionalLayerTags()
{
    return std::list<std::list<std::string> >();
}


std::list<std::string> Iayko::Factory::doProvidedLayerTags()
{
    return Iayko::tagsToPut;
}


std::list<std::string> Iayko::Factory::doAllLanguagesHandled() const {
    return boost::assign::list_of
        (std::string("pl"));
}


std::string Iayko::Factory::doGetContinuation(
    const boost::program_options::variables_map& /* options */) const
{
    return "simple-writer --tags iayko --sep \" \"";
}


bool Iayko::Factory::doCheckRequirements(
    const boost::program_options::variables_map& options,
    std::ostream& message) const
{
    return PluginManager::getInstance().checkPluginRequirements("openfst",
            options,
            message);
}


LatticeWorker* Iayko::doCreateLatticeWorker(Lattice& lattice)
{
    return new Worker(*this, lattice);
}


Iayko::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromListWithLangCode(
                  Iayko::tagsToPut, dynamic_cast<Iayko&>(processor_).langCode_))
{
}


void Iayko::Worker::doRun()
{
    Iayko& iaykoProcessor = dynamic_cast<Iayko&>(processor_);

    if (iaykoProcessor.isActive())
    {

        LayerTagMask tokenMask_ =
            lattice_.getLayerTagManager().getMaskWithLangCode(
                "token", iaykoProcessor.langCode_);

        Lattice::EdgesSortedByTargetIterator edgeIterator
            = lattice_.edgesSortedByTarget(tokenMask_);

        Lattice::EdgeDescriptor lastTokenEdge;
        Lattice::EdgeDescriptor lastSeparatingEdge;

        while (edgeIterator.hasNext())
        {
            Lattice::EdgeDescriptor currentEdge = edgeIterator.next();
            std::string category = lattice_.getAnnotationCategory(currentEdge);

            Lattice::VertexDescriptor source = lattice_.getEdgeSource(currentEdge);
            Lattice::VertexDescriptor target = lattice_.getEdgeTarget(currentEdge);

            std::string text = specialNormalize_(
                    iaykoProcessor.langCode_,
                    currentEdge);

            std::string normalized_text;
            if (std::find(iaykoProcessor.exceptions_.begin(),
                        iaykoProcessor.exceptions_.end(),
                        text) != iaykoProcessor.exceptions_.end()) {
                normalized_text = text;
            } else {
                normalized_text = fstNormalize_(text);
            }

            AnnotationItem ai("T", normalized_text);
            lattice_.addEdge(
                    source,
                    target,
                    ai,
                    textTags_);

        }
    }
}


std::string Iayko::Worker::fstNormalize_(const std::string& text)
{
    Iayko& iaykoProcessor = dynamic_cast<Iayko&>(processor_);
    return iaykoProcessor.getAdapter()->normalize(text);
}


std::string Iayko::Worker::specialNormalize_(
        const std::string& langCode,
        Lattice::EdgeDescriptor currentEdge)
{
    LayerTagMask lemmaMask_ =
        lattice_.getLayerTagManager().getMaskWithLangCode("lemma", langCode);

    Lattice::VertexDescriptor source = lattice_.getEdgeSource(currentEdge);
    Lattice::VertexDescriptor target = lattice_.getEdgeTarget(currentEdge);
    std::string text = lattice_.getAnnotationText(currentEdge);

    if (langCode == "pl") {
        if (boost::starts_with(text, "nie")) {

            bool currentTokenIsLemmatized = false;
            Lattice::InOutEdgesIterator oe = lattice_.outEdges(source, lemmaMask_);
            while (oe.hasNext()) {
                if (lattice_.getEdgeTarget(oe.next()) == target) {
                    currentTokenIsLemmatized = true;
                    break;
                }
            }

            if (!currentTokenIsLemmatized) {
                return std::string("nie ") + text.substr(3);
            }
        }
    }
    return text;
}


std::string Iayko::doInfo()
{
    return "Iayko";
}


Iayko::Iayko(const std::string& langCode,
             const std::string& far,
             const std::string& fst,
             std::vector<std::string> exceptions)
{
    init_(langCode, exceptions);

    if ( isActive() ) {
        openFSTAdapter_->init(far, fst);
    }
}


Iayko::Iayko(const std::string& langCode,
             std::vector< std::pair<std::string, std::string> > spec,
             std::vector<std::string> exceptions)
{
    init_(langCode, exceptions);

    if ( isActive() ) {
        openFSTAdapter_->init(spec);
    }
}


Iayko::~Iayko()
{
    if (openFSTAdapter_)
    {
        PluginManager::getInstance().destroyPluginAdapter("openfst", openFSTAdapter_);
    }
}


void Iayko::init_(const std::string& langCode,
                  std::vector<std::string> exceptions)
{
    langCode_ = langCode;
    exceptions_ = exceptions;

    openFSTAdapter_ = dynamic_cast<OpenFSTAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("openfst"));
}


OpenFSTAdapterInterface* Iayko::getAdapter()
{
    return openFSTAdapter_;
}


bool Iayko::isActive()
{
    if (openFSTAdapter_)
    {
        return true;
    }
    else
    {
        return false;
    }
}
