#include "stream_helpers.hpp"

#include <sstream>

std::string getStartingDataWithoutTouchingStream(std::istream& stream, int size) {
    char buffer[size];
    stream.read(buffer, size);

    // it is necessary to remove eofbit flag in the case of when the blockSize_
    // is greather than the total length of input
    stream.clear();

    int lastReadable = (int)stream.gcount();

    for (int i = lastReadable; i > 0; i--) {
        stream.putback(buffer[i - 1]);
    }

    return std::string(buffer, lastReadable);
}

std::string getDataFromIStream(std::istream& stream) {
    std::stringstream data;
    data << stream.rdbuf();

    stream.clear();

    return data.str();
}

void setDataToIStream(std::istream& stream, std::string data) {
    int dataSize = data.size();
    const char* rawData = data.c_str();

    for (int i = dataSize; i > 0; i--) {
        stream.putback(rawData[i - 1]);
    }
}

std::string getDataWithoutTouchingIStream_(std::istream& stream) {
    std::string data = getDataFromIStream(stream);
    setDataToIStream(stream, data);

    return data;
}
