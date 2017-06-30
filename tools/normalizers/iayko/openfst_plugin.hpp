#ifndef OPENFST_PLUGIN_HDR
#define OPENFST_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/openfst_adapter_interface.hpp"

class OpenFSTPlugin : public Plugin<OpenFSTAdapterInterface>
{
public:
    virtual std::string getLibraryName();
    virtual std::string getName();

private:
    virtual bool doCheckRequirementsWithOptionsDefaultOS(
        const boost::program_options::variables_map& options
    );

    virtual bool doCheckRequirementsWithOptionsUbuntu(
        const boost::program_options::variables_map& options);
    virtual bool areRequirementsDefinedForUbuntu();

    virtual bool doCheckRequirementsWithOptionsArchLinux(
        const boost::program_options::variables_map& options);
    virtual bool areRequirementsDefinedForArchLinux();
};

#endif
