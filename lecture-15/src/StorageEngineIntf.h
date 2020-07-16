#include <cstddef>
#include <vector>

template <typename EntityT>
class StorageEngineIntf 
{
public:
    using StorageKey = std::size_t;

	StorageEngineIntf();

	virtual StorageKey create(const EntityT& entity) = 0;

	virtual const EntityT& read(const StorageKey& key) const = 0;

	virtual std::vector<const EntityT&> read() const = 0;

	virtual void update(const StorageKey& key, const EntityT& entity) = 0;

	virtual void delette(const StorageKey& key) = 0;
};
