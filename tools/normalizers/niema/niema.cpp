#include "niema.hpp"

#include <climits>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string/predicate.hpp>

#include "config.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include "psi_exception.hpp"


const std::string Niema::Factory::DEFAULT_FAR_PATH
    = "%ITSDATA%/%LANG%/all.far";

const std::string Niema::Factory::DEFAULT_FSTS_PATH
    = "%ITSDATA%/%LANG%/rules.txt";

const std::string Niema::Factory::DEFAULT_EXCEPTIONS_PATH
    = "%ITSDATA%/%LANG%/exceptions.lst";

const std::list<std::string> Niema::requiredTags = boost::assign::list_of
        ("token");

const std::list<std::string> Niema::tagsToOperateOn = boost::assign::list_of
        ("token");

const std::list<std::string> Niema::providedTags = boost::assign::list_of
        ("niema")("normalization");

const std::list<std::string> Niema::tagsToPut = boost::assign::list_of
        ("niema")("normalization")("token")("conditional");


std::string Niema::Factory::getRealFileName(std::string fileSpec, std::string lang) const
{
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    return fileFetcher.getOneFile(fileSpec).string();
}


Annotator* Niema::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string lang = options["lang"].as<std::string>();
    std::string farFileSpec = options["far"].as<std::string>();
    std::string fst = options["fst"].as<std::string>();
    std::string condition = options["condition"].as<std::string>();
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

    Niema::Spec spec;
    if (options.count("spec")) {
        std::vector<std::string> optspec = options["spec"].as< std::vector<std::string> >();
        std::vector<std::string>::iterator si = optspec.begin();

        while (si != optspec.end()) {
            std::string far = getRealFileName(*si, lang);

            ++si;
            if (si == optspec.end()) {
                break;
            }
            std::string fst = *si;

            ++si;
            if (si == optspec.end()) {
                break;
            }
            std::string condition = *si;

            spec.push_back(std::make_pair(std::make_pair(far, fst), condition));
            ++si;
        }
    }

    if (!spec.empty()) {
        if (!fst.empty()) {
            throw PsiException("Options --fst and --spec must not be used together");
        }

        if (fstsFileSpec != DEFAULT_FSTS_PATH) {
            throw PsiException("Options --fsts and --spec must not be used together");
        }

        return new Niema(lang, spec, exceptions);
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
                spec.push_back(std::make_pair(std::make_pair(far, fstLine), condition));
            }
        }
        fin.close();

        return new Niema(lang, spec, exceptions);
    }

    spec.push_back(std::make_pair(std::make_pair(far, fst), condition));
    return new Niema(lang, spec, exceptions);
}


void Niema::Factory::doAddLanguageIndependentOptionsHandled(
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
    ("condition",
        boost::program_options::value<std::string>()
        ->default_value(std::string()),
        "condition for fst")
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


std::string Niema::Factory::doGetName() const
{
    return "niema";
}


std::string Niema::Factory::doGetSubType() const
{
    return "normalizer";
}


std::list<std::string> Niema::Factory::doGetAliases()
{
    return boost::assign::list_of<std::string>
        (std::string("niema-normalizer"))
        (std::string("fst-normalizer"))
        (std::string("thrax-normalizer"));
}


boost::filesystem::path Niema::Factory::doGetFile() const
{
    return __FILE__;
}


std::list<std::list<std::string> > Niema::Factory::doRequiredLayerTags()
{
    return boost::assign::list_of(Niema::requiredTags);
}


std::list<std::list<std::string> > Niema::Factory::doOptionalLayerTags()
{
    return std::list<std::list<std::string> >();
}


std::list<std::string> Niema::Factory::doProvidedLayerTags()
{
    return Niema::providedTags;
}


std::list<std::string> Niema::Factory::doAllLanguagesHandled() const {
    return boost::assign::list_of
        (std::string("pl"));
}


std::string Niema::Factory::doGetContinuation(
    const boost::program_options::variables_map& /* options */) const
{
    return "simple-writer --tags niema --sep \" \"";
}


bool Niema::Factory::doCheckRequirements(
    const boost::program_options::variables_map& options,
    std::ostream& message) const
{
    return PluginManager::getInstance().checkPluginRequirements("openfst",
            options,
            message);
}


LatticeWorker* Niema::doCreateLatticeWorker(Lattice& lattice)
{
    return new Worker(*this, lattice);
}


Niema::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor),
    outputTags_(lattice_.getLayerTagManager().createTagCollectionFromListWithLangCode(
                  Niema::tagsToPut, dynamic_cast<Niema&>(processor_).langCode_))
{
}


void Niema::Worker::doRun()
{
    Niema& niemaProcessor = dynamic_cast<Niema&>(processor_);

    if (niemaProcessor.isActive())
    {

        LayerTagMask tokenMask_ =
            lattice_.getLayerTagManager().getMaskWithLangCode(
                "token", niemaProcessor.langCode_);

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

            std::string text = lattice_.getAnnotationText(currentEdge);

            for (Spec::iterator si = niemaProcessor.spec_.begin();
                    si != niemaProcessor.spec_.end();
                    ++si) {
                std::string normalized_text;
                if (std::find(niemaProcessor.exceptions_.begin(),
                            niemaProcessor.exceptions_.end(),
                            text) != niemaProcessor.exceptions_.end()) {
                    normalized_text = text;
                } else {
                    normalized_text = fstNormalize_(
                            si->first.first,
                            si->first.second,
                            text);
                }

                AnnotationItem ai("T", normalized_text);
                if (normalized_text != text) {
                    lattice_.getAnnotationItemManager().setValue(
                            ai, "condition", si->second);
                }
                lattice_.addEdge(
                        source,
                        target,
                        ai,
                        outputTags_);
                text = normalized_text;
            }

        }
    }
}


std::string Niema::Worker::fstNormalize_(
        const std::string& far,
        const std::string& fst,
        const std::string& text)
{
    Niema& niemaProcessor = dynamic_cast<Niema&>(processor_);
    return niemaProcessor.getAdapter()->normalize(far, fst, text);
}


std::string Niema::doInfo()
{
    return "Niema";
}


Niema::Niema(const std::string& langCode,
             Niema::Spec spec,
             std::vector<std::string> exceptions)
{
    init_(langCode, spec, exceptions);

    if ( isActive() ) {
        openFSTAdapter_->init();
    }
}


Niema::~Niema()
{
    if (openFSTAdapter_)
    {
        PluginManager::getInstance().destroyPluginAdapter("openfst", openFSTAdapter_);
    }
}


void Niema::init_(const std::string& langCode,
                  Niema::Spec spec,
                  std::vector<std::string> exceptions)
{
    langCode_ = langCode;
    spec_ = spec;
    exceptions_ = exceptions;

    openFSTAdapter_ = dynamic_cast<OpenFSTAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("openfst"));
}


OpenFSTAdapterInterface* Niema::getAdapter()
{
    return openFSTAdapter_;
}


bool Niema::isActive()
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
