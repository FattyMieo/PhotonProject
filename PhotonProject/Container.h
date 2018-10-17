#pragma once

#include <list>
#include "ListExtension.h"

template<class T>
class Container
{
private:
	std::list<T*> m_list;

public:
	Container()
	{

	}

	~Container()
	{
		std::list<T*>::iterator it = m_list.begin();

		while (it != m_list.end())
		{
			delete *it;
			++it;
		}

		m_list.clear();
	}

	std::list<T*>& GetList()
	{
		return m_list;
	}

	T& GetItem(unsigned int index)
	{
		return ListExtension::Get(GetList(), index);
	}
};