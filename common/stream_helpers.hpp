#ifndef STREAM_HELPERS_HDR_HDR
#define STREAM_HELPERS_HDR_HDR

#include <iostream>

std::string getStartingDataWithoutTouchingStream(std::istream& stream, int size);

std::string getDataFromIStream(std::istream& stream);
void setDataToIStream(std::istream& stream, std::string data);

std::string getDataWithoutTouchingIStream(std::istream& stream);

#endif
