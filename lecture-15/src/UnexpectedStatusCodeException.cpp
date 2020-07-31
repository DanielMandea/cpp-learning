//
// Created by DragosIoanFechete on 31/07/2020.
//

#include "UnexpectedStatusCodeException.h"

UnexpectedStatusCodeException::UnexpectedStatusCodeException(const std::string& message)
: std::runtime_error(message)
{

}
