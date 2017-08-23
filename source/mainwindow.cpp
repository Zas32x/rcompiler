#include "mainwindow.h"
#include "namewindow.h"

//for window centering
#define private public
#include <FL/Fl_File_Chooser.H>
#undef private

MainWindow* MainWindow::instance=NULL;

MainWindow::MainWindow(int width,int height,const char *title,int argc,char *argv[]):Fl_Window(width,height,title)
{
	if (argc>0)
	{
		workDir=argv[0];

		unsigned int pos=workDir.find_last_of("/\\");
		if (pos!=std::string::npos)
		{
			workDir=workDir.substr(0,pos)+"/";
			if (workDir=="")
			workDir="./";
		}
		else
		workDir="./";
	}
	else
	workDir="./";

	setFile=workDir+"settings.dat";
	
	instance=this;
	begin();
		bGenerate=new Fl_Button(w()-110,h()-40,100,30,"Generate");
		bGenerate->callback(cbGenerate);
		bSave=new Fl_Button(w()-200,h()-40,80,30,"Save");
		bSave->callback(cbSave);
		bSaveAs=new Fl_Button(w()-290,h()-40,80,30,"Save As");
		bSaveAs->callback(cbSaveAs);
		bLoad=new Fl_Button(w()-380,h()-40,80,30,"Load");
		bLoad->callback(cbLoad);
		cbCompile=new Fl_Check_Button(10,h()-40,100,30,"Compile");
		cbCompile->deactivate();
		
		setView=new SetView(5,50,150,h()-130,"Sets");
		bAddSet=new Fl_Button(5,h()-80,75,30,"@+ Set");
		bAddSet->callback(cbAddSet);
		bAddSet->take_focus();
		bRemoveSet=new Fl_Button(80,h()-80,75,30,"@line Set");
		bRemoveSet->callback(cbRemoveSet);
		bRemoveSet->deactivate();

		resourceView=new ResourceView(160,50,w()-165,h()-130,"Resources");
		bAddSprite=new Fl_Button(w()-85,h()-80,80,30,"@+ Sprite");
		bAddSprite->callback(cbAddSprite);
		bAddSprite->deactivate();
		bAddSprite->hide();
		bAddSound=new Fl_Button(w()-85,h()-80,80,30,"@+ Sound");
		bAddSound->callback(cbAddSound);
		bAddSound->deactivate();
		bAddSound->hide();
		bAddFont=new Fl_Button(w()-85,h()-80,80,30,"@+ Font");
		bAddFont->callback(cbAddFont);
		bAddFont->deactivate();
		bAddFont->hide();
		bAddRaw=new Fl_Button(w()-85,h()-80,80,30,"@+ Raw");
		bAddRaw->callback(cbAddRaw);
		bAddRaw->deactivate();
		bAddRaw->hide();
		//*bAddSound,*bAddFont,*bAddRaw
	end();
	callback(cbClose);
	position((Fl::w()-w())/2,(Fl::h()-h())/2);
	show();
	
	saved=0;
	lastDir=".";
	lastSave="resources.tdat";
	renaming=-1;
	prevSmooth=1;
	prevSize=24;
	prevChars=256;
	checkAddButton();
	
	loadSettings(setFile);
	if (loadFile(lastSave))
	saved=1;
}

void MainWindow::selectedSet(int id)
{
	if (id>=0)
	{
		bRemoveSet->activate();
		bAddSprite->activate();
		bAddSound->activate();
		bAddFont->activate();
		bAddRaw->activate();
	}
	else
	{
		bRemoveSet->deactivate();
		bAddSprite->deactivate();
		bAddSound->deactivate();
		bAddFont->deactivate();
		bAddRaw->deactivate();
	}
	resourceView->fillWith(setView->getSet(id));
}

void MainWindow::checkAddButton()
{
	bAddSprite->hide();
	bAddSound->hide();
	bAddFont->hide();
	bAddRaw->hide();
	switch (resourceView->getTab())
	{
		default:
			break;
		case Resource::Sprite:
			bAddSprite->show();
			break;
		case Resource::Sound:
			bAddSound->show();
			break;
		case Resource::Font:
			bAddFont->show();
			break;
		case Resource::Raw:
			bAddRaw->show();
			break;
	}
}

