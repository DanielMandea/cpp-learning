#include "StorageEngineIntf.h"

class InMemoryStorageEngine : public StorageEngineIntf 
{
public:
	InMemoryStorageEngine();
	
	virtual void create() override;
	
	virtual void read() override;	
	
	virtual void update() override;
	
	virtual void delette() override;
};
