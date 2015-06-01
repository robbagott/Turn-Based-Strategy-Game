#include "AppInfo.h"
#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include <cassert>
#include "stdAfx.h"

using namespace std;

AppInfo::AppInfo(std::string configFilename)
{
	//set defaults
	m_windowWidth = 256;
	m_windowHeight = 144;
	m_screenMode = SM_WINDOW;
	m_splashEnabled = true;
	m_stretchToFit = false;

	//set from config file
	ifstream configStream("config.txt");
	stringstream ss;
	string valueStr = "";
	while (configStream.good())
	{
		getline(configStream, valueStr);

		//if the config file contains the screen_width setting, set appropriately
		if (valueStr.find("screen_width") != string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_windowWidth;
			ss.str("");
			ss.clear();
		}

		//Do same for screen_height
		else if (valueStr.find("screen_height") != string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_windowHeight;
			ss.str("");
			ss.clear();
		}

		else if (valueStr.find("fullscreen") != string::npos) {
			string value = "";

			ss.str(valueStr);
			ss >> value;
			ss >> value;
			ss.str("");
			ss.clear();

			if (value == "1") {
				m_screenMode = SM_FULLSCREEN;
			}
			else if (value == "2") {
				m_screenMode = SM_BORDERLESSWINDOW;
			}
			else {
				m_screenMode = SM_WINDOW;
			}
		}

		else if (valueStr.find("show_splash") != string::npos) {
			string value = "";

			ss.str(valueStr);
			ss >> valueStr;
			ss >> value;
			ss.str("");
			ss.clear();

			if (value != "0") {
				m_splashEnabled = true;
			}
			else {
				m_splashEnabled = false;
			}
		}

		else if (valueStr.find("resolution_x") != string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_resx;
			ss.str("");
			ss.clear();
		}

		else if (valueStr.find("resolution_y") != string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_resy;
			ss.str("");
			ss.clear();
		}

		else if (valueStr.find("stretch_to_fit") != string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_stretchToFit;
			ss.str("");
			ss.clear();
		}
	}

#ifdef DEBUG
	std::cout << "width: "<< m_windowWidth 
		<< ", height: " << m_windowHeight
		<< ", screen mode: " << m_screenMode 
		<< ", showing splash: " << m_splashEnabled << std::endl;
#endif
}

int AppInfo::windowWidth() {
	return m_windowWidth;
}

int AppInfo::windowHeight() {
	return m_windowHeight;
}

AppInfo::ScreenMode AppInfo::screenMode() {
	return m_screenMode;
}

bool AppInfo::splashEnabled() {
	return m_splashEnabled;
}

int AppInfo::resx() {
	return m_resx;
}

int AppInfo::resy() {
	return m_resy;
}

bool AppInfo::stretchToFitEnabled() {
	return m_stretchToFit;
}