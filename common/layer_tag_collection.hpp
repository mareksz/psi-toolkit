#ifndef LAYER_TAG_COLLECTION_HDR
#define LAYER_TAG_COLLECTION_HDR

#include <bitset>


/*!
  A large number of various annotations originated from various
  annotators (eg. tokeniser, parser, translator etc.) will be kept. In
  order to distinguish annotations of different origins, layer tags
  are introduced. Each lattice edge can be tagged with multiple layer tags.
  Layer tags are flexible, the following things can be expressed with layer tags:
  * annotation level (token, segment, phrase etc.)
  * annotator used (gobio, puddle, translatica-tokenizer etc.)
  * tagset used (translatica-dic, morfeusz, kipi etc.)

  To keep layer tags, LayerTagCollection are introduced (there is no
  LayerTag class).

  LayerTagCollection can be created with LayerTagManager.

  */
class LayerTagCollection {

public:
    bool isEmpty();

    bool isNonempty();

    /**
     * method for compatibility with boost::bimap
     */
    bool operator<(LayerTagCollection other) const;

    bool operator==(const LayerTagCollection& other) const;

    bool operator!=(const LayerTagCollection& other) const;

    /**
     * method for compatibility with HashWrapper3
     */
    unsigned long getHash() const;

    /**
     * created union of two layertaglists
     */
    friend LayerTagCollection createUnion(
        LayerTagCollection tag_list_a,
        LayerTagCollection tag_list_b
    );

    /**
     * created intersection of two layertaglists
     */
    friend LayerTagCollection createIntersection(
        LayerTagCollection tag_list_a,
        LayerTagCollection tag_list_b
    );

    friend bool isSubset(
        LayerTagCollection tag_list_a,
        LayerTagCollection tag_list_b);

    friend class LayerTagManager;

    friend class LayerTagMask;

private:
    /**
     * private constructor! only LayerTagManager can be used to create layer tag collecions
     */
    LayerTagCollection() {};

    /**
     * private constructor! only LayerTagManager can be used to create layer tag collecions
     */
    LayerTagCollection(int /* size */) { };

    std::bitset<64> v_;

    /**
     * resizes to the nearest power of two
     */
    size_t resize_(size_t size);

    /**
     * equalizes lengths of two LayerTagCollection vectors
     */
    size_t resize_(LayerTagCollection& other);

};


#endif
