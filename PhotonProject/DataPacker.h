#pragma once
#include "TypeDefinition.h"

template <typename T>
class DataPacker
{
private:
	T m_data;
	uint m_bitCount; //to count how many bits are used.

public:
	DataPacker();
	virtual ~DataPacker();

	//pack the "value" into m_data, it retuns true if there are enough bits available 
	//bitCount : the bit count of "value"
	bool Pack(T inValue, uint bitCount);

	bool Extract(T& outValue, uint bitCount);

	T GetData();
	void SetData(T data);
};

template<typename T>
inline DataPacker<T>::DataPacker()
{
	m_data = 0;
	m_bitCount = 0;
}

template<typename T>
inline DataPacker<T>::~DataPacker() { }

template<typename T>
inline bool DataPacker<T>::Pack(T inValue, uint bitCount)
{
	//Unable to pack if there's not enough bits available
	if (m_bitCount + bitCount > sizeof(T) * 8)
	{
		return false;
	}

	m_data = m_data << bitCount;
	m_data = m_data | inValue;

	m_bitCount += bitCount;

	return true;
}

template<typename T>
inline bool DataPacker<T>::Extract(T& outValue, uint bitCount)
{
	//Unable to pack if there's not enough bits available
	if (m_bitCount - bitCount < 0)
	{
		return false;
	}

	T mask = (1 << bitCount) - 1;
	outValue = m_data & mask;

	m_data = m_data >> bitCount;

	m_bitCount -= bitCount;

	return true;
}

template<typename T>
inline T DataPacker<T>::GetData()
{
	return m_data;
}

template<typename T>
inline void DataPacker<T>::SetData(T data)
{
	m_data = data;
}