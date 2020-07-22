//#include "InMemoryStorageEngine.h"
//
//InMemoryStorageEngine::InMemoryStorageEngine()
//: mStorageMap{}
//{
//
//}
//
//bool InMemoryStorageEngine::create(std::string name, std::string email)
//{
//    Entity entity{name,email};
//
//    auto itr = mStorageMap.find(entity.getId());
//
//    if (itr != mStorageMap.end())
//    {
//        return false;
//    }
//    mStorageMap.insert(std::make_pair(entity.getId(), std::make_unique<Entity>(entity)));
//    return true;
//}
//
//std::list<Entity> InMemoryStorageEngine::read()
//{
//    std::list<Entity> users{};
//
//    for (const auto& [key, value] : mStorageMap)
//    {
//        users.emplace_back(*value);
//    }
//    return users;
//}
//
//bool InMemoryStorageEngine::update(std::string name, std::string email)
//{
//    Entity entity {name, email};
//    auto itr = mStorageMap.find(entity.getId());
//
//    if (itr != mStorageMap.end())
//    {
//        itr->second = std::make_unique<Entity>(entity);
//    }
//
//
//    for (auto& [key, value] : mStorageMap)
//    {
//        if(name == value->getName())
//        {
//            value = std::make_unique<Entity>(entity);
//        }
//
//    }
//    return true;
//}
//
//bool InMemoryStorageEngine::delette(int id)
//{
//    if (id != 0)
//    {
//        mStorageMap.erase(id);
//    }
//    return true;
//}
