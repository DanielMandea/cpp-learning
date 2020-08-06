#include <optional>
#include <string>

class ClientAction
{
public:
	enum class Operation
	{
		CREATE,
		READ,
		UPDATE,
		DELETE
	};

	ClientAction(const std::string& operation, const std::string& userName);

	void setUserEmail(const std::string& userEmail);

	void setUpdatedUserName(const std::string& updatedUserName);

	void setUpdatedEmail(const std::string& updatedEmail);

	Operation getOperation() const;

	const std::string& getUserName() const;

	const std::string& getUserEmail() const;

	const std::string& getUpdatedUserName() const;

	const std::string& getUpdatedEmail() const;

private:
	Operation mOperation;
	std::string mUserName;
	std::optional<std::string> mUserEmail;
	std::optional<std::string> mUpdatedUserName;
	std::optional<std::string> mUpdatedEmail;

	Operation getOperationType(const std::string& operation) const;
};
