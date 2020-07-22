#include "Entity.h"

#include <utility>


Entity::Entity(std::string name, std::string email) :
        mId{},
        mName{std::move(name)},
        mEmail{std::move(email)}
{
    mId = computeStorageKey();
}

Entity::Entity(const Entity &entity) :
        mId{entity.computeStorageKey()},
        mName{entity.getMName()},
        mEmail{entity.getMEmail()}
{

}

const std::string &Entity::getMName() const {
    return mName;
}

void Entity::setMName(const std::string &name) {
    mName = name;
}

const std::string &Entity::getMEmail() const {
    return mEmail;
}

void Entity::setMEmail(const std::string &email) {
    mEmail = email;
}

int Entity::computeStorageKey() const
{
    int key = 0;
    for (char c : mName)
    {
        key += (int)c;
    }
    for (char c : mEmail)
    {
        key += (int)c;
    }

    return key % 1000;
}
