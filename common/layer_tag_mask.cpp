#include "layer_tag_mask.hpp"

#include <boost/foreach.hpp>

bool LayerTagMask::isNone() {
    if (none_)
        return true;

    if (any_)
        return false;

    BOOST_FOREACH(LayerTagCollection tagAlt, tagAlts_) {
        if (!tagAlt.isEmpty())
            return false;
    }

    none_ = true;
    return true;
}

bool LayerTagMask::isSome() {
    if (any_)
        return true;

    BOOST_FOREACH(LayerTagCollection tagAlt, tagAlts_) {
        if (!tagAlt.isEmpty())
            return true;
    }

    return false;
}

bool LayerTagMask::isAny() const {
    return any_;
}

bool LayerTagMask::operator<(LayerTagMask other) const {
    if (none_ && other.any_)
        return true;

    if (other.none_ && any_)
        return false;

    if (tagAlts_.size() < other.tagAlts_.size())
        return true;

    if (tagAlts_.size() > other.tagAlts_.size())
        return false;

    for (size_t i = 0; i < tagAlts_.size(); ++i) {
        if (tagAlts_[i] < other.tagAlts_[i])
            return true;

        if (other.tagAlts_[i] < tagAlts_[i])
            return false;
    }

    return false;
}

bool LayerTagMask::operator==(LayerTagMask other) const {
    if (any_ && other.any_)
        return true;

    if (none_ && other.none_)
        return true;

    if (any_ != other.any_)
        return false;

    if (none_ != other.none_)
        return false;

    if (tagAlts_.size() != other.tagAlts_.size())
        return false;

    for (size_t i = 0; i < tagAlts_.size(); ++i) {
        if (tagAlts_[i] != other.tagAlts_[i])
            return false;
    }

    return true;
}

unsigned long LayerTagMask::getHash() const {
    return hash_;
}

void LayerTagMask::computeHash_() {
    if (any_) {
        hash_ = 1;
    } else if (!none_) {
        BOOST_FOREACH(LayerTagCollection tagAlt, tagAlts_) {
            hash_ ^= tagAlt.getHash();
        }
        hash_ += 2;
    }
}

bool matches(
    LayerTagCollection tags,
    LayerTagMask mask) {

    if (mask.none_)
        return false;

    if (mask.any_)
        return true;

    BOOST_FOREACH(LayerTagCollection tagAlt, mask.tagAlts_) {
        if (isSubset(tagAlt, tags))
            return true;
    }

    return false;
}
