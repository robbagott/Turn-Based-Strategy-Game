#ifndef APPINFO_H
#define APPINFO_H

#include <SFML/Window.hpp>

class AppInfo
{
public:
	AppInfo();

	enum ScreenMode { SM_FULLSCREEN, SM_BORDERLESSWINDOW, SM_WINDOW };

	int windowWidth() const;
	int windowHeight() const;
	int resx() const;
	int resy() const;
	ScreenMode screenMode() const;
	bool splashEnabled() const;
	bool stretchToFitEnabled() const;
	int centerScreenx();
	int centerScreeny();
	int tileSize();

private:
	AppInfo(const AppInfo& other);
	void operator=(const AppInfo& other);

	int m_windowWidth, m_windowHeight;
	bool m_splashEnabled;
	bool m_stretchToFit;
	ScreenMode m_screenMode;

	//internal resolution. Should not be taken from config file
	int m_resx, m_resy;
	int m_tileSize;
};

#endif