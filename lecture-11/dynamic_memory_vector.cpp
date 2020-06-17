
#include <cstring>
#include <iostream>


template <typename T>
class vector
{
public:
	vector() : mCapacity{}, mSize{}, mStorage{}
	{

	}

	~vector()
    {
        delete[] mStorage;
        
        std::cout << "released memory\n\n";
    }

	void push_back(const T& t)
	{
		if (getSize() == getCapacity())
		{
			reallocate();
		}

		mStorage[mSize++] = t;
	}

	int getSize() const
	{
		return mSize;
	}

	int getCapacity() const
	{
		return mCapacity;
	}
    
    void display() const
    {
        for (auto itr = 0; itr < getSize(); ++itr)
        {
            std::cout << mStorage[itr] << ", ";
        }
        
        std::cout << "\n\n";
    }

private:
	int mCapacity;
	int mSize;
	T* mStorage;

	void reallocate()
	{
		auto* temporaryStorage = new T[2 * getCapacity()];

		std::memcpy(temporaryStorage, mStorage, getCapacity() * sizeof(T));

		mCapacity *= 2;
        
        delete[] mStorage;
        
        mStorage = temporaryStorage;
	}
};


int main()
{

    {
		vector<int> x{};

		x.push_back(10);
		x.push_back(20);
    	x.push_back(100);
    	x.display(); 
    }
    
	return 0;
}
