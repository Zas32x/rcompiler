#include "setview.h"
#include "resourceview.h"
#include "mainwindow.h"
#include "resources.h"

SetView *SetView::instance=NULL;

SetView::SetView(int x,int y,int width,int height,const char* label):Fl_Select_Browser(x,y,width,height,"")
{
	instance=this;

	new Fl_Box(x,y-20,width,20,label);
	box(FL_DOWN_BOX);
	type(VERTICAL);
	when(FL_WHEN_CHANGED|FL_WHEN_NOT_CHANGED);
	callback(&cbClick);
}

void SetView::addItem(const char* name)
{
	add((toString(size())+" "+name).c_str(),new Set(name));
	setSelection(size()-1);
}

void SetView::addItem(Set *set)
{
	add((toString(size())+" "+set->name.c_str()).c_str(),set);
	setSelection(size()-1);
}

void SetView::renameItem(const std::string &name,int id)
{
	((Set*)data(id+1))->name=name;
	text(id+1,(toString(id)+" "+name).c_str());
}

void SetView::removeItem(int id)
{
	setSelection(-1);
	delete (Set*)data(id+1);
	remove(id+1);
}

void SetView::click()
{
	if (Fl::event_clicks()==1)
	MainWindow::instance->renameSet(value()-1);
	else
	setSelection(value()-1);
}

void SetView::resetSelection()
{
	if (size()>0)
	setSelection(0);
	else
	setSelection(-1);
}

void SetView::setSelection(int id)
{
	select(id+1);
	MainWindow::instance->selectedSet(id);
}

void SetView::cbClick(Fl_Widget *widget,void*)
{
	instance->click();
}

void SetView::eraseAll()
{
	while(size())
	removeItem(0);
}

SetView::~SetView()
{
	eraseAll();
}
