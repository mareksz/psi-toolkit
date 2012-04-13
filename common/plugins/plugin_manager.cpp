#include "config.hpp"

#include "plugin_manager.hpp"

#if HAVE_ASPELL
#include "aspell_plugin.hpp"
#endif

PluginManager::PluginManager() {
#if HAVE_ASPELL
    registerPlugin_(new AspellPlugin());
#endif
}

PluginManager::~PluginManager() {
}

PluginManager& PluginManager::getInstance() {
    static PluginManager instance;
    return instance;
}

PluginAdapter * PluginManager::createPluginAdapter(const std::string & pluginName) {
    if (checkIsPluginActiveWithMessage_(pluginName)) {
        return getPlugin_(pluginName).createAdapter();
    } else {
        return NULL;
    }
}

void PluginManager::destroyPluginAdapter(const std::string & pluginName,
                                         PluginAdapter * adapterPointer) {
    if (checkIsPluginActive_(pluginName)) {
        AbstractPlugin & plugin = getPlugin_(pluginName);
        plugin.destroyAdapter(adapterPointer);
    }
}

bool PluginManager::checkPluginRequirements(const std::string & pluginName,
                             const boost::program_options::variables_map& options,
                                            std::ostream & message) {
    return getPlugin_(pluginName)
        .checkRequirements(options, message);
}


PluginManager::UnknownPluginException::UnknownPluginException(
    const std::string& pluginName)
    : PsiException(std::string("unknown plugin `") + pluginName + "`") {
}


void PluginManager::registerPlugin_(AbstractPlugin* pluginPointer) {
    pluginPointer->loadPlugin();

    if (! pluginPointer->isPluginActive()) {
        // @todo Maybe a message? (or maybe not!)
    }

    nameToPluginMap_[pluginPointer->getName()]
        = boost::shared_ptr<AbstractPlugin>(pluginPointer);
}

bool PluginManager::checkIsPluginActiveWithMessage_(const std::string & pluginName) {
    return checkIsPluginActive_(pluginName, true);
}

bool PluginManager::checkIsPluginActive_(const std::string & pluginName, bool withMessage) {
    AbstractPlugin & plugin = getPlugin_(pluginName);
    if (plugin.isPluginActive()) {
        return true;
    } else {
        if (withMessage) {
            INFO("Plugin: " << pluginName << " is not active.");
        }

        return false;
    }
}

AbstractPlugin & PluginManager::getPlugin_(const std::string & pluginName) {
    if (nameToPluginMap_.count(pluginName))
        return *nameToPluginMap_[pluginName];

    throw UnknownPluginException(pluginName);
}
