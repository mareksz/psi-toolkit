#include "configuration_finder.hpp"

#include <boost/property_tree/xml_parser.hpp>

#include "from_configuration_annotator_factory.hpp"

ConfigurationFinder::ConfigurationFinder(
    boost::filesystem::path startDirectory,
    FactoriesKeeper& factoryKeeper)
    :startDirectory_(startDirectory), factoryKeeper_(factoryKeeper) {
}

void ConfigurationFinder::run() {
    INFO("ConfigurationFinder looking in " << startDirectory_.string());

    boost::filesystem::recursive_directory_iterator end_itr;
    for (boost::filesystem::recursive_directory_iterator fiter(startDirectory_);
         fiter != end_itr;
         ++fiter) {
        boost::filesystem::path filePath(
            fiter->path() / CONFIGURATION_FILE_NAME);

        if (boost::filesystem::exists(filePath))
            runOnSingleFile_(filePath);
    }
}

void ConfigurationFinder::runOnSingleFile_(boost::filesystem::path filePath) {
    INFO("ConfigurationFinder found " << filePath.string());

    boost::property_tree::ptree xmlParsed_;

    boost::property_tree::read_xml(
        filePath.string(),
        xmlParsed_,
        boost::property_tree::xml_parser::no_comments);

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                  xmlParsed_.get_child("configurations")) {
        if (v.first == "configuration") {
            processConfiguration_(v.second);
        }
    }
}

void ConfigurationFinder::processConfiguration_(boost::property_tree::ptree& configurationNode) {
    std::string name = configurationNode.get<std::string>("<xmlattr>.name");
    std::string annotator = configurationNode.get<std::string>("<xmlattr>.annotator");

    boost::optional<std::string> options =
        configurationNode.get_optional<std::string>("<xmlattr>.options");

    boost::optional<std::string> description =
        configurationNode.get_optional<std::string>("<xmlattr>.description");

    INFO("configuration: " << name);
    INFO("  annotator: " << annotator);

    FromConfigurationAnnotatorFactory* factory =
        new FromConfigurationAnnotatorFactory(
            name,
            dynamic_cast<AnnotatorFactory&>(factoryKeeper_.getProcessorFactory(annotator)));

    if (options) {
        INFO("  options: " << *options);
        factory->setOptions(*options);
    }

    if (description) {
        INFO("  description: " << *description);
        factory->setXmlDescription_(*description);
    }

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                  configurationNode.get_child("")) {
        if (v.first == "provided")
            factory->setProvidedLayerTags(getListOfTags_(v.second));
        else if (v.first == "required")
            factory->setRequiredLayerTags(
                getListOfListOfTags_(v.second));
        else if (v.first == "optional")
            factory->setOptionalLayerTags(
                getListOfListOfTags_(v.second));
    }

    factoryKeeper_.takeProcessorFactory(factory);
}

std::list<std::string> ConfigurationFinder::getListOfTags_(
    boost::property_tree::ptree& configurationNode) {

    std::list<std::string> listOfTags_;

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                  configurationNode.get_child("")) {
        if (v.first == "tag") {
            std::string tag = v.second.get<std::string>("");
            INFO("    tag:" << tag);
            listOfTags_.push_back(tag);
        }
    }

    return listOfTags_;
}

std::list<std::list<std::string> > ConfigurationFinder::getListOfListOfTags_(
    boost::property_tree::ptree& configurationNode) {

    std::list<std::list<std::string> > listOfListsOfTags_;

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                  configurationNode.get_child("")) {
        if (v.first == "tags") {
            listOfListsOfTags_.push_back(getListOfTags_(v.second));
        }
    }

    return listOfListsOfTags_;
}

const boost::filesystem::path ConfigurationFinder::CONFIGURATION_FILE_NAME =
                          boost::filesystem::path("configurations.xml");
