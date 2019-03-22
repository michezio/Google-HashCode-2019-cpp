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
		photos.emplace_back(i, line);
		PRINT_PROGRESS(processed, number_of_photos);
		processed++;
	}
	inputFile.close();
	sort(photos.begin(), photos.end(),
		[](Photo const &L, Photo const &R)
		{ 
			return L.tags.size() < R.tags.size();
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
	Photo *ph1 = nullptr;
	Photo *ph2 = nullptr;
	for (ULONG i = 0; i < photos.size(); ++i)
	{
		if (photos[i].isHorizontal)
		{
			ph1 = &photos[i];
			photos[i].used = true;
			processed++;
			slideshow.emplace_back(*ph1);
			break;
		}
	}
	if (ph1 == nullptr)
	{
		ph1 = &photos[0];
		photos[0].used = true;
		ph2 = &photos[1];
		photos[1].used = true;
		processed += 2;
		slideshow.emplace_back(*ph1, *ph2);
	}
	while (processed < photos.size())
	{
		PRINT_PROGRESS(processed, photos.size());
		int points = -1;
		Slide *last = &slideshow.at(slideshow.size() - 1);
		ph1 = ph2 = nullptr;
		for (ULONG i = 0; i < photos.size(); ++i)
		{
			if (photos[i].used)
				continue;
			int new_points = Slide::score(last->tags, photos[i].tags);
			if (new_points > points)
			{
				points = new_points;
				ph1 = &photos[i];
			}
		}
		if (ph1)
		{
			ph1->used = true;
			if (ph1->isHorizontal)
			{
				slideshow.emplace_back(*ph1);
				processed++;
			}
			else
			{
				int best_delta = -1000;
				for (UINT i = 0; i < photos.size(); ++i)
				{
					if (photos[i].used || photos[i].isHorizontal)
						continue;
					int delta = Slide::previewScore(*ph1, photos[i], slideshow[slideshow.size() - 1]) - points;
					if (delta > best_delta)
					{
						best_delta = delta;
						ph2 = &photos[i];
					}
				}
				if (ph2)
				{
					points += best_delta;
					ph2->used = true;
					slideshow.emplace_back(*ph1, *ph2);
					processed += 2;
				}
			}
			slideshow[slideshow.size() - 1].points = points;
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