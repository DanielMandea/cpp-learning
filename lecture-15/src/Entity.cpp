#include "Entity.h"

Entity::Entity(std::string name, std::string email) :
        mId{},
        mName{std::move(name)},
        mEmail{std::move(email)}
{
}
Entity::Entity(const Entity &entity) :
        mId{entity.computeStorageKey()},
        mName{entity.getName()},
        mEmail{entity.getEmail()}
{
}
const std::string &Entity::getName() const {
    return mName;
}
void Entity::setName(const std::string &name) {
    mName = name;
}
const std::string &Entity::getEmail() const {
    return mEmail;
}
void Entity::setEmail(const std::string &email) {
    mEmail = email;
}
int Entity::computeStorageKey() const
{
    int key{};
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