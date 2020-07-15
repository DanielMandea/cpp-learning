#include "InMemoryStorageEngine.h"

#include <vector>

InMemoryStorageEngine::InMemoryStorageEngine() :
    mStorageMap{}
{

}

bool InMemoryStorageEngine::create(Entity entity)
{
    if (mStorageMap.find(entity.getMId()) != mStorageMap.end())
    {
        return false;
    }
    mStorageMap.insert(std::make_pair(entity.getMId(), entity));

    return true;
}

std::vector<Entity> InMemoryStorageEngine::read(int id = -1)
{
    std::vector<Entity> entities{};

    if (id == -1)
    {
//        std::for_each(mStorageMap.begin(), mStorageMap.end(), [&](std::pair<int, Entity>& user){
//            entities.push_back(user.second);
//        });
        for (const auto& [key, value] : mStorageMap)
        {
            entities.emplace_back(value);
        }
    }
    else
    {
        entities.emplace_back(mStorageMap[id]);
    }

    return entities;
}

bool InMemoryStorageEngine::update(int id, Entity updatedEntity)
{
    if (mStorageMap.find(id) != mStorageMap.end())
    {
        return false;
    }
    mStorageMap[id] = updatedEntity;

    return true;
}

bool InMemoryStorageEngine::update(std::string )

bool InMemoryStorageEngine::delette(std::string name)
{
    for (const auto& [key, value] : mStorageMap)
    {
        if (name == value.getMName())
        {
            mStorageMap.erase(key);
            return true;
        }
    }
    return false;   // the name was not found in the map
}
