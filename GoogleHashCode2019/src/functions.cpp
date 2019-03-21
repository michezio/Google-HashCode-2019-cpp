#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "functions.h"

#define PRINT_PROGRESS(x, y) cout << fixed << setprecision(2) << 100.0f * (x) / (y) << "%\r"
#define CLEAR_LINE cout << "\b\b\b\b\b\b\b\r"
#define LOG(x) cout << (x) << endl


using namespace std;


vector<Photo> generatePhotoList(const string &inputPath)
{
	int processed = 0;
	ifstream inputFile(inputPath);
	if (!inputFile.is_open())
	{
		LOG("!!! FILE NOT FOUND !!!");
		throw uncaught_exception;
	}
	string line;
	getline(inputFile, line);
	ULONG number_of_photos = stoi(line);
	vector<Photo> photos;
	photos.reserve(number_of_photos);
	for (ULONG i = 0; i < number_of_photos; ++i)
	{
		getline(inputFile, line);
		photos.push_back(Photo(i, line));
		PRINT_PROGRESS(processed, number_of_photos);
		processed++;
	}
	inputFile.close();
	sort(photos.begin(), photos.end(),
		[](Photo const &L, Photo const &R)
		{ 
			return L.tags.size() <= R.tags.size();
		}
	);
	CLEAR_LINE;
	LOG("INPUT ACQUIRED. PROCESSING...");
	return photos;
}

vector<Slide> generateSlideshow(vector<Photo> &photos)
{
	for (ULONG i = 0; i < photos.size(); ++i)
	{
		photos[i].used = false;
	}
	ULONG processed = 0;
	vector<Slide> slideshow;
	slideshow.reserve(photos.size());
	Slide *last = nullptr;
	for (ULONG i = 0; i < photos.size(); ++i)
	{
		if (photos[i].isHorizontal)
		{
			last = new Slide(photos[i]);
			photos[i].used = true;
			processed++;
			break;
		}
	}
	if (last == nullptr)
	{
		last = new Slide(photos[0]);
		photos[0].used = true;
		last->addVertical(photos[1]);
		photos[1].used = true;
		processed += 2;
	}
	slideshow.push_back(*last);
	while (processed < photos.size())
	{
		PRINT_PROGRESS(processed, photos.size());
		int points = -1;
		Photo *selected = nullptr;
		for (unsigned int i = 0; i < photos.size(); ++i)
		{
			if (photos[i].used)
				continue;
			int new_points = Slide::score(last->tags, photos[i].tags);
			if (new_points > points)
			{
				points = new_points;
				selected = &photos[i];
			}
		}
		if (selected)
		{
			last = new Slide(*selected);
			selected->used = true;
			if (selected->isHorizontal)
				processed++;
			else
			{
				Photo *match = nullptr;
				int best_delta = -1000;
				for (unsigned int i = 0; i < photos.size(); ++i)
				{
					if (photos[i].used || photos[i].isHorizontal)
						continue;
					int delta = last->preview(photos[i], slideshow[slideshow.size() - 1]) - points;
					if (delta > best_delta)
					{
						best_delta = delta;
						match = &photos[i];
					}
				}
				if (match)
				{
					points += best_delta;
					last->addVertical(*match);
					match->used = true;
					processed += 2;
				}
			}
			slideshow[slideshow.size() - 1].points = points;
			slideshow.push_back(*last);
		}
	}
	CLEAR_LINE;
	LOG("DONE.");
	return slideshow;
}



int calculateScore(const vector<Slide> &slideshow)
{
	int score = 0;
	for (unsigned int i = 0; i < slideshow.size()-1; ++i)
	{
		score += slideshow[i].points;
		//score += slideshow[i].pointsTo(slideshow[i + 1]);
	}
	return score;
}

void writeOutputFile(const string &outputPath, const vector<Slide> &slideshow)
{
	ofstream outputFile(outputPath);
	outputFile << slideshow.size() << endl;
	for (unsigned int i = 0; i < slideshow.size(); ++i)
	{
		outputFile << slideshow[i].toString() << endl;
	}
	outputFile.close();
}