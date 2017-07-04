#include "iayko.hpp"

#include "config.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include "logging.hpp"


const std::string Iayko::Factory::DEFAULT_FAR_PATH
    = "%ITSDATA%/%LANG%/all.far";

const std::string Iayko::Factory::DEFAULT_FST_NAME
    = "MAIN";

const std::list<std::string> Iayko::tagsToOperateOn = boost::assign::list_of("token");

const std::list<std::string> Iayko::tagsToPut = boost::assign::list_of
        ("token")("iayko")("normalization");


Annotator* Iayko::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string lang = options["lang"].as<std::string>();

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    std::string farFileSpec = options["far"].as<std::string>();
    std::string far = fileFetcher.getOneFile(farFileSpec).string();

    std::string fst = options["fst"].as<std::string>();

    return new Iayko(lang, far, fst);
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
         ->default_value(DEFAULT_FST_NAME),
         "fst name inside far")
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
    return boost::assign::list_of(Iayko::tagsToOperateOn);
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
    return "simple-writer --tags normalization";
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
                Iayko::tagsToOperateOn, iaykoProcessor.langCode_);

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
            std::string normalized_text = fstNormalize_(text);

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


std::string Iayko::doInfo()
{
    return "Iayko";
}


Iayko::Iayko(const std::string& langCode,
             const std::string& far,
             const std::string& fst)
{
    init_(langCode);

    if ( isActive() ) {
        openFSTAdapter_->init(far, fst);
    }
}


Iayko::~Iayko()
{
    if (openFSTAdapter_)
    {
        PluginManager::getInstance().destroyPluginAdapter("openfst", openFSTAdapter_);
    }
}


void Iayko::init_(const std::string& langCode)
{
    langCode_ = langCode;

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
