#include <atomic>
#include <string>
#include <utility>

class Entity
{
public:
    Entity(std::string name, std::string email);

    Entity(const Entity &entity);

    void setMName(const std::string &mName);

    const std::string &getMName() const;

    void setMEmail(const std::string &mEmail);

    const std::string &getMEmail() const;

    int computeStorageKey() const;

private:
    int mId;
    std::string mName;
    std::string mEmail;

    static std::atomic<int> mStaticId;
};
