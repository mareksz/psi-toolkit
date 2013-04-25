#include "split_pipeline.hpp"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

std::vector<std::string> splitPipeline(const std::string& pipeline) {
    std::vector<std::string> strs;
    // the first element should be the name of the program
    boost::tokenizer< boost::escaped_list_separator<char> >
        tokenized(pipeline, boost::escaped_list_separator<char>('\\', ' ', '\"'));
    BOOST_FOREACH(std::string token, tokenized) {
        strs.push_back(token);
    }
    return strs;
}
