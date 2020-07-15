#include "Entity.h"


Entity::Entity(std::string name, std::string email) :
    mId{mStaticId++},
    mName{name},
    mEmail{email}
{

}

Entity::Entity(const Entity &entity) :
    mId{entity.getMId()},
    mName{entity.getMName()},
    mEmail{entity.getMEmail()}
{

}

const std::string &Entity::getMName() const {
    return mName;
}

void Entity::setMId(int mId) {
    Entity::mId = mId;
}

void Entity::setMName(const std::string &mName) {
    Entity::mName = mName;
}

const std::string &Entity::getMEmail() const {
    return mEmail;
}

int Entity::getMId() const {
    return mId;
}

void Entity::setMEmail(const std::string &mEmail) {
    Entity::mEmail = mEmail;
}
