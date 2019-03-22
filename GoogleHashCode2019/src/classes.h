#pragma once

#include <string>
#include <vector>

#include <iostream>

#define UINT unsigned int
#define ULONG unsigned long
#define V_ULONG vector<ULONG>

using namespace std;


class Photo
{
public:
	ULONG id;
	bool isHorizontal;
	V_ULONG tags;
	bool used;
	Photo(ULONG id, const string &line);
};

class Slide
{
public:
	UINT points;
	bool isHorizontal;
	V_ULONG tags;
	Slide(const Photo &photo);
	Slide(const Photo &p1, const Photo &p2);
	void addVertical(const Photo &photo);
	inline int pointsTo(const Slide &slide) const;
	int preview(const Photo &photo, const Slide &slide);
	static UINT previewScore(const Photo &p1, const Photo &p2, const Slide &s);
	static int preview(const Photo &p1, const Photo &p2, const Slide &s);
	static int score(const V_ULONG &t1, const V_ULONG &t2);
	string toString() const;

private:
	ULONG photo1_id;
	ULONG photo2_id;
	void mergeTags(const V_ULONG &array);
	static V_ULONG mergeTags(const V_ULONG &t1, const V_ULONG &t2);
	inline int intersectionSize(const V_ULONG &array) const;
	static int intersectionSize(const V_ULONG &v1, const V_ULONG &v2);
};