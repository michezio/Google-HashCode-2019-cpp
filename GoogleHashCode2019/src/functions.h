#pragma once

#include <vector>
#include <string>

#include "classes.h"

using namespace std;

int calculateScore(const vector<Slide> &slideshow);
void writeOutputFile(string outputPath, const vector<Slide> &slideshow);
vector<Photo> generatePhotoList(string inputPath);
vector<Slide> generateSlideshow(vector<Photo> &photos);