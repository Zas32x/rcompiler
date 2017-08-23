#include "resourceview.h"
#include <FL\Fl_Input.H>
#include <FL\Fl_Int_Input.H>
#include <FL\Fl_Check_Button.H>
#include "editfield.h"
#include "mainwindow.h"

ResourceView *ResourceView::instance=NULL;

ResourceView::ResourceView(int xp,int yp,int width,int height,const char* label):Fl_Group(xp,yp,width,height)
{
	instance=this;

	new Fl_Box(x(),y()-20,width,20,label);
	
	begin();
		tabs=new Fl_Tabs(x(),y(),w(),h());
		tabs->begin();
			gSprites=new Fl_Group(x(),y()+30,w(),h()-30,"Sprites");
			gSprites->begin();

				sprites=new Fl_Scroll(x()+5,y()+50,w()-10,h()-55);
				sprites->type(Fl_Scroll::VERTICAL);
				sprites->end();
				
				(new Fl_Box(x()+35,y()+30,165,20,"Name"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+205,y()+30,sprites->w()-260,20,"File"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+sprites->w()-55-Fl::scrollbar_size(),y()+30,60,20,"Smooth"))->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
			gSprites->end();
			
			gSounds=new Fl_Group(x(),y()+30,w(),h()-30,"Sounds");
			gSounds->begin();
				sounds=new Fl_Scroll(x()+5,y()+50,w()-10,h()-55);
				sounds->type(Fl_Scroll::VERTICAL);
				sounds->end();
				
				(new Fl_Box(x()+35,y()+30,165,20,"Name"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+205,y()+30,sounds->w()-260,20,"File"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+sounds->w()-55-Fl::scrollbar_size(),y()+30,60,20,"Stream"))->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
			gSounds->end();

			gFonts=new Fl_Group(x(),y()+30,w(),h()-30,"Fonts");
			gFonts->begin();
				fonts=new Fl_Scroll(x()+5,y()+50,w()-10,h()-55);
				fonts->type(Fl_Scroll::VERTICAL);
				fonts->end();
				
				(new Fl_Box(x()+35,y()+30,165,20,"Name"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+205,y()+30,fonts->w()-370,20,"File"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+fonts->w()-160-Fl::scrollbar_size(),y()+30,50,20,"Size"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+fonts->w()-105-Fl::scrollbar_size(),y()+30,50,20,"Chars"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+fonts->w()-55-Fl::scrollbar_size(),y()+30,60,20,"Smooth"))->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
			gFonts->end();

			gRaw=new Fl_Group(x(),y()+30,w(),h()-30,"Raw");
			gRaw->begin();
				raw=new Fl_Scroll(x()+5,y()+50,w()-10,h()-55);
				raw->type(Fl_Scroll::VERTICAL);
				raw->end();

				(new Fl_Box(x()+35,y()+30,165,20,"Name"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
				(new Fl_Box(x()+205,y()+30,sprites->w()-210,20,"File"))->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
			gRaw->end();
		tabs->end();
		tabs->callback(cbTabChange,this);
	end();

	fillWith(NULL);
}

void ResourceView::addItem(Resource *res,bool select)
{
	switch(res->type)
	{
		default:
			break;
		case Resource::Sprite:
			{
				SpriteEdit *e=new SpriteEdit(res,sprites->x(),sprites->y()-sprites->yposition()+40*eSprite.size(),sprites->w()-Fl::scrollbar_size(),40);
				sprites->add(e);
				eSprite.push_back(e);
				tabs->value(gSprites);
				scrollTo(sprites,e,eSprite.size());
			}
			break;
		case Resource::Font:
			{
				FontEdit *e=new FontEdit(res,fonts->x(),fonts->y()-fonts->yposition()+40*eFont.size(),fonts->w()-Fl::scrollbar_size(),40);
				fonts->add(e);
				eFont.push_back(e);
				tabs->value(gFonts);
				scrollTo(fonts,e,eFont.size());
			}
			break;
		case Resource::Sound:
			{
				SoundEdit *e=new SoundEdit(res,sounds->x(),sounds->y()-sounds->yposition()+40*eSound.size(),sounds->w()-Fl::scrollbar_size(),40);
				sounds->add(e);
				eSound.push_back(e);
				tabs->value(gSounds);
				scrollTo(sounds,e,eSound.size());
			}
			break;
		case Resource::Raw:
			{
				RawEdit *e=new RawEdit(res,raw->x(),raw->y()-raw->yposition()+40*eRaw.size(),raw->w()-Fl::scrollbar_size(),40);
				raw->add(e);
				eRaw.push_back(e);
				tabs->value(gRaw);
				scrollTo(raw,e,eRaw.size());
			}
			break;
	}
	redraw();
}

void ResourceView::moveUp(int type,Fl_Widget *who)
{
	switch (type)
	{
		default:
			break;
		case Resource::Sprite:
			{
				bool first=1;
				std::vector<SpriteEdit*>::iterator del=eSprite.end();
				for(std::vector<SpriteEdit*>::iterator iter=eSprite.begin();iter!=eSprite.end();iter++)
				{
					if ((*iter)==who)
					del=iter;

					if ((*iter)->y()>=who->y()&&!first)
					(*iter)->resize((*iter)->x(),(*iter)->y()-40,(*iter)->w(),(*iter)->h());

					first=0;
				}
				if (del!=eSprite.end())
				eSprite.erase(del);
				
				scrollCheck(sprites,eSprite.size());
			}
			break;
		case Resource::Sound:
			{
				bool first=1;
				std::vector<SoundEdit*>::iterator del=eSound.end();
				for(std::vector<SoundEdit*>::iterator iter=eSound.begin();iter!=eSound.end();iter++)
				{
					if ((*iter)==who)
					del=iter;

					if ((*iter)->y()>=who->y()&&!first)
					(*iter)->resize((*iter)->x(),(*iter)->y()-40,(*iter)->w(),(*iter)->h());

					first=0;
				}
				if (del!=eSound.end())
				eSound.erase(del);
				
				scrollCheck(sounds,eSound.size());
			}
			break;
		case Resource::Font:
			{
				bool first=1;
				std::vector<FontEdit*>::iterator del=eFont.end();
				for(std::vector<FontEdit*>::iterator iter=eFont.begin();iter!=eFont.end();iter++)
				{
					if ((*iter)==who)
					del=iter;

					if ((*iter)->y()>=who->y()&&!first)
					(*iter)->resize((*iter)->x(),(*iter)->y()-40,(*iter)->w(),(*iter)->h());

					first=0;
				}
				if (del!=eFont.end())
				eFont.erase(del);
				
				scrollCheck(fonts,eFont.size());
			}
			break;
		case Resource::Raw:
			{
				bool first=1;
				std::vector<RawEdit*>::iterator del=eRaw.end();
				for(std::vector<RawEdit*>::iterator iter=eRaw.begin();iter!=eRaw.end();iter++)
				{
					if ((*iter)==who)
					del=iter;

					if ((*iter)->y()>=who->y()&&!first)
					(*iter)->resize((*iter)->x(),(*iter)->y()-40,(*iter)->w(),(*iter)->h());

					first=0;
				}
				if (del!=eRaw.end())
				eRaw.erase(del);

				scrollCheck(raw,eRaw.size());
			}
			break;
	}
	redraw();
}

void ResourceView::tabChange()
{
	MainWindow::instance->checkAddButton();
}

void ResourceView::resetSelection()
{
	tabs->value(gSprites);
	tabChange();
}

int ResourceView::getTab()
{
	Fl_Widget *w=tabs->value();
	if (w==gSprites)
	return Resource::Sprite;
	if (w==gSounds)
	return Resource::Sound;
	if (w==gFonts)
	return Resource::Font;
	if (w==gRaw)
	return Resource::Raw;
	return -1;
}

void ResourceView::scrollTo(Fl_Scroll *scroll,Fl_Widget *to,int amount)
{
	int py=0;
	
	int showMin=to->y()-scroll->y()+scroll->yposition();
	int showMax=to->y()-scroll->y()+scroll->yposition()+to->h();
	
	py=scroll->yposition();

	if (py<showMax-scroll->h())
	py=showMax-scroll->h();
	
	if (py>showMin)
	py=showMin;
	
	if (py>amount*40-scroll->h())
	py=amount*40-scroll->h();

	if (py<0)
	py=0;
	
	scroll->scroll_to(0,py);
}

void ResourceView::scrollCheck(Fl_Scroll *scroll,int amount)
{
	int py=scroll->yposition();

	if (py>amount*40-scroll->h())
	py=amount*40-scroll->h();
	
	if (py<0)
	py=0;
	
	scroll->scroll_to(0,py);
}

void ResourceView::fillWith(Set *set)
{
	eSprite.clear();
	eFont.clear();
	eSound.clear();
	eRaw.clear();

	sprites->clear();
	fonts->clear();
	sounds->clear();
	raw->clear();

	if (set!=NULL)
	for(Resource *res:set->res)
	{
		addItem(res,0);
	}

	sprites->redraw();
	fonts->redraw();
	sounds->redraw();
	raw->redraw();

	sprites->scroll_to(0,0);
	fonts->scroll_to(0,0);
	sounds->scroll_to(0,0);
	raw->scroll_to(0,0);
}

void ResourceView::eraseAll()
{
	fillWith(NULL);
}

ResourceView::~ResourceView()
{
	
}
