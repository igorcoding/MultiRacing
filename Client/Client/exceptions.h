#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <string>

class ResourceException : public std::exception
{
public:

    ResourceException(const char *str)
        : _what(str)
    {}

    const char *what() const throw()
    {
        return _what.c_str();
    }

private:
    std::string _what;
};

#endif // EXCEPTIONS_H
