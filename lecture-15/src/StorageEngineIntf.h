#include "Entity.h"

#include <string>

class StorageEngineIntf
{
public:
	StorageEngineIntf();

//	virtual bool create() = 0;
//
//  virtual bool read() = 0;
//
//	virtual bool update() = 0;
//
//	virtual bool delette() = 0;

    virtual bool create(Entity entity) = 0;

    virtual std::vector<Entity> read(int id) = 0;

    virtual bool update(std::string name, std::string updatedEmail) = 0;

    virtual bool delette(std::string name) = 0;
};
