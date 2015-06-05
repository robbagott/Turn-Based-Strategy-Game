#include "AppInfo.h"
#include <fstream>
#include <sstream>
#include "stdAfx.h"


AppInfo::AppInfo() {
	//set defaults
	m_windowWidth = 256;
	m_windowHeight = 144;
	m_resx = 256;
	m_resy = 144;
	m_screenMode = SM_WINDOW;
	m_splashEnabled = true;
	m_stretchToFit = false;

	//set from EXPECTED config file
	std::ifstream configStream("config.txt");
	std::stringstream ss;
	std::string valueStr = "";
	while (configStream.good())
	{
		getline(configStream, valueStr);

		//if the config file contains the screen_width setting, set appropriately
		if (valueStr.find("screen_width") != std::string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_windowWidth;
			ss.str("");
			ss.clear();
		}

		//Do same for screen_height
		else if (valueStr.find("screen_height") != std::string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_windowHeight;
			ss.str("");
			ss.clear();
		}

		else if (valueStr.find("fullscreen") != std::string::npos) {
			std::string value = "";

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

		else if (valueStr.find("show_splash") != std::string::npos) {
			std::string value = "";

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

		else if (valueStr.find("resolution_x") != std::string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_resx;
			ss.str("");
			ss.clear();
		}

		else if (valueStr.find("resolution_y") != std::string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_resy;
			ss.str("");
			ss.clear();
		}

		else if (valueStr.find("stretch_to_fit") != std::string::npos) {
			ss.str(valueStr);
			ss >> valueStr;
			ss >> m_stretchToFit;
			ss.str("");
			ss.clear();
		}
	}

#ifdef DEBUG

	std::cout << "width: " << m_windowWidth
		<< ", height: " << m_windowHeight
		<< ", screen mode: " << m_screenMode
		<< ", showing splash: " << m_splashEnabled << std::endl;
#endif
}

int AppInfo::windowWidth() const {
	return m_windowWidth;
}

int AppInfo::windowHeight() const {
	return m_windowHeight;
}

AppInfo::ScreenMode AppInfo::screenMode() const	{
	return m_screenMode;
}

bool AppInfo::splashEnabled() const {
	return m_splashEnabled;
}

int AppInfo::resx() const {
	return m_resx;
}

int AppInfo::resy() const {
	return m_resy;
}

bool AppInfo::stretchToFitEnabled() const {
	return m_stretchToFit;
}

int AppInfo::centerScreenx() {
	return m_resx / 2;
}

int AppInfo::centerScreeny(){
	return m_resy / 2;
}