#include <algorithm>
#include <unordered_map>
#include <sstream>

#include "classes.h"

using namespace std;


unordered_map<string, ULONG> taglist;

ULONG tag2code(string tag)
{
	ULONG code = (ULONG)taglist.size();
	unordered_map<string, ULONG>::iterator it = taglist.find(tag);
	if (it == taglist.end()) taglist[tag] = code;
	else code = taglist[tag];
	return code;
}


Photo::Photo(ULONG id, string line)
{
	this->id = id;
	used = false;
	stringstream tokenizer(line);
	string token;
	getline(tokenizer, token, ' ');
	isHorizontal = (token == "H");
	getline(tokenizer, token, ' ');
	UINT tags_length = stoi(token);
	tags.reserve(tags_length);
	while (getline(tokenizer, token, ' '))
	{
		ULONG code = tag2code(token);
		tags.insert(lower_bound(tags.begin(), tags.end(), code), code);
	} 
}


Slide::Slide(const Photo &photo) : tags(photo.tags)
{
	this->photo1_id = photo.id;
	this->photo2_id = -1;
	this->isHorizontal = photo.isHorizontal;
	this->points = 0;
}

Slide::Slide(const Photo &p1, const Photo &p2) : Slide::Slide(p1)
{
	this->addVertical(p2);
}

void Slide::addVertical(const Photo &photo)
{
	this->photo2_id = photo.id;
	this->mergeTags(photo.tags);
}

V_ULONG Slide::mergeTags(const V_ULONG t1, const V_ULONG t2)
{
	V_ULONG merged;
	merged.reserve(t1.size() + t2.size());
	UINT index_a = 0;
	UINT index_b = 0;
	while (index_a < t1.size() && index_b < t2.size())
	{
		ULONG a = t1[index_a];
		ULONG b = t2[index_b];
		if (a <= b) {
			merged.push_back(a);
			index_a++;
			if (a == b) index_b++;
		}
		else {
			merged.push_back(b);
			index_b++;
		}
	}
	while (index_a < t1.size())
		merged.push_back(t1[index_a++]);
	while (index_b < t2.size())
		merged.push_back(t2[index_b++]);
	return merged;
}

inline void Slide::mergeTags(V_ULONG array)
{
	this->tags = Slide::mergeTags(this->tags, array);
}

int Slide::intersectionSize(const V_ULONG v1, const V_ULONG v2)
{
	UINT index_a = 0;
	UINT index_b = 0;
	UINT equals = 0;
	while (index_a < v1.size() && index_b < v2.size())
	{
		ULONG a = v1[index_a];
		ULONG b = v2[index_b];
		if (a <= b) {
			index_a++;
			if (a == b) {
				index_b++;
				equals++;
			}
		}
		else {
			index_b++;
		}
	}
	return equals;
}

inline int Slide::intersectionSize(V_ULONG array) const
{
	return Slide::intersectionSize(this->tags, array);
}

int Slide::preview(const Photo &photo, const Slide &slide)
{
	return Slide::score(Slide::mergeTags(this->tags, photo.tags), slide.tags);
}

int Slide::score(const V_ULONG t1, const V_ULONG t2)
{
	UINT int_size = Slide::intersectionSize(t1, t2);
	if (int_size == 0) return 0;
	UINT min_size = (UINT)(min(t1.size(), t2.size()));
	return min(int_size, min_size - int_size);
}

inline int Slide::pointsTo(const Slide &slide) const
{
	return Slide::score(this->tags, slide.tags);
}

string Slide::toString() const
{
	string s = to_string(photo1_id);
	if (photo2_id != -1)
		s += " " + to_string(photo2_id);
	return s;
}