#ifndef TOOLPROJECT_UNEXPECTEDSTATUSCODEEXCEPTION_H
#define TOOLPROJECT_UNEXPECTEDSTATUSCODEEXCEPTION_H


#include <exception>
#include <string>

class UnexpectedStatusCodeException : public std::runtime_error {
public:
    UnexpectedStatusCodeException(const std::string& message);
};


#endif //TOOLPROJECT_UNEXPECTEDSTATUSCODEEXCEPTION_H
