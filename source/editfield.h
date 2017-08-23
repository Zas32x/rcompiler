#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <string>
#include "resources.h"

class SpriteEdit:public Fl_Group
{
	private:
	Resource *resource;
	Fl_Button *del;
	Fl_Input *name;
	Fl_Input *file;
	Fl_Check_Button *smooth;

	public:
	SpriteEdit(Resource *res,int xp,int yp,int width,int height);
	void remove();
	void changed();
	void changedSmooth();
	static void cbRemove(Fl_Widget *widget,void *data)
	{
		((SpriteEdit*)data)->remove();
	}
	static void cbChanged(Fl_Widget *widget,void *data)
	{
		((SpriteEdit*)data)->changed();
	}
	static void cbChangedSmooth(Fl_Widget *widget,void *data)
	{
		((SpriteEdit*)data)->changedSmooth();
	}
};

class FontEdit:public Fl_Group
{
	private:
	Resource *resource;
	Fl_Button *del;
	Fl_Input *name;
	Fl_Input *file;
	Fl_Int_Input *characters;
	Fl_Int_Input *size;
	Fl_Check_Button *smooth;

	public:
	FontEdit(Resource *res,int xp,int yp,int width,int height);
	void remove();
	void changed();
	void changedSmooth();
	void changedSize();
	void changedChars();
	static void cbRemove(Fl_Widget *widget,void *data)
	{
		((FontEdit*)data)->remove();
	}
	static void cbChanged(Fl_Widget *widget,void *data)
	{
		((FontEdit*)data)->changed();
	}
	static void cbChangedSmooth(Fl_Widget *widget,void *data)
	{
		((FontEdit*)data)->changedSmooth();
	}
	static void cbChangedSize(Fl_Widget *widget,void *data)
	{
		((FontEdit*)data)->changedSize();
	}
	static void cbChangedChars(Fl_Widget *widget,void *data)
	{
		((FontEdit*)data)->changedChars();
	}
};

class SoundEdit:public Fl_Group
{
	private:
	Resource *resource;
	Fl_Button *del;
	Fl_Input *name;
	Fl_Input *file;
	Fl_Check_Button *streamed;

	public:
	SoundEdit(Resource *res,int xp,int yp,int width,int height);
	void remove();
	void changed();
	static void cbRemove(Fl_Widget *widget,void *data)
	{
		((SoundEdit*)data)->remove();
	}
	static void cbChanged(Fl_Widget *widget,void *data)
	{
		((SoundEdit*)data)->changed();
	}
};

class RawEdit:public Fl_Group
{
	private:
	Resource *resource;
	Fl_Button *del;
	Fl_Input *name;
	Fl_Input *file;

	public:
	RawEdit(Resource *res,int xp,int yp,int width,int height);
	void remove();
	void changed();
	static void cbRemove(Fl_Widget *widget,void *data)
	{
		((RawEdit*)data)->remove();
	}
	static void cbChanged(Fl_Widget *widget,void *data)
	{
		((RawEdit*)data)->changed();
	}
};
