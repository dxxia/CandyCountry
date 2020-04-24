#include "PostDate.h"

PostDate::PostDate()
{
}

PostDate::~PostDate()
{
}

bool PostDate::init(){
	return true;
}

void PostDate::setVector(Vector<Entity*> itemList){
	m_itemList = itemList;
}

Vector<Entity*> PostDate::getVector(){
	return m_itemList;
}