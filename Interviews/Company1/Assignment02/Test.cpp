#include "Test.h"
#include "SocialNetwork.h"

#include <cassert>

namespace {


void testUser_emptyName()
{
	try {
		User user("id-001", ""); //  this is expected to fail - throws an exception
		assert(false); // should not be called if an exception is thrown
	}
	catch (const std::exception& e) {
		// this is expected
	}
}

void testUser_emptyId()
{
	try {
		User user("", "John"); //  this is expected to fail - throws an exception
		assert(false); // should not be called if an exception is thrown
	}
	catch (const std::exception& e) {
		// this is expected
	}
}

void testDuplicates()
{
	SocialNetwork sn;

	User user1("id-001", "John");

	sn.addUser(user1);
	assert(sn.userCount() == 1);

	sn.addUser(user1);
	assert(sn.userCount() == 1);
}

void testAddUser()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		sn.addUser(user1);
		assert(sn.userCount() == 1);

		User user2("id-002", "John"); // Same name but different ID
		sn.addUser(user2);
		assert(sn.userCount() == 2);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() <<std::endl;
		assert(0);
	}
}

void testAddUser_sameNameAndId()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		sn.addUser(user1);
		assert(sn.userCount() == 1);

		User user2("id-001", "John"); // Same name & ID
		sn.addUser(user2); //  this is expected to fail - throws an exception
		assert(sn.userCount() == 1); // adding a user of same ID/name is ignored
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() <<std::endl;
		assert(0);
	}
}

void testAddUser_sameId()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		sn.addUser(user1);
		assert(sn.userCount() == 1);

		User user2("id-001", "Paul"); // Same name & ID
		sn.addUser(user2); //  this is expected to fail - throws an exception
		assert(false); // should not be called if an exception is thrown
	}
	catch (const std::exception& e) {
		// this is expected
	}
}

void testDeleteUser()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		sn.addUser(user1);
		assert(sn.userCount() == 1);

		User user2("id-002", "Paul");
		sn.addUser(user2);
		assert(sn.userCount() == 2);

		sn.deleteUser(user1.id()); // delete by ID
		assert(sn.userCount() == 1);

		sn.deleteUser(user2); // delete by user (and its ID)
		assert(sn.userCount() == 0);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() <<std::endl;
		assert(0);
	}
}

void testSearchUserByName()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		sn.addUser(user1);

		User user2("id-002", "Paul");
		sn.addUser(user2);

		User user3("id-003", "John"); // same name as for user1
		sn.addUser(user3);

		assert(sn.userCount() == 3);

		auto users = sn.searchUserByName(user1.name());

		assert(users.size() == 2); // Two users: user1, user3

		for (auto const &user : users) {
			assert(user->name() == user1.name());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() <<std::endl;
		assert(0);
	}
}

void testSearchUserByAge()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		user1.setAge(30);
		sn.addUser(user1);

		User user2("id-002", "Paul");
		user2.setAge(30);  // same age as for user1
		sn.addUser(user2);

		User user3("id-003", "John");
		user3.setAge(33);
		sn.addUser(user3);

		assert(sn.userCount() == 3);

		auto users = sn.searchUserByAge(user1.age());

		assert(users.size() == 2); // Two users: user1, user2

		for (auto const &user : users) {
			assert(user->age() == user1.age());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() <<std::endl;
		assert(0);
	}
}

void testSearchUserByHobbies()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		user1.setHobbies({"Jogging", "Football"});
		sn.addUser(user1);

		User user2("id-002", "Paul");
		user2.setHobbies({"Movies", "Jogging"});
		sn.addUser(user2);

		User user3("id-003", "John");
		user3.setHobbies({"Footbal", "Astronomy"});
		sn.addUser(user3);

		User user4("id-004", "Anna");
		user4.setHobbies({"Reading", "Astronomy"});
		sn.addUser(user4);

		User user5("id-005", "Katie");
		//user5.setHobbies({}); // no hobbies
		sn.addUser(user5);

		assert(sn.userCount() == 5);

		auto users = sn.searchUserByHobbies({"Reading", "Jogging"});

		assert(users.size() == 3); // Three users: user1, user2 and user4

		for (auto const &user : users) {
			assert(user->hobbies().count("Reading") > 0
				|| user->hobbies().count("Jogging") > 0
			);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() <<std::endl;
		assert(0);
	}
}

void testSearchUserByFriends()
{
	try {
		SocialNetwork sn;

		User user1("id-001", "John");
		user1.setFriends({"id-002", "id-005"});
		sn.addUser(user1);

		User user2("id-002", "Paul");
		user2.setFriends({"id-001", "id-003", "id-005"});
		sn.addUser(user2);

		User user3("id-003", "John");
		user3.setFriends({"id-001"});
		sn.addUser(user3);

		User user4("id-004", "Anna");
		user4.setFriends({"id-005", "id-002"});
		sn.addUser(user4);

		User user5("id-005", "Katie");
		//user5.setFriends({}); // no friends
		sn.addUser(user5);

		assert(sn.userCount() == 5);

		auto userIds = sn.getFriendsOfUser(user1); // id-002, id-005 + id-003

		assert(userIds.size() == 3); // Three users: user2, user3 and user5
		assert(userIds == std::set<std::string>({"id-002", "id-003", "id-005"}));
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() <<std::endl;
		assert(0);
	}
}

} // anonymous ns

void test()
{
	std::cout << "Running tests..." << std::endl;
	testUser_emptyName();
	testUser_emptyId();

	testDuplicates();
	testAddUser();
	testAddUser_sameNameAndId();
	testAddUser_sameId();

	testDeleteUser();

	testSearchUserByName();
	testSearchUserByAge();
	testSearchUserByHobbies();
	testSearchUserByFriends();
	std::cout << "All tests passed." << std::endl;
}

