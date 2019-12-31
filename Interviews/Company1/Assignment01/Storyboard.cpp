#include "Storyboard.h"


bool Note::operator==(const Note &note) const
{
	return
		note.title == title
		&& note.text == text
		&& note.tags == tags
	;
}

bool Note::operator<(const Note &note) const
{
	return std::tie(note.title, note.text, note.tags)
		< std::tie(title, text, tags)
	;
}


void Storyboard::addNote(const Note &note)
{
	auto ret = m_notes.insert(note);
	if (!ret.second) {
		return; // no insertion / duplicate
	}
	auto it = ret.first;
	m_titleMap.insert({it->title, it});
	m_textMap.insert({it->text, it});
	// Any Note item can be represented by all or even single its tag.
	// Further optimization can be done if there is need to have plenty of tags for
	// a Note.
	for (auto tag : it->tags) {
		m_tagsMap.insert({tag, it});
	}
}

void Storyboard::deleteNote(const Note &note)
{
	removeNoteMapItems(m_titleMap, note, note.title);
	removeNoteMapItems(m_textMap, note, note.text);
	for (auto const &tag : note.tags) {
		removeNoteMapItems(m_tagsMap, note, tag);
	}

	m_notes.erase(note);
}

using NoteSet = Storyboard::NoteSet;

NoteSet Storyboard::searchByTitle(const std::string &title) const
{
	return searchHelper(m_titleMap, title);
}

NoteSet Storyboard::searchByText(const std::string &text) const
{
	return searchHelper(m_textMap, text);
}

NoteSet Storyboard::searchByTag(const std::string &tag) const
{
	return searchHelper(m_tagsMap, tag);
}

NoteSet Storyboard::searchByTag(const std::set<std::string> &tags) const
{
	NoteSet s;

	for (auto const &tag : tags) {
		auto s2 = searchByTag(tag);
		// set::merge is not available until c++17...
		for (auto const &t : s2) {
			s.insert(t);
		}
	}

	return s;
}

using StringNoteMap = Storyboard::StringNoteMap;

NoteSet Storyboard::searchHelper(const StringNoteMap &map, const std::string &str) const
{
	auto range = map.equal_range(str); // Note: log complexity
	if (range.first == map.end()) {
		return NoteSet(); // none
	}
	NoteSet s;
	for (auto it = range.first; it != range.second; it++) {
		s.insert(*(it->second));
	}
	return s;
}

void Storyboard::removeNoteMapItems(StringNoteMap &map,  const Note &note, const std::string &str)
{
	auto range = map.equal_range(str); // Note: log complexity
	if (range.first != map.end()) {
		for (auto it = range.first; it != range.second; ) {
			auto n = *(it->second);
			if (n == note) {
				it = map.erase(it);
			} else {
				it++;
			}
		}
	}
}



// debug/testing purpose only
std::ostream& operator<< (std::ostream &os, const Note &n)
{
	auto const &tags = n.tags;
	os << "{" << std::quoted(n.title)
		<< ", " << std::quoted(n.text)
		<< ", {" // tags
	;
	for (auto it = tags.cbegin(); it != tags.cend(); it++) {
		os << std::quoted(*it) << ((it == --tags.cend()) ? "" : ", ");
	}
	os << "}}";
	return os;
}

// debug/testing purpose only
std::ostream& operator<<(std::ostream &os, const Storyboard::NoteSet &notes)
{
	if (notes.empty()) {
		os << "{}";
		return os;
	}

	os << "{" << std::endl;
	for (auto const &n : notes) {
		os << "\t" << n << std::endl;
	}
	os << "}";
	return os;
}

// debug/testing purpose only
std::ostream& operator<<(std::ostream &os, const Storyboard &sb)
{
	os << sb.notes();
	return os;
}
