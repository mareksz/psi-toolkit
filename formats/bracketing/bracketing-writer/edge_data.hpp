#ifndef EDGE_DATA_HDR
#define EDGE_DATA_HDR


#include <map>
#include <set>
#include <string>

#include <boost/optional.hpp>

#include "lattice.hpp"


struct EdgeData {
    boost::optional<Lattice::EdgeDescriptor> source;
    boost::optional<Lattice::EdgeDescriptor> parent;
    std::set<std::string> tags;
    std::string category;
    std::string text;
    std::map<std::string, std::string> avMap;
    double score;
    std::string role;

    EdgeData() :
        tags(std::set<std::string>()),
        category(""),
        text(""),
        avMap(std::map<std::string, std::string>()),
        score(0.0),
        role("")
    { }

    EdgeData(
        Lattice & a_lattice,
        Lattice::EdgeDescriptor a_source,
        std::set<std::string> a_tags,
        std::string a_category,
        std::string a_text,
        std::map<std::string, std::string> a_avMap,
        double a_score,
        std::string a_role
    ) :
        source(a_source),
        parent(a_lattice.getParent(a_source)),
        tags(a_tags),
        category(a_category),
        text(a_text),
        avMap(a_avMap),
        score(a_score),
        role(a_role)
    { }

    bool operator<(const EdgeData & other) const {
        return category < other.category ||
            text < other.text ||
            score < other.score ||
            tags < other.tags ||
            avMap < other.avMap ||
            role < other.role;
    }
};


#endif
