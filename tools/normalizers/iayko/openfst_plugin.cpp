#include "openfst_plugin.hpp"
#include <boost/assign.hpp>

std::string OpenFSTPlugin::getLibraryName()
{
    return "libpsitoolkit_tools-normalizers-openfst-plugin.so";
}

std::string OpenFSTPlugin::getName()
{
    return "openfst";
}

bool OpenFSTPlugin::doCheckRequirementsWithOptionsDefaultOS(
    const boost::program_options::variables_map& /* options */
)
{

    if (! isPluginActive())
    {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of
            ("libfst.so")
        );
        return false;
    }
    return true;
}

bool OpenFSTPlugin::doCheckRequirementsWithOptionsUbuntu(
    const boost::program_options::variables_map& /* options */)
{
    if (!isPluginActive())
    {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of
            ("libfst.so")
        );
        return false;
    }
    return true;
}

bool OpenFSTPlugin::areRequirementsDefinedForUbuntu()
{
    return true;
}

bool OpenFSTPlugin::doCheckRequirementsWithOptionsArchLinux(
    const boost::program_options::variables_map& /* options */)
{
    if (!isPluginActive())
    {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of
            ("libfst")
        );
        return false;
    }
    return true;
}

bool OpenFSTPlugin::areRequirementsDefinedForArchLinux()
{
    return true;
}
