#include "StorageEngineIntf.h"

#include <unordered_map>

template <typename EntityT>
class InMemoryStorageEngine : public StorageEngineIntf<EntityT>
{
public:
    using typename StorageEngineIntf<EntityT>::StorageKey;
	InMemoryStorageEngine();

    virtual StorageKey create(const EntityT& entity) override;

    virtual const EntityT& read(const StorageKey& key) const override;

    virtual std::vector<const EntityT&> read() const override;

    virtual void update(const StorageKey& key, const EntityT& entity) override;

    virtual void delette(const StorageKey& key) override;

private:
    std::unordered_multimap<StorageKey, EntityT> mEntityMap;
};

template <typename EntityT>
InMemoryStorageEngine<EntityT>::InMemoryStorageEngine()
: mEntityMap{}
{

}

template <typename EntityT>
typename InMemoryStorageEngine<EntityT>::StorageKey InMemoryStorageEngine<EntityT>::create(const EntityT& entity)
{
    if (mEntityMap.find(entity.computeStorageKey()) == mEntityMap.end())
    {
        mEntityMap.emplace(entity.computeStorageKey(), entity);
    }
    else
    {
        throw std::runtime_error{"Key" + entity.computeStorageKey() + " already in the map"};
    }
}

template <typename EntityT>
const EntityT& InMemoryStorageEngine<EntityT>::read(const StorageKey& key) const
{

    auto range = mEntityMap.equal_range(key);
    std::vector<EntityT> entities();

    std::transform(range.first, range.second, entities.begin(), [&](const std::pair<StorageKey, EntityT>& entity){
        return entity.second;
    });
    auto itr = mEntityMap.find(key);
    if (itr != mEntityMap.end())
    {
        return itr->second;
    }
    throw std::runtime_error{"Key" + key + " not found in the map"};
}

template <typename EntityT>
std::vector<const EntityT&> InMemoryStorageEngine<EntityT>::read() const
{
    std::vector<const EntityT&> entities{};

    for(const auto& [key, entity] : mEntityMap)
    {
        entities.push_back(entity);
    }
}

template <typename EntityT>
void InMemoryStorageEngine<EntityT>::update(const StorageKey& key, const EntityT& entity)
{
    if (mEntityMap.find(entity.computeStorageKey()) != mEntityMap.end())
    {
        mEntityMap[entity.computeStorageKey()] = entity;
    }
    else
    {
        throw std::runtime_error{"Key" + entity.computeStorageKey() + " not found in the map"};
    }
}

template <typename EntityT>
void InMemoryStorageEngine<EntityT>::delette(const StorageKey& key)
{
    auto itr = mEntityMap.find(key);
    if (itr != mEntityMap.end())
    {
        mEntityMap.erase(itr);
    }
    else
    {
        throw std::runtime_error{"Key" + key + " not found in the map"};
    }
}
