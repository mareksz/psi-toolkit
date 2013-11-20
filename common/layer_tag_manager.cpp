#include "layer_tag_manager.hpp"


#include <boost/algorithm/string.hpp>


LayerTagManager::LayerTagManager() : symbolTag_(createSingletonTagCollection("symbol")) { }

LayerTagCollection LayerTagManager::createSingletonTagCollection(std::string tagName) {
    LayerTagCollection result = LayerTagCollection();
    m_.insert(StringBimapItem(tagName, m_.size()));
    size_t tagIndex = m_.left.at(tagName);
    if (tagIndex >= result.v_.size()) {
        result.resize_(tagIndex + 1);
    }
    result.v_.set(tagIndex, true);
    if (tagName[0] == '!') {
        if (tagIndex >= planeTags_.v_.size()) {
            planeTags_.resize_(tagIndex + 1);
        }
        planeTags_.v_.set(tagIndex, true);
    }
    return result;
}

LayerTagCollection LayerTagManager::createTagCollection(std::list<std::string> tagNames) {
    LayerTagCollection result = LayerTagCollection();
    BOOST_FOREACH(std::string tagName, tagNames) {
        m_.insert(StringBimapItem(tagName, m_.size()));
        size_t tagIndex = m_.left.at(tagName);
        if (tagIndex >= result.v_.size()) {
            result.resize_(tagIndex + 1);
        }
        result.v_.set(tagIndex, true);
        if (tagName[0] == '!') {
            if (tagIndex >= planeTags_.v_.size()) {
                planeTags_.resize_(tagIndex + 1);
            }
            planeTags_.v_.set(tagIndex, true);
        }
    }
    return result;
}

LayerTagCollection LayerTagManager::createTagCollection(std::vector<std::string> tagNames) {
    LayerTagCollection result = LayerTagCollection();
    BOOST_FOREACH(std::string tagName, tagNames) {
        m_.insert(StringBimapItem(tagName, m_.size()));
        size_t tagIndex = m_.left.at(tagName);
        if (tagIndex >= result.v_.size()) {
            result.resize_(tagIndex + 1);
        }
        result.v_.set(tagIndex, true);
        if (tagName[0] == '!') {
            if (tagIndex >= planeTags_.v_.size()) {
                planeTags_.resize_(tagIndex + 1);
            }
            planeTags_.v_.set(tagIndex, true);
        }
    }
    return result;
}

std::list<std::string> LayerTagManager::getTagNames(const LayerTagCollection& tagCollection) {
    std::list<std::string> result;
    for (
        size_t i = tagCollection.v_.find_first();
        i != boost::dynamic_bitset<>::npos;
        i = tagCollection.v_.find_next(i)
    ) {
        result.push_back(m_.right.at(i));
    }
    result.sort();
    return result;
}

LayerTagMask LayerTagManager::getSingletonMask(std::string tagName) {
    return getMask(createSingletonTagCollection(tagName));
}

LayerTagMask LayerTagManager::getMask(std::string specification) {
    std::list< std::list<std::string> > tagNames = splitMaskSpecification(specification);
    if (tagNames.empty()) {
        return getSingletonMask(specification);
    } else {
        return getAlternativeMaskFromTagNames(tagNames);
    }
}

std::list<std::list<std::string> > LayerTagManager::multiplyMaskListByLangCode(
        const std::list<std::list<std::string> >& maskList,
        const std::string& langCode) {

    std::list<std::list<std::string> > retMaskList = maskList;

    std::string langCodeTag = getLanguageTag(langCode);

    for (std::list<std::list<std::string> >::iterator iter = retMaskList.begin();
         iter != retMaskList.end();
         ++iter)
        iter->push_back(langCodeTag);

    return retMaskList;
}


LayerTagMask LayerTagManager::getAlternativeMaskFromTagNames(
        std::list< std::list<std::string> > tagNames) {
    std::vector<LayerTagCollection> tagCollections;
    BOOST_FOREACH(std::list<std::string> conjunction, tagNames) {
        tagCollections.push_back(createTagCollection(conjunction));
    }
    return getAlternativeMask(tagCollections);
}

LayerTagCollection LayerTagManager::planeTags() {
    return planeTags_;
}

LayerTagCollection LayerTagManager::onlyPlaneTags(LayerTagCollection tags) {
    return createIntersection(planeTags_, tags);
}

bool LayerTagManager::areInTheSamePlane(LayerTagCollection tags1, LayerTagCollection tags2) {
    LayerTagCollection ptags1 = onlyPlaneTags(tags1);
    LayerTagCollection ptags2 = onlyPlaneTags(tags2);
    return ptags1 == ptags2;
}

bool LayerTagManager::isThere(std::string tagName, LayerTagCollection tags) {
    m_.insert(StringBimapItem(tagName, m_.size()));
    tags.resize_(m_.left.at(tagName) + 1);
    return tags.v_[m_.left.at(tagName)];
}

bool LayerTagManager::canBeAppliedToImplicitSymbol(const LayerTagMask& tagMask) {
    if (tagMask.isAny())
        return true;

    BOOST_FOREACH(LayerTagCollection tagAlt, tagMask.tagAlts_) {
        if (tagAlt == symbolTag_)
            return true;
    }

    return false;
}

std::list<std::string> LayerTagManager::splitCollectionSpecification(
        std::string specification) {
    std::vector<std::string> result;
    boost::split(result, specification, boost::is_any_of(","));
    return std::list<std::string>(result.begin(), result.end());
}

std::list< std::list<std::string> > LayerTagManager::splitMaskSpecification(
        std::string specification) {
    std::list< std::list<std::string> > result;
    LayerTagMaskSpecificationGrammar grammar;
    std::string::const_iterator begin = specification.begin();
    std::string::const_iterator end = specification.end();
    parse(begin, end, grammar, result);
    return result;
}
