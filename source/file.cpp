#include "mainwindow.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <algorithm>
#include <string>

void MainWindow::generateData(const std::string &folder)
{
	std::ofstream file;
	
	file.open(folder+"/"+"resources.h");

	if (file.is_open())
	{
		file << "#pragma once\n";
		file << "#include <SE3D/SE3D.h>\n";
		file << "\n";
		file << "/* USAGE: \n";
		file << "\tSet:\n";
		file << "\t\tR::set\n";
		file << "\t\tR::set->id()\n";
		file << "\t\tResourceSet *s=R::set\n";
		file << "\t\tR::set->load()";
		file << "\t\n";
		file << "\tResource:\n";
		file << "\t\tR::set.res\n";
		file << "\t\tR::set.res->id()\n";
		file << "\t\tSprite *s=R::set.res\n";
		file << "*/\n";
		file << "\n";
		file << "namespace ____R\n";
		file << "{\n";
		for(unsigned int i=0;i<setView->size();i++)
		{
			Set *set=setView->getSet(i);
			std::stringstream ss;
			ss << "\tstruct ____" << i << set->name << ":public SE3D::_SetPointer\n";
			ss << "\t{\n";
			ss << "\t\tSE3D::ResourceSet* operator->()\n";
			ss << "\t\t{\n";
			ss << "\t\t\treturn _s;\n";
			ss << "\t\t}\n";
			for(unsigned int j=0;j<set->res.size();j++)
			{
				Resource *res=set->res[j];
				if (res->type==Resource::Raw)
				continue;
				ss << "\t\tstatic SE3D::_ResPointer " << res->name << ";\n";
			}
			ss << "\t};\n";
			file << ss.str();
		}
		file << "}\n";
		file << "\n";
		file << "namespace R\n";
		file << "{\n";

		//resources
		for(unsigned int i=0;i<setView->size();i++)
		{
			Set *set=setView->getSet(i);
			file << "\textern ____R::____" << i << set->name << " " << set->name << ";\n";
		}

		//load
		file << "\t\n";
		file << "\tvoid load();\n";
		file << "};\n";

		file.close();
	}
	
	file.open(folder+"/"+"resources.cpp");

	if (file.is_open())
	{
		file << "#include \"resources.h\"\n";
		file << "\n";
		file << "namespace R\n";
		file << "{\n";
		for(unsigned int i=0;i<setView->size();i++)
		{
			Set *set=setView->getSet(i);
			file << "\t____R::____" << i << set->name << " " << set->name << ";\n";
		}
		file << "}\n";
		file << "\n";
		file << "namespace ____R\n";
		file << "{\n";
		for(unsigned int i=0;i<setView->size();i++)
		{
			Set *set=setView->getSet(i);
			for(unsigned int j=0;j<set->res.size();j++)
			{
				Resource *res=set->res[j];
				if (res->type==Resource::Raw)
				continue;
				file << "\tSE3D::_ResPointer ____" << i << set->name << "::" << res->name << "(" << j << ");\n";
			}
		}
		file << "}\n";
		file << "\n";
		file << "void R::load()\n";
		file << "{\n";
		for(unsigned int i=0;i!=setView->size();i++)
		{
			Set *set=setView->getSet(i);
			file<< "\t_engineprivate::_SetHelper::assign(&" << set->name << ",new SE3D::ResourceSet());\n";
			for(unsigned int j=0;j<set->res.size();j++)
			{
				Resource *res=set->res[j];

				switch (res->type)
				{
					default:
					case Resource::Raw:
						continue;
					case Resource::Sprite:
						file << "\t_engineprivate::_ResHelper::assign(&" << set->name <<"." << res->name <<"," << set->name << "->addSprite(\"" <<  res->file << "\"," << res->smooth << "));\n";
						break;
					case Resource::Sound:
						file << "\t_engineprivate::_ResHelper::assign(&" << set->name <<"." << res->name <<"," << set->name << "->addSound(\"" <<  res->file << "\"," << res->streamed << "));\n";
						break;
					case Resource::Font:
						file << "\t_engineprivate::_ResHelper::assign(&" << set->name <<"." << res->name <<"," << set->name << "->addFont(\"" <<  res->file << "\"," << res->size << "," << res->fontchars << "," << res->smooth << "));\n";
						break;
				}
			}
			if (i!=setView->size()-1)
			file << "\t\n";
		}
		file << "}\n";
	}
}

