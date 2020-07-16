#include "InMemoryStorageEngine.h"

InMemoryStorageEngine::InMemoryStorageEngine()
{
	
}

StorageEngineIntf::StorageKey InMemoryStorageEngine::create(const EntityT& entity) = 0;

const EntityT& InMemoryStorageEngine::read(const StorageEngineIntf::StorageKey& key) const = 0;

std::vector<const EntityT&> InMemoryStorageEngine::read() const = 0;

void InMemoryStorageEngine::update(const StorageKey& key, const EntityT& entity) = 0;

void InMemoryStorageEngine::delette(const StorageKey& key) = 0;
