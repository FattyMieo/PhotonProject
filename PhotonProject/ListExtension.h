#pragma once

#include <list>

namespace ListExtension
{
	template<class T>
	static T& Get(std::list<T*> mList, std::size_t index)
	{
		std::list<T*>::iterator it = mList.begin();
		std::size_t count = 0;

		while (it != mList.end())
		{
			if (count == index)
				return **it;

			it++;
			count++;
		}

		throw;
		//return NULL;
	}
}
