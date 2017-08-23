#pragma once
#include <vector>
#include <string>
#include <sstream>

template<class T>
std::string toString(T i)
{
	std::ostringstream o;
	o << i;
	return o.str();
}

int resInt(const char *str);

struct Resource
{
	enum Type{Sprite,Font,Sound,Raw};
	int type;
	//sprite,font
	bool smooth;
	//font
	int fontchars;
	int size;
	//sound
	bool streamed;

	std::string file;
	std::string name;
};

struct Set
{
	std::string name;
	std::vector<Resource*> res;
	Set(){}
	Set(const std::string &str):name(str){}
	virtual ~Set()
	{
		for(Resource *res:res)
		delete res;
	}
};