void MainWindow::saveFile(const std::string &filename)
{
	std::ofstream file;

	std::string fn=filename;
	std::transform(fn.begin(),fn.end(),fn.begin(),::tolower);
	if (fn.substr(fn.size()-5).compare(".tdat"))
	fn+=".tdat";

	file.open(fn);

	if (file.is_open())
	{
		using namespace rapidjson;

		Document doc;
		doc.SetObject();
		doc.AddMember("directory",StringRef(lastDir.c_str()),doc.GetAllocator());
		doc.AddMember("compile",(bool)(cbCompile->value()==0?0:1),doc.GetAllocator());
		
		Value jprev;
		jprev.SetObject();
		jprev.AddMember("fontsize",prevSize,doc.GetAllocator());
		jprev.AddMember("imagesmooth",prevSmooth,doc.GetAllocator());
		jprev.AddMember("fontchars",prevChars,doc.GetAllocator());
		doc.AddMember("prev",jprev,doc.GetAllocator());

		Value sets;
		sets.SetArray();

		for(unsigned int i=0;i!=setView->size();i++)
		{
			Set *set=setView->getSet(i);

			Value jset;
			jset.SetObject();
			jset.AddMember("name",StringRef(set->name.c_str()),doc.GetAllocator());
			
			Value resources;
			resources.SetArray();

			for(unsigned int j=0;j<set->res.size();j++)
			{
				Resource *res=set->res[j];

				Value jres;
				jres.SetObject();
				jres.AddMember("type",res->type,doc.GetAllocator());
				jres.AddMember("name",StringRef(res->name.c_str()),doc.GetAllocator());
				jres.AddMember("filename",StringRef(res->file.c_str()),doc.GetAllocator());

				switch (res->type)
				{
					default:
						break;
					case Resource::Sprite:
						jres.AddMember("smooth",res->smooth,doc.GetAllocator());
						break;
					case Resource::Sound:
						jres.AddMember("streamed",res->streamed,doc.GetAllocator());
						break;
					case Resource::Font:
						jres.AddMember("smooth",res->smooth,doc.GetAllocator());
						jres.AddMember("size",res->size,doc.GetAllocator());
						jres.AddMember("chars",res->fontchars,doc.GetAllocator());
						break;
					case Resource::Raw:
						break;
				}
				resources.PushBack(jres,doc.GetAllocator());
			}
			
			jset.AddMember("resources",resources,doc.GetAllocator());

			sets.PushBack(jset,doc.GetAllocator());
		}

		doc.AddMember("sets",sets,doc.GetAllocator());

		StringBuffer buffer;
		buffer.Clear();
		Writer<StringBuffer> writer(buffer);
		doc.Accept(writer);

		file << buffer.GetString();
		file.close();
	}
}

