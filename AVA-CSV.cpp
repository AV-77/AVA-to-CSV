#include "AVA-CSV.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>


int main(int argc, char* argv[])
{
	boost::filesystem::path filepath;
	if (argc < 2)
	{
		filepath = boost::filesystem::current_path();
	}
	else
	{
		filepath = argv[1];
	}
	if (boost::filesystem::exists(filepath))
	{
		if (boost::filesystem::is_directory(filepath))
		{
			boost::filesystem::directory_iterator dirIter{ filepath };
			for (dirIter; dirIter != boost::filesystem::directory_iterator{}; *dirIter++)
			{
				boost::filesystem::path currentFile = *dirIter;
				if (boost::filesystem::is_regular_file(currentFile))
				{
					if (currentFile.extension() == ".ava")
					{
						spectrum newSpec(currentFile);
						if (newSpec.createCSV())
							std::cout << "file created" << std::endl;
					}
				}
			}
		}
		if (boost::filesystem::is_regular_file(filepath))
		{
			if (filepath.extension() == ".ava")
			{
				spectrum newSpec(filepath);
				if (newSpec.createCSV())
					std::cout << "file created" << std::endl;
			}
		}
	}
}

spectrum::spectrum(boost::filesystem::path fileLocation)
{
	this->sFileLocation = fileLocation.string();
	this->sFilename = boost::filesystem::change_extension(fileLocation.filename(), ".csv").string();
	this->setXMLData();
}

void spectrum::setXMLData()
{
	boost::property_tree::ptree pt1;
	boost::property_tree::read_xml(this->sFileLocation, pt1);


	float slope = 0.0;
	float offset = 0.0;
	this->sData = pt1.get<std::string>("mcadata.spectrum");
	BOOST_FOREACH(boost::property_tree::ptree::value_type const& node, pt1.get_child("mcadata"))
	{
		boost::property_tree::ptree attr = node.second;
		if (node.first == "parameter")
		{
			int code = std::stoi(attr.get<std::string>("<xmlattr>.code"));

			switch (code) 
			{
			case 50:
				slope = std::stof(attr.get<std::string>("<xmlattr>.value"));
				break;
			case 49:
				offset = std::stof(attr.get<std::string>("<xmlattr>.value"));
				break;
			default:
				break;
			}
		}
	}
	this->setCalibration(slope, offset);
}

int spectrum::createCSV()
{
	std::istringstream iss;

	std::ofstream outputFile(this->sFilename);
	if (outputFile.is_open())
	{
		outputFile << "File Name, " << this->sFilename << std::endl;
		outputFile << "Calibration, " << this->fSlope << "Ch + " << this->fOffset << std::endl;
		outputFile << "Energy, Counts" << std::endl;
		std::string val;
		iss.str(this->sData);
		for (int i = 0; i < 2048; i++)
		{
			std::string channel = std::to_string(this->calibration(i)) + ", ";
				iss >> val;
			outputFile << channel << val << std::endl;
		}
		outputFile.close();
	}
	else
	{
		std::cout << "failed to open file" << std::endl;
		return -1;
	}
	return 1;
}

void spectrum::setCalibration(float slope, float offset)
{
	this->fSlope = slope;
	this->fOffset = offset;
}

float spectrum::calibration(int channel)
{
	return this->fSlope * channel + this->fOffset;
}