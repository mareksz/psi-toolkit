#include "from_configuration_annotator_factory.hpp"

#include "split_pipeline.hpp"

#include <boost/program_options/parsers.hpp>
#include <boost/scoped_array.hpp>

FromConfigurationAnnotatorFactory::FromConfigurationAnnotatorFactory(
    const std::string& name,
    AnnotatorFactory& annotatorFactory)
    :name_(name), annotatorFactory_(annotatorFactory) {
}

void FromConfigurationAnnotatorFactory::setOptions(const std::string& optionsAsString) {

    std::vector<std::string> processorArgs = splitPipeline(optionsAsString);

    int argc = processorArgs.size() + 1;
    boost::scoped_array<char*> argv(new char* [argc + 1]);

    size_t i = 1;
    for (std::vector<std::string>::const_iterator iter = processorArgs.begin();
         iter != processorArgs.end();
         ++iter, ++i)
        argv[i] = const_cast<char*>((*iter).c_str()); //trick necessary because of Boost 1.42 bug
    argv[argc] = 0;

    boost::program_options::variables_map options;
    boost::program_options::store(
        boost::program_options::parse_command_line(
            argc, argv.get(),
            annotatorFactory_.optionsHandled()), options);
    boost::program_options::notify(options);

    options_ = options;
}

void FromConfigurationAnnotatorFactory::setRequiredLayerTags(std::list<std::list<std::string> > requiredLayerTags) {
    requiredLayerTags_ = requiredLayerTags;
}

void FromConfigurationAnnotatorFactory::setOptionalLayerTags(std::list<std::list<std::string> > optionalLayerTags) {
    optionalLayerTags_ = optionalLayerTags;
}

void FromConfigurationAnnotatorFactory::setProvidedLayerTags(std::list<std::string> providedLayerTags) {
    providedLayerTags_ = providedLayerTags;
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
