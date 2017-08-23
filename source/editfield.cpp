#include "editfield.h"
#include "mainwindow.h"
#include "setview.h"
#include "resourceview.h"

SpriteEdit::SpriteEdit(Resource *res,int xp,int yp,int width,int height):Fl_Group(xp,yp,width,height)
{
	resource=res;

	begin();
		del=new Fl_Button(x(),y(),30,30,"@line");
		del->callback(cbRemove,this);
		add(del);

		name=new Fl_Input(x()+35,y(),165,30);
		name->take_focus();
		name->value(res->name.c_str());
		name->callback(cbChanged,this);
		add(name);

		file=new Fl_Input(x()+205,y(),w()-260,30);
		file->value(res->file.c_str());
		file->callback(cbChanged,this);
		add(file);

		smooth=new Fl_Check_Button(x()+w()-35,y(),35,30);
		smooth->value(res->smooth);
		smooth->callback(cbChangedSmooth,this);
		add(smooth);
	end();
}

void SpriteEdit::remove()
{
	SetView *sets=SetView::instance;
	for(std::vector<Resource*>::iterator iter=sets->getSet()->res.begin();iter!=sets->getSet()->res.end();iter++)
	if ((*iter)==resource)
	{
		sets->getSet()->res.erase(iter);
		delete resource;
		resource=NULL;
		break;
	}
	ResourceView::instance->moveUp(Resource::Sprite,this);
	clear();
	Fl::delete_widget(this);
}

void SpriteEdit::changed()
{
	resource->name=name->value();
	resource->file=file->value();
	resource->smooth=smooth->value()==0?0:1;
}

void SpriteEdit::changedSmooth()
{
	changed();
	MainWindow::instance->updateSmooth(smooth->value()==0?0:1);
}













FontEdit::FontEdit(Resource *res,int xp,int yp,int width,int height):Fl_Group(xp,yp,width,height)
{
	resource=res;

	begin();
		del=new Fl_Button(x(),y(),30,30,"@line");
		del->callback(cbRemove,this);
		add(del);

		name=new Fl_Input(x()+35,y(),165,30);
		name->take_focus();
		name->value(res->name.c_str());
		name->callback(cbChanged,this);
		add(name);

		file=new Fl_Input(x()+205,y(),w()-370,30);
		file->value(res->file.c_str());
		file->callback(cbChanged,this);
		add(file);
		
		size=new Fl_Int_Input(x()+w()-160,y(),50,30);
		size->value(toString(res->size).c_str());
		size->callback(cbChangedSize,this);
		add(size);

		characters=new Fl_Int_Input(x()+w()-105,y(),50,30);
		characters->value(toString(res->fontchars).c_str());
		characters->callback(cbChangedChars,this);
		add(characters);

		smooth=new Fl_Check_Button(x()+w()-35,y(),35,30);
		smooth->value(res->smooth);
		smooth->callback(cbChangedSmooth,this);
		add(smooth);
	end();
}

void FontEdit::remove()
{
	SetView *sets=SetView::instance;
	for(std::vector<Resource*>::iterator iter=sets->getSet()->res.begin();iter!=sets->getSet()->res.end();iter++)
	if ((*iter)==resource)
	{
		sets->getSet()->res.erase(iter);
		delete resource;
		resource=NULL;
		break;
	}
	ResourceView::instance->moveUp(Resource::Font,this);
	clear();
	Fl::delete_widget(this);
}

void FontEdit::changed()
{
	resource->name=name->value();
	resource->file=file->value();
	resource->smooth=smooth->value()==0?0:1;
}

void FontEdit::changedSmooth()
{
	changed();
	MainWindow::instance->updateSmooth(smooth->value()==0?0:1);
}

void FontEdit::changedSize()
{
	int val=resInt(size->value());
	if (val<=0)
	{
		size->value("24");
		val=24;
	}

	changed();
	MainWindow::instance->updateSize(val);
}

void FontEdit::changedChars()
{
	int val=resInt(characters->value());
	if (val<=0)
	{
		characters->value("256");
		val=256;
	}

	changed();
	MainWindow::instance->updateChars(val);
}







SoundEdit::SoundEdit(Resource *res,int xp,int yp,int width,int height):Fl_Group(xp,yp,width,height)
{
	resource=res;

	begin();
		del=new Fl_Button(x(),y(),30,30,"@line");
		del->callback(cbRemove,this);
		add(del);

		name=new Fl_Input(x()+35,y(),165,30);
		name->take_focus();
		name->value(res->name.c_str());
		name->callback(cbChanged,this);
		add(name);

		file=new Fl_Input(x()+205,y(),w()-260,30);
		file->value(res->file.c_str());
		file->callback(cbChanged,this);
		add(file);

		streamed=new Fl_Check_Button(x()+w()-35,y(),35,30);
		streamed->value(res->streamed);
		streamed->callback(cbChanged,this);
		add(streamed);
	end();
}

void SoundEdit::remove()
{
	SetView *sets=SetView::instance;
	for(std::vector<Resource*>::iterator iter=sets->getSet()->res.begin();iter!=sets->getSet()->res.end();iter++)
	if ((*iter)==resource)
	{
		sets->getSet()->res.erase(iter);
		delete resource;
		resource=NULL;
		break;
	}
	ResourceView::instance->moveUp(Resource::Sound,this);
	clear();
	Fl::delete_widget(this);
}

void SoundEdit::changed()
{
	resource->name=name->value();
	resource->file=file->value();
	resource->streamed=streamed->value()==0?0:1;
}








RawEdit::RawEdit(Resource *res,int xp,int yp,int width,int height):Fl_Group(xp,yp,width,height)
{
	resource=res;

	begin();
		del=new Fl_Button(x(),y(),30,30,"@line");
		del->callback(cbRemove,this);
		add(del);

		name=new Fl_Input(x()+35,y(),165,30);
		name->take_focus();
		name->value(res->name.c_str());
		name->callback(cbChanged,this);
		add(name);

		file=new Fl_Input(x()+205,y(),w()-210,30);
		file->value(res->file.c_str());
		file->callback(cbChanged,this);
		add(file);
	end();
}

void RawEdit::remove()
{
	SetView *sets=SetView::instance;
	for(std::vector<Resource*>::iterator iter=sets->getSet()->res.begin();iter!=sets->getSet()->res.end();iter++)
	if ((*iter)==resource)
	{
		sets->getSet()->res.erase(iter);
		delete resource;
		resource=NULL;
		break;
	}
	ResourceView::instance->moveUp(Resource::Raw,this);
	clear();
	Fl::delete_widget(this);
}

void RawEdit::changed()
{
	resource->name=name->value();
	resource->file=file->value();
}