void MainWindow::addSet()
{
	if (NameWindow::instance)
	NameWindow::instance->close();

	renaming=-1;
	new NameWindow(300,50,"Add resource set","");
}

void MainWindow::renameSet(int id)
{
	if (NameWindow::instance)
	NameWindow::instance->close();
	
	if (id>=0)
	{
		renaming=id;
		new NameWindow(300,50,(std::string("Rename \"")+setView->getSet(id)->name+"\" set").c_str(),setView->getSet(id)->name.c_str());
	}
}

void MainWindow::removeSet()
{
	int id=setView->getValue()-1;
	if (id<0)
	return;

	setView->removeItem(id);
}

void MainWindow::nameAdded(const std::string &str)
{
	if (renaming<0)
	{
		setView->addItem(str.c_str());
	}
	else
	{
		setView->renameItem(str,renaming);
	}
}

void MainWindow::loadSet(Set *set)
{
	setView->addItem(set);
}

void MainWindow::loadResource(Set *set,Resource *res)
{
	set->res.push_back(res);
	resourceView->addItem(res);
}

void MainWindow::addSprite()
{
	Set *set=setView->getSet();
	if (set)
	{
		Resource *res=new Resource();
		res->type=Resource::Sprite;
		res->file="";
		res->name="";
		res->smooth=prevSmooth;
		set->res.push_back(res);
		resourceView->addItem(res);
	}
}

void MainWindow::addFont()
{
	Set *set=setView->getSet();
	if (set)
	{
		Resource *res=new Resource();
		res->type=Resource::Font;
		res->file="";
		res->name="";
		res->size=prevSize;
		res->fontchars=prevChars;
		res->smooth=prevSmooth;
		set->res.push_back(res);
		resourceView->addItem(res);
	}
}

void MainWindow::addSound()
{
	Set *set=setView->getSet();
	if (set)
	{
		Resource *res=new Resource();
		res->type=Resource::Sound;
		res->file="";
		res->name="";
		res->streamed=0;
		set->res.push_back(res);
		resourceView->addItem(res);
	}
}

void MainWindow::addRaw()
{
	Set *set=setView->getSet();
	if (set)
	{
		Resource *res=new Resource();
		res->type=Resource::Raw;
		res->file="";
		res->name="";
		set->res.push_back(res);
		resourceView->addItem(res);
	}
}

void MainWindow::saveAs()
{
	Fl_File_Chooser chooser(lastSave.c_str(),"*.tdat",Fl_File_Chooser::CREATE,"Save as");
	chooser.preview(0);
	chooser.show();
	chooser.window->position((Fl::w()-chooser.window->w())/2,(Fl::h()-chooser.window->h())/2);

	while(chooser.shown())
	{
		Fl::wait();
	}

	if (chooser.value()!=NULL)
	{
		lastSave=chooser.value();

		saveFile(lastSave);
		saved=1;
	}
}

bool MainWindow::load()
{
	Fl_File_Chooser chooser(lastSave.c_str(),"*.tdat",Fl_File_Chooser::SINGLE,"Load");
	chooser.preview(0);
	chooser.show();
	chooser.window->position((Fl::w()-chooser.window->w())/2,(Fl::h()-chooser.window->h())/2);

	while(chooser.shown())
	{
		Fl::wait();
	}

	if (chooser.value()!=NULL)
	{
		lastSave=chooser.value();

		if (loadFile(lastSave))
		{
			setView->resetSelection();
			resourceView->resetSelection();
			saved=1;
			return 1;
		}
	}
	return 0;
}

void MainWindow::save()
{
	if (saved)
	{
		saveFile(lastSave);
		saved=1;
	}
	else
	saveAs();
}

void MainWindow::generate()
{
	Fl_File_Chooser chooser(lastDir.c_str(),"",Fl_File_Chooser::DIRECTORY,"Export to");
	chooser.preview(0);
	chooser.show();
	chooser.window->position((Fl::w()-chooser.window->w())/2,(Fl::h()-chooser.window->h())/2);

	while(chooser.shown())
	{
		Fl::wait();
	}

	if (chooser.value()!=NULL)
	{
		lastDir=chooser.value();

		generateData(lastDir);
	}
}

void MainWindow::close()
{
	saveSettings(setFile);
	hide();
}

MainWindow::~MainWindow()
{
	
}
