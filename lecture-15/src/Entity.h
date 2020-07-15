#include <string>

class Entity
{
public:

    Entity(std::string name, std::string email);

    Entity(const Entity &entity) : mId{entity.getId()}, mName{entity.getName()}, mEmail{entity.getEmail()}
    {

    }

    const std::string& getName() const {return mName;}

    const std::string& getEmail() const {return mEmail;}

    int getId() const {return mId;}

    void setName(std::string name) { mName = std::move(name);}

    void setEmail(std::string email) { mName = std::move(email);}

private:
    int mId;
    std::string mName;
    std::string mEmail;
};
