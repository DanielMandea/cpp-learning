#include "StorageEngineIntf.h"

#include <string>
#include <unordered_map>

template <typename EntityT>
class InMemoryStorageEngine : public StorageEngineIntf<EntityT>
{
public:
    using typename StorageEngineIntf<EntityT>::StorageKey;
	InMemoryStorageEngine();

    StorageKey create(const EntityT& entity) override;

    const EntityT& read(const StorageKey& key) const override;

    std::vector<std::reference_wrapper<const EntityT>> read() const override;

    void update(const StorageKey& key, const EntityT& entity) override;

    void delette(const StorageKey& key) override;

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
        return entity.computeStorageKey();
    }

    throw std::runtime_error{"Key" + std::to_string(entity.computeStorageKey()) + " already in the map"};
}

template <typename EntityT>
const EntityT& InMemoryStorageEngine<EntityT>::read(const StorageKey& key) const
{
    std::vector<EntityT> entities{};

    auto itr = mEntityMap.find(key);
    if (itr != mEntityMap.end())
    {
        return itr->second;
    }
    throw std::runtime_error{"Key `" + std::to_string(key) + "` not found in the map"};
}

template <typename EntityT>
std::vector<std::reference_wrapper<const EntityT>> InMemoryStorageEngine<EntityT>::read() const
{
    std::vector<std::reference_wrapper<const EntityT>> entities{};

    for(const auto& [key, entity] : mEntityMap)
    {
        entities.push_back(std::cref(entity));
    }

    return entities;
}

template <typename EntityT>
void InMemoryStorageEngine<EntityT>::update(const StorageKey& key, const EntityT& entity)
{
    const auto itr = mEntityMap.find(key);
    if (itr == mEntityMap.end())
    {
        throw std::runtime_error{"Key " + std::to_string(key) + " not found in the map"};
    }

    if (!entity.getMName().empty())
    {
        itr->second.setMName(entity.getMName());
    }

    if (!entity.getMEmail().empty())
    {
        itr->second.setMEmail(entity.getMEmail());
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
        throw std::runtime_error{"Key " + std::to_string(key) + " not found in the map"};
    }
}
