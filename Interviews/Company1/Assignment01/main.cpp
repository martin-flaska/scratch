#include <iostream>

#include "Storyboard.h"
#include "Test.h"

using namespace std;


namespace {

void demo()
{
	Storyboard sb;

	sb.addNote({"Note 1", "desc 1", {"t1", "t2"}});
	sb.addNote({"Note 2", "desc 2", {"t2", "t3"}});
	sb.addNote({"Note 2", "desc 2", {"t2", "t3"}});
	sb.addNote({"Note 3a", "desc 3", {"t3", "t4"}});
	sb.addNote({"Note 3b", "desc 3", {"t3", "t4"}});
	sb.addNote({"Note 2", "desc 2", {"t2", "t3"}});
	sb.addNote({"Note 2x", "desc 2x-1", {"t2", "t3"}});
	sb.addNote({"Note 2x", "desc 2x-2", {"t2", "t3"}});
	std::cout << "Storyboard: " << sb << std::endl;

	std::cout << R"(Removing note: {"Note 2", "desc 2", {"t2", "t3"}})" << std::endl;
	sb.deleteNote({"Note 2", "desc 2", {"t2", "t3"}});
	std::cout << "Storyboard: " << sb << std::endl;

	std::cout << R"(Removing non-existing note: {"Note xy", "desc xy", {"tx", "ty"}})" << std::endl;
	sb.deleteNote({"Note xy", "desc xy", {"tx", "ty"}});
	std::cout << "Storyboard: " << sb << std::endl;

	auto notes = sb.searchByTitle("Note 2x");
	std::cout << "Notes with title: \"Note 2x\"" << notes << std::endl;

	notes = sb.searchByTitle("Note 2");
	std::cout << "Notes with title: \"Note 2\"" << notes << std::endl;

	notes = sb.searchByText("desc 3");
	std::cout << "Notes with text: \"desc 3\"" << notes << std::endl;

	notes = sb.searchByTag("t2");
	std::cout << "Notes with tag: \"t2\"" << notes << std::endl;

	notes = sb.searchByTag("t2", "t4");
	std::cout << "Notes with tags: \"t2\", \"t4\"" << notes << std::endl;

	notes = sb.searchByTag({"t1", "t2", "t3", "t4"});
	std::cout << R"(Notes with tags: "t1", "t2", "t3", "t4")" << notes << std::endl;
}

} // anonymous ns

int main(int argc, char **argv)
{
	std::cout << "Assignment 1 ..." << std::endl;
	test();
	demo();
    return 0;
}
