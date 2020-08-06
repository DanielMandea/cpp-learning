#include "Entity.h"

#include <utility>

std::atomic<int> Entity::mStaticId{};

Entity::Entity(std::string name, std::string email) :
        mId{++mStaticId},
        mName{std::move(name)},
        mEmail{std::move(email)}
{

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
    return mId;
}
