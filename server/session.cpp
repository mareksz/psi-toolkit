#include "session.hpp"
#include "logging.hpp"

#include <time.h>

Session::Session(std::string sid)
    : id(sid), createdTime(time(NULL)) { }

std::string Session::getId() const {
    return id;
}

bool Session::isData(const std::string& key) {
    std::map<std::string, std::string>::iterator it = data.find(key);

    if (it != data.end()) {
        if (it->second == "") {
            return false;
        }
        return true;
    }

    return false;
}

std::string Session::getData(const std::string& key) {
    std::map<std::string, std::string>::iterator it = data.find(key);
    std::string value("");

    if (it != data.end()) {
        value = it->second;
    }

    return value;
}

std::string Session::getDataOrSetIfNotExists(const std::string& key, const std::string& value) {
    if (not isData(key)) {
        setData(key, value);
    }
    return getData(key);
}

void Session::setData(const std::string& key, const std::string& value) {
    data[key] = value;
}

void Session::clearData(const std::string& key) {
    data[key] = std::string("");
}
