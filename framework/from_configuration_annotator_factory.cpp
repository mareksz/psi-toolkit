#include "from_configuration_annotator_factory.hpp"

FromConfigurationAnnotatorFactory::FromConfigurationAnnotatorFactory(
    const std::string& name,
    AnnotatorFactory& annotatorFactory)
    :name_(name), annotatorFactory_(annotatorFactory) {
}

boost::program_options::options_description
FromConfigurationAnnotatorFactory::doOptionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    return desc;
}

boost::filesystem::path
FromConfigurationAnnotatorFactory::doGetFile() const {
    return annotatorFactory_.getFile();
}

std::string
FromConfigurationAnnotatorFactory::doGetName() const {
    return name_;
}

Annotator*
FromConfigurationAnnotatorFactory::doCreateAnnotator(
    const boost::program_options::variables_map&) {

    return annotatorFactory_.createAnnotator(options_);
}

std::list<std::list<std::basic_string<char> > >
FromConfigurationAnnotatorFactory::doRequiredLayerTags() {
    return requiredLayerTags_.get_value_or(annotatorFactory_.requiredLayerTags());

}
std::list<std::list<std::basic_string<char> > >
FromConfigurationAnnotatorFactory::doOptionalLayerTags() {
    return optionalLayerTags_.get_value_or(annotatorFactory_.optionalLayerTags());
}

std::list<std::basic_string<char> >
FromConfigurationAnnotatorFactory::doProvidedLayerTags() {
    return providedLayerTags_.get_value_or(annotatorFactory_.providedLayerTags());
}

AnnotatorFactory::LanguagesHandling
FromConfigurationAnnotatorFactory::doLanguagesHandling(
    const boost::program_options::variables_map&) const {

    return annotatorFactory_.languagesHandling(options_);
}

std::list<std::basic_string<char> >
FromConfigurationAnnotatorFactory::doLanguagesHandled(const boost::program_options::variables_map&) const {

    return annotatorFactory_.languagesHandled(options_);
}
