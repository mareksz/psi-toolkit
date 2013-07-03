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
    boost::optional<int> begin;
    boost::optional<int> end;

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
    {
        try {
            try {
                begin = a_lattice.getEdgeBeginIndex(a_source);
            } catch (WrongVertexException) {
                if (parent) {
                    begin = a_lattice.getEdgeBeginIndex(*parent);
                } else {
                    throw WrongVertexException("Loose edge.");
                }
            }
            try {
                end = a_lattice.getEdgeEndIndex(a_source);
            } catch (WrongVertexException) {
                if (parent) {
                    end = a_lattice.getEdgeEndIndex(*parent);
                } else {
                    throw WrongVertexException("Loose edge.");
                }
            }
        } catch (WrongVertexException) {
            // Leave begin and end uninitialized.
        }
    }

    bool operator<(const EdgeData & other) const {
        return category < other.category ||
            text < other.text ||
            score < other.score ||
            tags < other.tags ||
            avMap < other.avMap ||
            role < other.role ||
            begin < other.begin ||
            end < other.end;
    }
};


#endif
