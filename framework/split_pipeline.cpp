#include "split_pipeline.hpp"

#include <boost/algorithm/string.hpp>

std::vector<std::string> splitPipeline(const std::string& pipeline) {
    std::vector<std::string> strs;
    // the first element should be the name of the program
    boost::split(strs, pipeline, boost::is_any_of(" "));
    return strs;
}
