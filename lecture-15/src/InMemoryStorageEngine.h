#include "StorageEngineIntf.h"

class InMemoryStorageEngine : public StorageEngineIntf 
{
public:
	InMemoryStorageEngine();
	
	virtual bool create() override;
	
	virtual bool read() override;
	
	virtual bool update() override;
	
	virtual bool delette() override;
};
