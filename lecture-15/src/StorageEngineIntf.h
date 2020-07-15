#include "Entity.h"
#include <list>

class StorageEngineIntf 
{
public:
	StorageEngineIntf();

	virtual bool create(std::string name, std::string email) = 0;

	virtual std::list<Entity> read() = 0;

	virtual bool update(std::string name, std::string email) = 0;

	virtual bool delette(int id) = 0;
};
