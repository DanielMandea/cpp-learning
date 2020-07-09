
class StorageEngineIntf 
{
public:
	StorageEngineIntf();

	virtual void create() = 0;

	virtual void read() = 0;

	virtual void update() = 0;

	virtual void delette() = 0;
};
