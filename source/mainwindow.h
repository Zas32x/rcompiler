#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scroll.H>
#include "setview.h"
#include "resourceview.h"
#include "resources.h"
#include <vector>
#include <string>

class MainWindow:public Fl_Window
{
	private:
	Fl_Button *bGenerate,*bSave,*bLoad,*bSaveAs,*bAddSet,*bRemoveSet;
	Fl_Button *bAddSprite,*bAddSound,*bAddFont,*bAddRaw;
	Fl_Check_Button *cbCompile;
	SetView *setView;
	ResourceView *resourceView;
	void addSet(),removeSet();
	void addSprite(),addFont(),addSound(),addRaw();
	bool prevSmooth;
	int prevSize,prevChars;
	int renaming;
	bool saved;
	std::string lastDir;
	std::string lastSave;
	std::string workDir,setFile;
	void generateData(const std::string &folder);
	void saveFile(const std::string &filename);
	bool loadFile(const std::string &filename);
	void saveSettings(const std::string &filename);
	bool loadSettings(const std::string &filename);

	public:
	static MainWindow *instance;
	void checkAddButton();
	void selectedSet(int id);
	void renameSet(int id);
	void nameAdded(const std::string &str);
	void generate();
	void save(),saveAs(),close();
	bool load();
	void loadSet(Set *set);
	void loadResource(Set *set,Resource *res);
	
	inline void updateSmooth(bool sm)
	{
		prevSmooth=sm;
	}
	inline void updateSize(int size)
	{
		prevSize=size;
	}
	inline void updateChars(int chars)
	{
		prevChars=chars;
	}
	static void cbAddSet(Fl_Widget *widget,void*)
	{
		instance->addSet();
	}
	static void cbAddSprite(Fl_Widget *widget,void*)
	{
		instance->addSprite();
	}
	static void cbAddFont(Fl_Widget *widget,void*)
	{
		instance->addFont();
	}
	static void cbAddSound(Fl_Widget *widget,void*)
	{
		instance->addSound();
	}
	static void cbAddRaw(Fl_Widget *widget,void*)
	{
		instance->addRaw();
	}
	static void cbRemoveSet(Fl_Widget *widget,void*)
	{
		instance->removeSet();
	}
	static void cbGenerate(Fl_Widget *widget,void*)
	{
		instance->generate();
	}
	static void cbSave(Fl_Widget *widget,void*)
	{
		instance->save();
	}
	static void cbSaveAs(Fl_Widget *widget,void*)
	{
		instance->saveAs();
	}
	static void cbLoad(Fl_Widget *widget,void*)
	{
		instance->load();
	}
	static void cbClose(Fl_Widget *widget,void*)
	{
		instance->close();
	}
	MainWindow(int width,int height,const char *title,int argc,char *argv[]);
	virtual ~MainWindow();
};
