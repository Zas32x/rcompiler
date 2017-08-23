#include "namewindow.h"
#include "mainwindow.h"

NameWindow* NameWindow::instance=NULL;

NameWindow::NameWindow(int width,int height,const char *title,const std::string &contents):Fl_Window(width,height,title)
{
	instance=this;
	begin();
		bOk=new Fl_Button(w()-110,10,100,30,"OK");
		bOk->callback(cbOk);
		iName=new Fl_Input(10,10,w()-130,30,"");
		iName->value(contents.c_str());
		iName->when(FL_WHEN_ENTER_KEY);
		iName->callback(cbOk);
		iName->take_focus();
	end();
	position((Fl::w()-w())/2,(Fl::h()-h())/2);
	callback(cbClose);
	show();
}

void NameWindow::close()
{
	Fl_Window::hide();
	instance=NULL;
}

void NameWindow::ok()
{
	std::string val=iName->value();
	if (!val.empty())
	MainWindow::instance->nameAdded(val);
	Fl_Window::hide();
	instance=NULL;
}

NameWindow::~NameWindow()
{
	
}
