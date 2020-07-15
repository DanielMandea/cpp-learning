#include "StorageEngineIntf.h"

#include <list>
#include <map>
#include <string>

class InMemoryStorageEngine : public StorageEngineIntf 
{
public:
	InMemoryStorageEngine();
	
	virtual bool create(std::string name, std::string email) override;
	
	virtual std::list<Entity> read() override;
	
	virtual bool update(std::string name, std::string email) override;
	
	virtual bool delette(int id) override;

private:
    std::map<int, std::unique_ptr<Entity>> mStorageMap;
};
