#include "LocationNotFoundException.h"

LocationNotFoundException::LocationNotFoundException(const std::string &message)
        : std::runtime_error(message)
{

}