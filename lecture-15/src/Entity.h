#include <string>
#include <utility>

class Entity
{
public:
    Entity(std::string name, std::string email);

    Entity(const Entity &entity);

    void setMName(const std::string &mName);

    const std::string &getMName() const;

    void setMName(const std::string &mName);

    const std::string &getMEmail() const;

    void setMEmail(const std::string &mEmail);

    int getMId() const;

    void setMId(int mId);

private:
    int mId;
    std::string mName;
    std::string mEmail;

    static int mStaticId;
};
