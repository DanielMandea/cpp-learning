

// Team.hxx header file

class Player;

class Team
{
private:
	std::vector<std::unique_ptr<Player>> mPlayerList;
};








// -- Player.hxx header file

class Team;

class Player
{
public:

	void  setTeam(const std::shared_ptr<Team>& team)
	{
		mTeam = team;
	}

	void doSmth()
	{
		if (mTeam)
		{
			shared_ptr<Team> strongPtr = mTeam.lock();
		}

		mTeam.lock()->showTeam();
	}

private:
	std::weak_ptr<Team> mTeam;
};







// -- Main.cpp -- client code

int main()
{
	Player p1, p2, p3;

	Team t{p1, p2, p3};

	p1.setTeam(t);
	p2.setTeam(t);

	return 0;
}




