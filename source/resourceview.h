#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Tabs.H>
#include "resources.h"
#include <vector>
#include "editfield.h"

class ResourceView:Fl_Group
{
	private:
	Fl_Scroll *sprites,*fonts,*sounds,*raw;
	Fl_Tabs *tabs;
	Fl_Group *gSprites,*gFonts,*gSounds,*gRaw;
	std::vector<SpriteEdit*> eSprite;
	std::vector<FontEdit*> eFont;
	std::vector<SoundEdit*> eSound;
	std::vector<RawEdit*> eRaw;

	public:
	static ResourceView *instance;
	ResourceView(int xp,int yp,int width,int height,const char* label);
	virtual ~ResourceView();
	void addItem(Resource *res,bool select=0);
	void moveUp(int type,Fl_Widget *who);
	void scrollTo(Fl_Scroll *scroll,Fl_Widget *to,int amount);
	void scrollCheck(Fl_Scroll *scroll,int amount);
	void eraseAll();
	int getTab();
	void tabChange();
	void resetSelection();

	void fillWith(Set *set);
	static void cbTabChange(Fl_Widget *widget,void *who)
	{
		((ResourceView*)who)->tabChange();
	}
};
