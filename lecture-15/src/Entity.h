#include <string>

class Entity
{
public:

    Entity(const Entity &entity);

    Entity(std::string name, std::string email);

    const std::string& getName() const;

    const std::string& getEmail() const;

    int getId() const;

    void setName(const std::string &name);

    void setEmail(const std::string &email);

    int computeStorageKey() const;

private:
    int mId;
    std::string mName;
    std::string mEmail;
};
