#pragma once

/* In this exercise we want to design a Storyboard. Our version of the Storyboard
 * contains arbitrary many notes (imagine it like putting sticky notes on a board).
 * Every note has a title, a text and a set of tags. E.g.
 * 	- title: "Test Traceplayer"
 * 	- text: "Implement a unit test for the class Traceplayer of the spark core framework."
 * 	- tags: {"unit test", "traceplayer", "testing", "spark core"}
 *
 * Our Storyboard should enable us to search for notes by title, text and tags.
 * E.g.:
 *      searchByTitle("Test Traceplayer")
 *      searchByTag({"testing", "unit test"})
 *   	searchByText("Implement a unit test for the class Traceplayer of the spark core framework.")
 * For the sake of simplicity we don't want to do any similarity or prefix matching when
 * searching for a title, tag or text. Only an exact match should give results.
 *
 * The skeleton code below can be used as a starting point but doesn't have to.
 * The comments "fill in" are placeholders where you definitely have to put in some code when
 * you use this skeleton code. But this doesn't mean that you shouldn't or can't put code anywhere else.
 *
 * Also write some simple unit tests to show how you would test the functionality of the Storyboard.
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


struct Note
{
	std::string title;
	std::string text;
	std::set<std::string> tags;

	bool operator==(const Note &note) const;
	bool operator<(const Note &note) const;
};


class Storyboard
{
public:
	void addNote(const Note &note);
	void deleteNote(const Note &note);

	typedef std::set<Note> NoteSet;

 	NoteSet searchByTitle(const std::string &title) const;
	NoteSet searchByText(const std::string &text) const;
	NoteSet searchByTag(const std::string &tag) const;

	// Convenient method to allow search for multiple / non-identical tags
	// Note: For simplicity, there is no check for identical tags
	//       but the returned value is ok anyway.
	template<typename... Args>
	NoteSet searchByTag(const std::string &tag, Args... args) const
	{
		auto s = searchByTag(tag);
		auto s2 = searchByTag(args...);
		// set::merge is not available until c++17...
		for (auto const &t : s2) {
			s.insert(t);
		}
		return s;
	}

	NoteSet searchByTag(const std::set<std::string> &tags) const;

	// debug/testing purpose only
	const NoteSet & notes() const { return m_notes; }
	using StringNoteMap = std::multimap<std::string, NoteSet::const_iterator>;

private:
	NoteSet searchHelper(const StringNoteMap &map, const std::string &str) const;

	/** helper method to remove all @map items related to @note
	 *
	 * @Note std::erase_if could be used for c++20 with the same predicate for all maps...
	 */
	void removeNoteMapItems(StringNoteMap &map,  const Note &note, const std::string &str);

	// To have variadic searchByTag
	NoteSet searchByTag() const { return NoteSet();	}

private:
	NoteSet m_notes;
	// maps value to note set item
	StringNoteMap m_titleMap;
	StringNoteMap m_textMap;
	StringNoteMap m_tagsMap;
};


// debug/testing purpose only
std::ostream& operator<< (std::ostream &os, const Note &n);
std::ostream& operator<<(std::ostream &os, const Storyboard::NoteSet &notes);
std::ostream& operator<<(std::ostream &os, const Storyboard &sb);
