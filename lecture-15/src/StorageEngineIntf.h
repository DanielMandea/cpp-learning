
class StorageEngineIntf 
{
public:
	StorageEngineIntf();

	virtual bool create() = 0;

	virtual bool read() = 0;

	virtual bool update() = 0;

	virtual bool delette() = 0;
};
