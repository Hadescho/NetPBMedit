#include "ImageFactory.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdlib> // strtoul; 

#include "Image.h"

ImageFactory::ImageFactory(): stream(NULL)
{
}

ImageFactory::ImageFactory(std::ifstream& stream):stream(NULL) {
	if (!stream){
		throw new std::invalid_argument("Problem with the stream");
	}
	this->stream = &stream;
	initializeImage(); // Remove it from here
}

void ImageFactory::getNextString(char destination[]){
	do{
		checkStream();
		*stream >> destination;
	} while (isComment(destination) || !strcmp(destination, ""));
}

void ImageFactory::initializeImage(){
	int type;
	int width;
	int height;
	int maxVal = 1;
	char string[128];
	getNextString(string);
	// Checks type
	type = string[1] - '0';
	if (string[0] != 'P' || type < 1 || type > 6){
		throw "Invalid image format. The file should begin with \'P\' and the value after that should be between 1 and 6.";
	}
	// Get width
	getNextString(string);
	width = (int) std::strtoul(string, NULL, 0);
	if (width <= 0){
		throw "Invalid image size. Width should be greater than 0.";
	}
	// Get height
	getNextString(string);
	height = (int)std::strtoul(string, NULL, 0);
	if (height <= 0){
		throw "Invalid image size. Height should be greater than 0.";
	}
	// Get maxVal if needed
	if (type != 1 && type != 4){
		getNextString(string);
		maxVal = (int)std::strtoul(string, NULL, 0);
		if (maxVal <= 0 || maxVal > Image::MAX_VAL_MAX){
			throw "Invalid maxVal value. It should be between 1 and 65536";
		}
	}
}

bool ImageFactory::isComment(char* str){
	if (str[0] == '#'){
		while (stream->get() != '\n' || !stream->eof() || !stream->fail());
		return true;
	}
	return false;
	/*char* c = strchr(str, '#');
	if (!stream->good()){
		std::cerr << std::hex << stream->flags() << std::endl;
	}
	if (c == NULL){
		return false;
	}
	while (stream->get() != '\n');
	return true;*/
}

ImageFactory::~ImageFactory()
{
}

void ImageFactory::checkStream(){
	if (!*stream){
		throw "Find better object to throw";
	}
}
