#pragma once

#include "annotator_factory.hpp"

class FromConfigurationAnnotatorFactory: public AnnotatorFactory {

public:
    FromConfigurationAnnotatorFactory(
        const std::string& name,
        AnnotatorFactory& annotatorFactory);

    void setOptions(const std::string& optionsAsString);
    void setContinuation(const std::string& continuation);
    void setQualityScore(double qualityScore);
    void setEstimatedTime(double estimatedTime);
    void setRequiredLayerTags(std::list<std::list<std::string> > requiredLayerTags);
    void setOptionalLayerTags(std::list<std::list<std::string> > optionalLayerTags);
    void setProvidedLayerTags(std::list<std::string> providedLayerTags);

private:
    virtual boost::program_options::options_description doOptionsHandled();
    virtual boost::filesystem::path doGetFile() const;
    virtual std::string doGetName() const;
    virtual Annotator* doCreateAnnotator(const boost::program_options::variables_map&);
    virtual std::list<std::list<std::basic_string<char> > > doRequiredLayerTags();
    virtual std::list<std::list<std::basic_string<char> > > doOptionalLayerTags();
    virtual std::list<std::basic_string<char> > doProvidedLayerTags();
    virtual LanguagesHandling doLanguagesHandling(const boost::program_options::variables_map&) const;
    virtual std::list<std::basic_string<char> > doLanguagesHandled(const boost::program_options::variables_map&) const;

    std::string name_;
    AnnotatorFactory& annotatorFactory_;

    std::string optionsAsString_;
    boost::program_options::variables_map options_;

    boost::optional<std::string> continuation_;
    boost::optional<double> qualityScore_;
    boost::optional<double> estimatedTime_;
    boost::optional<std::list<std::list<std::string> > > requiredLayerTags_;
    boost::optional<std::list<std::list<std::string> > > optionalLayerTags_;
    boost::optional<std::list<std::string> > providedLayerTags_;

};
