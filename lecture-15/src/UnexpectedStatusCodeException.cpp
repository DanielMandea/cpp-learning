
#include "UnexpectedStatusCodeException.h"

UnexpectedStatusCodeException::UnexpectedStatusCodeException(const std::string& message)
: std::runtime_error(message)
{

}