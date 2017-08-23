#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <string>

class NameWindow:public Fl_Window
{
	private:
	Fl_Button *bOk;
	Fl_Input *iName;

	public:
	static NameWindow *instance;
	NameWindow(int width,int height,const char *title,const std::string &contents);
	virtual ~NameWindow();
	void ok(),close();
	static void cbOk(Fl_Widget *widget,void*)
	{
		instance->ok();
	}
	static void cbClose(Fl_Widget *widget,void*)
	{
		instance->close();
	}
};
