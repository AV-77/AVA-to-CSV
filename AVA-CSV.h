#pragma once

#include <iostream>
#include <stdlib.h>
#include <boost\filesystem.hpp>

class spectrum
{
public:
	std::string sFileLocation;
	std::string sFilename;
	std::string sData;
	float fSlope;
	float fOffset;

	spectrum(boost::filesystem::path);
	void setXMLData();
	void setCalibration(float, float);
	int createCSV();
	float calibration(int);
};