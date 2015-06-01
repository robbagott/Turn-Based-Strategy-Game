#ifndef APPINFO_H
#define APPINFO_H

#include <SFML/Window.hpp>
#include <string>

class AppInfo
{
public:
	AppInfo(std::string configFilename);

	enum ScreenMode { SM_FULLSCREEN, SM_BORDERLESSWINDOW, SM_WINDOW };

	int windowWidth();
	int windowHeight();
	int resx();
	int resy();
	ScreenMode screenMode();
	bool splashEnabled();
	bool stretchToFitEnabled();

private:

	int m_windowWidth, m_windowHeight;
	bool m_splashEnabled;
	bool m_stretchToFit;
	ScreenMode m_screenMode;
	int m_resx = 256, m_resy = 144;
};

#endif