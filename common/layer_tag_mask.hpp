#ifndef LAYER_TAG_MASK_HDR
#define LAYER_TAG_MASK_HDR


#include <vector>

#include "layer_tag_collection.hpp"


class LayerTagMask {

public:

    bool isNone();

    bool isSome();

    bool isAny() const;

    /**
     * method for compatibility with boost::bimap
     */
    bool operator<(LayerTagMask other) const;

    bool operator==(LayerTagMask other) const;

    /**
     * method for compatibility with HashWrapper3
     */
    unsigned long getHash() const;

    friend bool matches(
        LayerTagCollection tags,
        LayerTagMask mask);

    friend class LayerTagManager;

private:
    /**
     * private constructor! only LayerTagManager can be used to create layer tag masks
     */
    LayerTagMask(LayerTagCollection tags) :
        tagAlts_(std::vector<LayerTagCollection>(1, tags)),
        any_(false),
        none_(false),
        hash_(0) {
            computeHash_();
    }

    LayerTagMask(const std::vector<LayerTagCollection> & tagAlts) :
        tagAlts_(tagAlts),
        any_(false),
        none_(false),
        hash_(0) {
            computeHash_();
    }

    /**
     * private constructor! only LayerTagManager can be used to create layer tag masks.
     * Creates mask ANY if val==1 and mask NONE if val==0.
     */
    LayerTagMask(bool val) :
        any_(val),
        none_(!val),
        hash_(0) {
            computeHash_();
    }

    std::vector<LayerTagCollection> tagAlts_;

    bool any_;
    bool none_;

    unsigned long hash_;

    void computeHash_();
};


#endif
