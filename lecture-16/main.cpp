#include <list>

class User
{
private:
	int mAge;

	bool operator<(const User& user) const
	{
		return mAge < user.mAge;
	}
}

namespace std
{
	template<> 
	hash<User>
	{
		size_t operator(const User& user) const
		{
			return hash<int>{}(user.mAge);
		}
	}
}

int main()
{
	std::array<int, size> a;
	
	/*
		inserare
		cautare
		stergere
		iterare
	*/

	std::list<int> list{};
	// list.at(4) = 5;

	std::queue<>;
	std::stack<>;
	std::deque<>;
	std::priority_queue<>;
	// 10, 20, 30, 40, 50, 60, 70, 80
	std::set<>;
	std::map<>;
	std::unordered_set<>;
	std::unordered_map<>;

	std::map<User, std::string> age_map{};
	std::unordered_map<User, std::string> age_map{};

	return 0;
}