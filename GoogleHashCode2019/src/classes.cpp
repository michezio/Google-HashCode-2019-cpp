#include <algorithm>
#include <unordered_map>
#include <sstream>

#include "classes.h"

using namespace std;


unordered_map<string, ULONG> taglist;

ULONG tag2code(const string &tag)
{
	ULONG code = (ULONG)taglist.size();
	unordered_map<string, ULONG>::iterator it = taglist.find(tag);
	if (it == taglist.end()) taglist[tag] = code;
	else code = taglist[tag];
	return code;
}


Photo::Photo(ULONG id, const string &line)
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

V_ULONG Slide::mergeTags(const V_ULONG &t1, const V_ULONG &t2)
{
	V_ULONG merged;
	merged.reserve(t1.size() + t2.size());
	UINT index_a = 0;
	UINT index_b = 0;
	while (index_a < t1.size() && index_b < t2.size())
	{
		ULONG a = t1[index_a];
		ULONG b = t2[index_b];
		if (a <= b)
		{
			merged.push_back(a);
			index_a++;
			if (a == b) index_b++;
		}
		else
		{
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

inline void Slide::mergeTags(const V_ULONG &array)
{
	this->tags = Slide::mergeTags(this->tags, array);
}

int Slide::intersectionSize(const V_ULONG &v1, const V_ULONG &v2)
{
	UINT index_a = 0;
	UINT index_b = 0;
	UINT equals = 0;
	while (index_a < v1.size() && index_b < v2.size())
	{
		ULONG a = v1[index_a];
		ULONG b = v2[index_b];
		if (a <= b)
		{
			index_a++;
			if (a == b)
			{
				index_b++;
				equals++;
			}
		}
		else
		{
			index_b++;
		}
	}
	return equals;
}

inline int Slide::intersectionSize(const V_ULONG &array) const
{
	return Slide::intersectionSize(this->tags, array);
}

int Slide::preview(const Photo &photo, const Slide &slide)
{
	return Slide::score(Slide::mergeTags(this->tags, photo.tags), slide.tags);
}

int Slide::preview(const Photo &p1, const Photo &p2, const Slide &s)
{
	V_ULONG merged = Slide::mergeTags(p1.tags, p2.tags);
	UINT int_size = Slide::intersectionSize(merged, s.tags);
	if (int_size == 0) return 0;
	UINT min_size = (UINT)(min(merged.size(), s.tags.size()));
	return min(int_size, min_size - int_size);
}

UINT Slide::previewScore(const Photo &p1, const Photo &p2, const Slide &s)
{
	UINT index_a = 0;
	UINT index_b = 0;
	UINT index_c = 0;
	UINT size = 0;
	UINT equals = 0;
	while (index_a < p1.tags.size() && index_b < p2.tags.size() && index_c < s.tags.size())
	{
		ULONG a = p1.tags[index_a];
		ULONG b = p2.tags[index_b];
		ULONG selected;
		if (a <= b)
		{
			selected = a;
			index_a++;
			if (a == b) index_b++;
		}
		else
		{
			selected = b;
			index_b++;
		}
		size++;

		while (s.tags[index_c] < selected && index_c < s.tags.size())
			index_c++;
		if (s.tags[index_c] == selected) equals++;
	}
	while (index_a < p1.tags.size())
	{
		size++;
		while (s.tags[index_c] < p1.tags[index_a] && index_c < s.tags.size())
			index_c++;
		if (s.tags[index_c] == p1.tags[index_a]) equals++;
		index_a++;
	}
	while (index_b < p2.tags.size())
	{
		size++;
		while (s.tags[index_c] < p2.tags[index_b] && index_c < s.tags.size())
			index_c++;
		if (s.tags[index_c] == p2.tags[index_b]) equals++;
		index_b++;
	}
	return min(equals, min(size, s.tags.size()) - equals);
}

int Slide::score(const V_ULONG &t1, const V_ULONG &t2)
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