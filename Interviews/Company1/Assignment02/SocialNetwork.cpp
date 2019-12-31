#include "SocialNetwork.h"

void User::setName(const std::string &name)
{
	if (name.empty()) {
		throw std::invalid_argument("Empty name.");
	}
	m_name = name;
}

void User::setAge(uint8_t age)
{
	if (age == 0) {
		throw std::invalid_argument("Invalid age");
	}
	m_age = age;
}

void User::setHeight(uint8_t height)
{
	if (height < 66) { // Yoda can also join :)
		throw std::invalid_argument("Invalid height");
	}
	m_height = height;
}

void User::setId(const ID& id)
{
	if (id.empty()) {
		throw std::invalid_argument("Empty ID.");
	}
	m_id = id;
}


void SocialNetwork::addUser(const User& user)
{
	auto id = user.id();
	assert(!id.empty());

	auto it = m_users.find(id);
	if (it != m_users.end()) {
		auto usr = *(it->second);
		if (usr.name() != user.name()) {
			throw std::invalid_argument("Another user with that ID already exists");
		}
		return; // No insertion, same user (id/name) already added
	}

	m_users.insert({id, std::make_shared<User>(user)});

	auto name = user.name();
	assert(!name.empty());

	m_nameIdMap.insert({name, id});

	m_ageIdMap.insert({user.age(), id});

	auto hobbies = user.hobbies();
	for (auto const &hoby : hobbies) {
		m_hobbiesMap.insert({hoby, id});
	}

	auto friendIDs = user.friends();
	for (auto const &fId : friendIDs) {
		m_friendsMap.insert({fId, id});
	}
}

void SocialNetwork::deleteUser(const ID& id)
{
	auto it = m_users.find(id);
	if (it == m_users.end()) { // map::contains is not available until c++20...
		// not removed / invalid ID
		throw std::invalid_argument("Not existing user/ID");
	}

	removeMapItems(m_nameIdMap, it->second->name(), id);
	removeMapItems(m_ageIdMap, it->second->age(), id);

	auto hobbies = it->second->hobbies();
	for (auto const &hoby : hobbies) {
		removeMapItems(m_hobbiesMap, hoby, id);
	}

	auto friendIDs = it->second->friends();
	for (auto const &fId : friendIDs) {
		removeMapItems(m_friendsMap, fId, id);
	}

	m_users.erase(it);
}

User SocialNetwork::getUser(const ID &id) const
{
	auto it = m_users.find(id);
	if (it == m_users.end()) {
		// not existig / invalid ID
		throw std::invalid_argument("Not existing user/ID");
	}

	return *(it->second);
}

SocialNetwork::SharedUserList SocialNetwork::searchUserByHobbies(const std::set<std::string> &hobbies) const
{
	SocialNetwork::SharedUserList list;
	for (auto const &hoby : hobbies) {
		list.merge(searchUser(m_hobbiesMap, hoby));
	}
	return list;
}

std::set<ID> SocialNetwork::getFriendsOfUser(const ID& id) const
{
	std::set<ID> ret;

	auto search = m_users.find(id);
	if (search == m_users.end()) {
		// not existig / invalid ID
		throw std::invalid_argument("Not existing user/ID");
	}

	// Start with user's own users,
	ret = (*(search->second)).friends();

	// ... and append other users which reffer to the same user.
	auto range = m_friendsMap.equal_range(id);

	if (range.first == m_friendsMap.end()) {
		return ret; // no other records
	}

	for (auto it = range.first; it != range.second; ++it) {
		auto id_ = it->second;
		ret.insert(id_);
	}

	return ret;
}
