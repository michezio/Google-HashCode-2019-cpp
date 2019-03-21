#include <iostream>
#include <vector>
#include <string>

#include "functions.h"
#include "classes.h"

#define OUT_FOLDER "E:\\out\\"
#define IN_FOLDER "..\\input\\"

using namespace std;

string getFileName(char letter)
{
	string name;
	switch (letter) {
	case 'a': name = "a_example"; break;
	case 'b': name = "b_lovely_landscapes"; break;
	case 'c': name = "c_memorable_moments"; break;
	case 'd': name = "d_pet_pictures"; break;
	case 'e': name = "e_shiny_selfies"; break;
	}
	return name;
}

int main(int argc, char *argv[])
{

	string long_letter;
	string inputFolder;
	string outputFolder;

	cout << endl << "Select input file (only first letter) -> ";
	cin >> long_letter;
	cout << "Path of the input folder -> ";
	cin >> inputFolder;
	cout << "Path of the output folder -> ";
	cin >> outputFolder;

	if (inputFolder == "0") inputFolder = IN_FOLDER;
	if (outputFolder == "0") outputFolder = OUT_FOLDER;

	string fileName = getFileName(long_letter[0]);

	string inputPath = inputFolder + fileName + ".txt";

	vector<Photo> photos = generatePhotoList(inputPath);

	vector<Slide> slideshow = generateSlideshow(photos);

	int score = calculateScore(slideshow);

	cout << "Found solution with score " << score << endl;

	string outputPath = outputFolder + fileName + "_out_" + to_string(score) + ".txt";

	writeOutputFile(outputPath, slideshow);

	cin.get();
}

