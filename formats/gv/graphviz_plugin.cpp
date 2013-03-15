#include "graphviz_plugin.hpp"

#include <boost/assign.hpp>

#include "config.hpp"


std::string GraphvizPlugin::getLibraryName() {
    return "libpsitoolkit_formats-gv-writer-plugin.so";
}


std::string GraphvizPlugin::getName() {
    return "graphviz";
}


bool GraphvizPlugin::doCheckRequirementsWithOptionsDefaultOS(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of
                ("libgvc.so")
#if GRAPHVIZ_CGRAPH
                ("libcgraph.so")
#else
                ("libgraph.so")
#endif
                ("libcdt.so")
                ("libpathplan.so")
        );
        return false;
    }
    return true;
}


bool GraphvizPlugin::doCheckRequirementsWithOptionsUbuntu(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
            boost::assign::list_of
                ("libgvc5")
#if GRAPHVIZ_CGRAPH
                ("libcgraph5")
#else
                ("libgraph4")
#endif
                ("libcdt4")
                ("libpathplan4")
        );
        return false;
    }
    return true;
}


bool GraphvizPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}
