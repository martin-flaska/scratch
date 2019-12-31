#include "Test.h"
#include "Storyboard.h"

#include <cassert>

namespace {

void testDuplicates()
{
	Storyboard sb;

	Note note1 = {"Note 1", "desc 1", {"t1", "t2"}};

	sb.addNote(note1);
	assert(sb.notes().size() == 1);

	sb.addNote(note1);
	assert(sb.notes().size() == 1);
}

void testAddNote()
{
	Storyboard sb;

	Note note1 = {"Note 1", "desc 1", {"t1"}};
	Note note2 = {"Note 2", "desc 2", {"t1"}};
	Note note3 = {"Note 3", "desc 3", {"t1"}};

	sb.addNote(note1);
	assert(sb.notes().size() == 1);
	auto notes = sb.searchByTag("t1");
	assert(notes.size() == 1);
	assert(notes.find(note1) != notes.end());

	sb.addNote(note2);
	assert(sb.notes().size() == 2);
	notes = sb.searchByTag("t1");
	assert(notes.size() == 2);
	assert(notes.find(note1) != notes.end());
	assert(notes.find(note2) != notes.end());

	sb.addNote(note3);
	assert(sb.notes().size() == 3);
	notes = sb.searchByTag("t1");
	assert(notes.size() == 3);
	assert(notes.find(note1) != notes.end());
	assert(notes.find(note2) != notes.end());
	assert(notes.find(note3) != notes.end());
}

void testDeleteNote()
{
	Storyboard sb;

	Note note1 = {"Note 1", "desc 1", {"t1"}};
	Note note2 = {"Note 2", "desc 2", {"t1"}};
	Note note3 = {"Note 3", "desc 3", {"t1"}};

	sb.addNote(note1);
	sb.addNote(note2);
	sb.addNote(note3);
	assert(sb.notes().size() == 3);

	sb.deleteNote(note2);
	assert(sb.notes().size() == 2);
	auto notes = sb.searchByTag("t1");
	assert(notes.size() == 2);
	assert(notes.find(note2) == notes.end());
	assert(notes.find(note1) != notes.end());
	assert(notes.find(note3) != notes.end());


	sb.deleteNote(note1);
	assert(sb.notes().size() == 1);
	notes = sb.searchByTag("t1");
	assert(notes.size() == 1);
	assert(notes.find(note2) == notes.end());
	assert(notes.find(note1) == notes.end());
	assert(notes.find(note3) != notes.end());

	sb.deleteNote(note3);
	assert(sb.notes().size() == 0);
	notes = sb.searchByTag("t1");
	assert(notes.size() == 0);
	assert(notes.find(note2) == notes.end());
	assert(notes.find(note1) == notes.end());
	assert(notes.find(note3) == notes.end());
}

void testSearchByTitle()
{
	Storyboard sb;

	Note note1 = {"Note 1", "desc", {"t"}};
	Note note2 = {"Note 2", "desc", {"t"}};
	Note note3 = {"Note 3", "desc", {"t"}};

	sb.addNote(note1);
	sb.addNote(note2);
	sb.addNote(note3);
	assert(sb.notes().size() == 3);

	auto notes = sb.searchByTitle(note2.title);
	assert(notes.size() == 1);
	assert(notes.find(note2) != notes.end());

	Note note4 = {"Note 2", "desc 2", {"t"}};
	assert(note4.title == note2.title);
	sb.addNote(note4);
	assert(sb.notes().size() == 4);

	notes = sb.searchByTitle(note2.title);
	assert(notes.size() == 2);
	assert(notes.find(note2) != notes.end());
	assert(notes.find(note4) != notes.end());
}

void testSearchByText()
{
	Storyboard sb;

	Note note1 = {"Note", "desc 1", {"t"}};
	Note note2 = {"Note", "desc 2", {"t"}};
	Note note3 = {"Note", "desc 3", {"t"}};

	sb.addNote(note1);
	sb.addNote(note2);
	sb.addNote(note3);
	assert(sb.notes().size() == 3);

	auto notes = sb.searchByText(note3.text);
	assert(notes.size() == 1);
	assert(notes.find(note3) != notes.end());

	Note note4 = {"Note 2", "desc 3", {"t"}};
	assert(note4.text == note3.text);
	sb.addNote(note4);
	assert(sb.notes().size() == 4);

	notes = sb.searchByText(note3.text);
	assert(notes.size() == 2);
	assert(notes.find(note3) != notes.end());
	assert(notes.find(note4) != notes.end());
}

void testSearchByTag()
{
	Storyboard sb;

	Note note1 = {"Note", "desc 1,0",  {"t1", "t0"}};
	Note note2 = {"Note", "desc 2",    {"t2"}};
	Note note3 = {"Note", "desc 3,33", {"t3", "t33"}};

	sb.addNote(note1);
	sb.addNote(note2);
	sb.addNote(note3);
	assert(sb.notes().size() == 3);

	auto notes = sb.searchByTag("t2");
	assert(notes.size() == 1);
	assert(notes.find(note2) != notes.end());

	Note note4 = {"Note", "desc 2,4", {"t2", "t4"}};
	assert(note2.tags.count("t2") == 1);
	assert(note4.tags.count("t2") == 1);
	sb.addNote(note4);
	assert(sb.notes().size() == 4);

	notes = sb.searchByTag("t2");
	assert(notes.size() == 2);
	assert(notes.find(note2) != notes.end());
	assert(notes.find(note4) != notes.end());

	////////////////////////////
	// test overloaded versions
	notes = sb.searchByTag("t3", "t1");
	assert(notes.size() == 2);
	assert(notes.find(note1) != notes.end());
	assert(notes.find(note3) != notes.end());

	notes = sb.searchByTag({"t1", "t3", "t4"});
	assert(notes.size() == 3);
	assert(notes.find(note1) != notes.end());
	assert(notes.find(note3) != notes.end());
	assert(notes.find(note4) != notes.end());
}

} // anonymous ns

void test()
{
	std::cout << "Running tests..." << std::endl;
	testDuplicates();
	testAddNote();
	testDeleteNote();
	testSearchByTitle();
	testSearchByText();
	testSearchByTag();
	std::cout << "All tests passed." << std::endl;
}

