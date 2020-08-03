#ifndef TOOLPROJECT_LOCATIONNOTFOUNDEXCEPTION_H
#define TOOLPROJECT_LOCATIONNOTFOUNDEXCEPTION_H


#include <exception>
#include <string>

class LocationNotFoundException : public std::runtime_error {
public:
    LocationNotFoundException(const std::string& message);
};


#endif //TOOLPROJECT_LOCATIONNOTFOUNDEXCEPTION_H
