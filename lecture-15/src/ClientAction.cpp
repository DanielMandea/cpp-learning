#include "ClientAction.h"

#include <stdexcept>

ClientAction::ClientAction(const std::string& operation, const std::string& userName)
: mOperation{getOperationType(operation)},
  mUserName{userName},
  mUserEmail{},
  mUpdatedUserName{}
{

}

void ClientAction::setUserEmail(const std::string& userEmail)
{
	mUserEmail = userEmail;
}

void ClientAction::setUpdatedUserName(const std::string& updatedUserName)
{
	mUpdatedUserName = updatedUserName;
}

ClientAction::Operation ClientAction::getOperation() const
{
	return mOperation;
}

const std::string& ClientAction::getUserName() const
{
	return mUserName;
}

const std::string& ClientAction::getUserEmail() const
{
	return *mUserEmail;
}

const std::string& ClientAction::getUpdatedUserName() const
{
	return *mUpdatedUserName;
}

const std::string& ClientAction::getUpdatedEmail() const
{
    return *mUpdatedEmail;
}

ClientAction::Operation ClientAction::getOperationType(const std::string& operation) const
{
	if ("create" == operation)
	{
		return Operation::CREATE;
	} 

	if ("read" == operation)
	{
		return Operation::READ;
	}

	if ("update" == operation)
	{
		return Operation::UPDATE;
	} 

	if ("delete" == operation)
	{
		return Operation::DELETE;
	} 

	throw std::runtime_error{"Unknown operation"};
}
