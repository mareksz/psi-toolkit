#include "annotator_factory.hpp"

Annotator* AnnotatorFactory::createAnnotator(
    const boost::program_options::variables_map& options) {
    return dynamic_cast<Annotator*>(createProcessor(options));
}

Annotator* AnnotatorFactory::doCreateProcessor(
    const boost::program_options::variables_map& options) {
    return doCreateAnnotator(options);
}

std::list<std::list<std::string> > AnnotatorFactory::requiredLayerTags() {
    return doRequiredLayerTags();
}

std::list<std::list<std::string> > AnnotatorFactory::requiredLayerTags(
    const boost::program_options::variables_map& options) {

    return doRequiredLayerTags(options);
}


std::list<std::list<std::string> > AnnotatorFactory::optionalLayerTags() {
    return doOptionalLayerTags();
}

std::list<std::list<std::string> > AnnotatorFactory::optionalLayerTags(
    const boost::program_options::variables_map& options) {

    return doOptionalLayerTags(options);
}


std::list<std::string> AnnotatorFactory::providedLayerTags() {
    return doProvidedLayerTags();
}

std::list<std::string> AnnotatorFactory::providedLayerTags(
    const boost::program_options::variables_map& options) {

    return doProvidedLayerTags(options);
}


std::string AnnotatorFactory::doGetContinuation(
    const boost::program_options::variables_map& /*options*/) const {
    return "psi-writer";
}

AnnotatorFactory::LanguagesHandling AnnotatorFactory::languagesHandling(
    const boost::program_options::variables_map& options) const {
    return doLanguagesHandling(options);
}

std::list<std::string> AnnotatorFactory::languagesHandled(
    const boost::program_options::variables_map& options) const {
    return doLanguagesHandled(options);
}

std::list<std::list<std::string> > AnnotatorFactory::doRequiredLayerTags(
    const boost::program_options::variables_map& /*options*/) {

    return doRequiredLayerTags();
}

std::list<std::list<std::string> > AnnotatorFactory::doOptionalLayerTags(
    const boost::program_options::variables_map& /*options*/) {

    return doOptionalLayerTags();
}

std::list<std::string> AnnotatorFactory::doProvidedLayerTags(
    const boost::program_options::variables_map& /*options*/) {

    return doProvidedLayerTags();
}

std::string AnnotatorFactory::doGetSubType() const {
    return "uncategorized";
}