bool MainWindow::loadFile(const std::string &filename)
{
	std::ifstream file;
	
	std::string fn=filename;
	std::transform(fn.begin(),fn.end(),fn.begin(),::tolower);
	if (fn.substr(fn.size()-5).compare(".tdat"))
	fn+=".tdat";

	file.open(fn);

	if (file.is_open())
	{
		using namespace rapidjson;
		
		std::string line;
		std::string contents="";

		while(std::getline(file,line))
		contents+=line+'\n';
		file.close();
		
		Document d;
		d.Parse(contents.c_str());

		if (!d.IsObject()||!d.HasMember("directory")||!d.HasMember("compile")||!d.HasMember("sets")||!d["directory"].IsString()||!d["compile"].IsBool()||!d["sets"].IsArray())
		return 0;
		
		const Value& sets=d["sets"];
		for(unsigned int i=0;i<sets.Size();i++)
		{
			if (!sets[i].IsObject()||!sets[i].HasMember("name")||!sets[i].HasMember("resources")||!sets[i]["name"].IsString()||!sets[i]["resources"].IsArray())
			return 0;
			const Value& resources=sets[i]["resources"];
			for(unsigned int j=0;j<resources.Size();j++)
			{
				if (!resources[j].IsObject()||!resources[j].HasMember("type")||!resources[j].HasMember("name")||!resources[j].HasMember("filename")||!resources[j]["type"].IsInt()||!resources[j]["name"].IsString()||!resources[j]["filename"].IsString())
				return 0;
				switch(resources[j]["type"].GetInt())
				{
					default:
						return 0;
					case Resource::Sprite:
						if (!resources[j].HasMember("smooth")||!resources[j]["smooth"].IsBool())
						return 0;
						break;
					case Resource::Sound:
						if (!resources[j].HasMember("streamed")||!resources[j]["streamed"].IsBool())
						return 0;
						break;
					case Resource::Font:
						if (!resources[j].HasMember("smooth")||!resources[j].HasMember("size")||!resources[j].HasMember("chars")||!resources[j]["smooth"].IsBool()||!resources[j]["size"].IsInt()||!resources[j]["chars"].IsInt())
						return 0;
						break;
					case Resource::Raw:
						break;
				}
			}
		}
		
		//all validated

		setView->eraseAll();
		resourceView->eraseAll();

		cbCompile->value(d["compile"].GetBool());
		lastDir=d["directory"].GetString();

		if (d.HasMember("prev")&&d["prev"].IsObject())
		{
			if (d["prev"].HasMember("imagesmooth")&&d["prev"]["imagesmooth"].IsBool())
			prevSmooth=d["prev"]["imagesmooth"].GetBool();
			if (d["prev"].HasMember("fontchars")&&d["prev"]["fontchars"].IsInt())
			{
				prevChars=d["prev"]["fontchars"].GetInt();
				if (prevChars<=0)
				prevChars=256;
			}
			if (d["prev"].HasMember("fontsize")&&d["prev"]["fontsize"].IsInt())
			{
				prevSize=d["prev"]["fontsize"].GetInt();
				if (prevSize<=0)
				prevSize=24;
			}
		}

		for(unsigned int i=0;i<sets.Size();i++)
		{
			Set *set=new Set();
			set->name=sets[i]["name"].GetString();
			loadSet(set);
			
			const Value& resources=sets[i]["resources"];
			for(unsigned int j=0;j<resources.Size();j++)
			{
				Resource *res=new Resource();
				res->type=resources[j]["type"].GetInt();
				res->name=resources[j]["name"].GetString();
				res->file=resources[j]["filename"].GetString();
				switch(res->type)
				{
					default:
						break;
					case Resource::Sprite:
						res->smooth=resources[j]["smooth"].GetBool();
						break;
					case Resource::Sound:
						res->streamed=resources[j]["streamed"].GetBool();
						break;
					case Resource::Font:
						res->smooth=resources[j]["smooth"].GetBool();
						res->size=resources[j]["size"].GetInt();
						res->fontchars=resources[j]["chars"].GetInt();
						break;
					case Resource::Raw:
						break;
				}
				loadResource(set,res);
			}
		}
	}

	return 1;
}

void MainWindow::saveSettings(const std::string &filename)
{
	std::ofstream file;

	std::string fn=filename;

	file.open(fn);

	if (file.is_open())
	{
		using namespace rapidjson;

		Document doc;
		doc.SetObject();
		doc.AddMember("directory",StringRef(lastDir.c_str()),doc.GetAllocator());
		doc.AddMember("file",StringRef(lastSave.c_str()),doc.GetAllocator());
		
		StringBuffer buffer;
		buffer.Clear();
		Writer<StringBuffer> writer(buffer);
		doc.Accept(writer);

		file << buffer.GetString();
		file.close();
	}
}

bool MainWindow::loadSettings(const std::string &filename)
{
	std::ifstream file;
	
	std::string fn=filename;

	file.open(fn);

	if (file.is_open())
	{
		using namespace rapidjson;
		
		std::string line;
		std::string contents="";

		while(std::getline(file,line))
		contents+=line+'\n';
		file.close();
		
		Document d;
		d.Parse(contents.c_str());

		if (!d.IsObject()||!d.HasMember("directory")||!d.HasMember("file")||!d["directory"].IsString()||!d["file"].IsString())
		return 0;
		
		lastDir=d["directory"].GetString();
		lastSave=d["file"].GetString();
	}
	return 1;
}
