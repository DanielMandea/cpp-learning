#include "StorageEngineIntf.h"

#include <map>

class InMemoryStorageEngine : public StorageEngineIntf 
{
public:
	InMemoryStorageEngine();

    virtual bool create(Entity entity) override;

    virtual std::vector<Entity> read(int id) override;
	
	virtual bool update(std::string name, std::string updatedEmail) override;
	
	virtual bool delette(std::string name) override;

private:
    std::map<int, Entity> mStorageMap;
};
