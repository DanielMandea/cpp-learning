#ifndef SRC_STORAGEENGINEINTF_H
#define SRC_STORAGEENGINEINTF_H

#include <cstddef>
#include <vector>

template <typename EntityT>
class StorageEngineIntf 
{
public:
    using StorageKey = std::size_t;

	virtual ~StorageEngineIntf() = default;

	virtual StorageKey create(const EntityT& entity) = 0;

	virtual const EntityT& read(const StorageKey& key) const = 0;

	virtual std::vector<std::reference_wrapper<const EntityT>> read() const = 0;

	virtual void update(const StorageKey& key, const EntityT& entity) = 0;

	virtual void delette(const StorageKey& key) = 0;
};

#endif
