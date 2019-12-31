#pragma once

/* In this exercise we want to design a simple social network.
 * We have users which have a name, an age, a height (in cm), a set of hobbies,
 * an entry whether they are male or female, an internal ID and a set of friends.
 * A user doesn't have to provide all information except the name. The name always has to be provided. (think about how to design this)
 *
 * Our SocialNetwork should provide us with the possibility to add and delete users
 * from the network. It should also allow us to search users by name, age, id or hobbies.
 * Eg.:
 *      searchUserByName("Jochen");
 *      searchUserByAge(36);
 *      searchUserByHobbies({"Jogging", "Football", "Tennis"}); //Returns alls users which have the hobbies jogging, footbal AND tennis.
 * Additionally we want to receive all the friends a particular user has. But we only want to receive the set of ID's of those users (friends).
      ... getFriendsOfUser(...)
 *
 * Neither the names of the users have to be unique (e.g. we can have multiple Klaus in the network) nor any other attribute.
 * Only the id of every user has to be unique in the whole network.
 *
 * The skeleton code below can be used as a starting point but doesn't have to.
 * The comments "fill in" are placeholders where you definitely have to put in some code when
 * you use this skeleton code. But this doesn't mean that you shouldn't or can't put code anywhere else.
 *
 * Also write some simple unit tests to show how you would test the functionality of the Network.
 * Don't use any testing framework. Simple if-statements are sufficient for this exercise.
 *
 * Hint: Think about performance versus memory tradeoffs in your design, so you can give good
 *       reasons for your decision.
 */

// Martin Flaska - flegy@flegy.sk / https://www.linkedin.com/in/martinflaska

#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include <cassert>

enum class Gender {
	male,
	female
};

typedef std::string ID;

class User
{
public:
	/** Create a user
	 * @param id Unique/nonempty user ID
	 * @param name User name. Must not be empty.
	 */
	User(ID id, const std::string &name)
		: m_name(name)
		, m_id(id)
	{
		if (m_name.empty()) {
			throw std::invalid_argument("Empty name.");
		}
		if (m_id.empty()) {
			throw std::invalid_argument("Empty ID.");
		}
	}
	User() = delete; // Disable default ctor / do not allow a user with empty id/name

	void setName(const std::string &name);
	const std::string& name() const { return m_name; }

	/** Set age (years) */
	void setAge(uint8_t age);
	uint8_t age() const { return m_age; }

	/** Set height (cm) */
	void setHeight(uint8_t height);
	uint8_t height() const { return m_height; }

	void setHobbies(const std::set<std::string> &hobbies) { m_hobbies = hobbies; }
	const std::set<std::string> &hobbies() const { return m_hobbies; }

	void setGenderu(Gender gender) { m_gender = gender; }
	Gender gender() const { return m_gender; }

	void setId(const ID &id);
	const ID& id() const { return m_id; }

	void setFriends(const std::set<std::string> &friends) { m_friends = friends; }
	const std::set<ID> &friends() const { return m_friends; }

private:
	std::string m_name;
	uint8_t m_age; ///< years
	uint8_t m_height; ///< cm
	std::set<std::string> m_hobbies;
	Gender m_gender;
	ID m_id;
	std::set<ID> m_friends;
};

class SocialNetwork
{
public:
	void addUser(const User &user);
	//void addUser(User &&user); // in a case we want do even better memory optimization...

	void deleteUser(const ID &id);
	void deleteUser(const User &user) { deleteUser(user.id()); } // Convenience / overloaded method
	User getUser(const ID &id) const;
	int userCount() const { return m_users.size(); }

	typedef std::list<std::shared_ptr<User>> SharedUserList;

	/** Lookup methods
	 *
	 * @note Returned list is optimized to save memory and to not copy & pase User data from the user map (using shared_ptr)
	 *
	 * @{
	 */
	SharedUserList searchUserByName(const std::string &name) const { return searchUser(m_nameIdMap, name); }
	SharedUserList searchUserByAge(uint8_t age) const { return searchUser(m_ageIdMap, age); }
	SharedUserList searchUserByHobbies(const std::set<std::string> &hobbies) const;
	/** Return user friends by ID
	 *
	 * @note This one is little bit tricky as there is unclear how to decide who are User's friends. I suppose this is meant
	 * to be:
	 *    User::friends() + [any other users which have set their frend this user]
	 *
	 * @see getUser
	 */
	std::set<ID> getFriendsOfUser(const ID &id) const;
	std::set<ID> getFriendsOfUser(const User &user) const { return getFriendsOfUser(user.id()); } // Convenience / overloaded method
	/* @} */

private:
	typedef std::multimap<std::string, ID> StringIdMap;
	typedef std::multimap<uint8_t, ID> Uint8IdMap;

	/** helper method to remove all @map items related to @id */
	template<typename Map, typename Value>
	static void removeMapItems(Map &map, const Value &value, const ID &id)
	{
		auto range = map.equal_range(value);
		if (range.first != map.end()) {
			for (auto it = range.first; it != range.second; ) {
				auto id_ = it->second;
				if (id_ == id) {
					it = map.erase(it);
				} else {
					it++;
				}
			}
		}
	}

	/** Helper method to get all users for a @map related to @value */
	template<typename Map, typename Value>
	SharedUserList searchUser(const Map &map, const Value &value) const
	{
		auto range = map.equal_range(value);

		if (range.first == map.end()) {
			return SocialNetwork::SharedUserList(); // no record
		}

		SocialNetwork::SharedUserList list;

		for (auto it = range.first; it != range.second; ++it) {
			auto id = it->second;
			auto uIt = m_users.find(id);
			list.push_back(uIt->second);
		}

		return list;
	}

private:
	/** Map of all users (by ID key)
	 * We could have it also as a 'map<ID, shared_ptr<User>>>' if the number of user is really huge, so we can expect also
	 * big mem overhead for 'searchUserBy...' methods returning neither user IDs nor shared_ptr<User> but User.
	 */
	std::map<ID, std::shared_ptr<User>> m_users; // Note: ID is also part of User - if ID is not too big we do not care much about this overhead

	// Helper maps for faster lookup into 'm_users' by name, age, ...
	StringIdMap m_nameIdMap;
	Uint8IdMap m_ageIdMap;
	StringIdMap m_hobbiesMap;
	std::multimap<ID, ID> m_friendsMap;
};
