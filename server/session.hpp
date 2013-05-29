#ifndef SESSION_HDR
#define SESSION_HDR

#include <string>
#include <map>
#include <time.h>

class Session
{

public:

    Session(std::string sid);
    std::string getId() const;

    bool isData(const std::string& key);
    std::string getData(const std::string& key);
    void setData(const std::string& key, const std::string& value);
    void clearData(const std::string& key);

private:

    std::string id;
    time_t createdTime;

    std::map<std::string, std::string> data;

};

#endif
