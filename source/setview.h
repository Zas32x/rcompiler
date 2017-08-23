#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Select_Browser.H>
#include <string>
#include "resources.h"

class SetView:Fl_Select_Browser
{
	private:

	public:
	static SetView *instance;
	SetView(int x,int y,int width,int height,const char* label);
	virtual ~SetView();
	void addItem(const char* name);
	void addItem(Set *set);
	void click();
	void removeItem(int id);
	void eraseAll();
	void resetSelection();
	int getValue()
	{
		return value();
	}
	Set* getSet(int id)
	{
		return id<(int)size()?(Set*)data(id+1):NULL;
	}
	Set* getSet()
	{
		return getSet(value()-1);
	}
	unsigned int size()
	{
		return Fl_Select_Browser::size();
	}
	void setSelection(int id);
	void renameItem(const std::string &name,int id);
	static void cbClick(Fl_Widget *widget,void*);
};
