#pragma once
#include "TypeDefinition.h"

template <typename T>
class DataSplitter
{
private:
	byte* m_bytes;
	const byte byteMask = (byte)255;

public:
	DataSplitter();
	virtual ~DataSplitter();

	byte* GetArray();
	void SetArray(byte* data);
	void Split(T value); //store "1 unsigned int" value into "4 bytes"
	T Merge(void); //merge the bytes amd return the unsigned int value
};

template<typename T>
inline DataSplitter<T>::DataSplitter()
{
	m_bytes = new byte[sizeof(T)];
}

template<typename T>
inline DataSplitter<T>::~DataSplitter() { }

template<typename T>
inline byte * DataSplitter<T>::GetArray()
{
	return m_bytes;
}

template<typename T>
inline void DataSplitter<T>::SetArray(byte * data)
{
	m_bytes = data;
}

template<typename T>
inline void DataSplitter<T>::Split(T value)
{
	for (int i = 0; i < sizeof(T); ++i)
	{
		m_bytes[i] = (byte)(value & byteMask);
		
		value = value >> 7 >> 1; //Fix Warning "'>>': Right shift by too large ammount, data loss"
	}
}

template<typename T>
inline T DataSplitter<T>::Merge(void)
{
	T ret = 0;

	for (int i = sizeof(T) - 1; i >= 0; --i)
	{
		ret = ret << 8;
		ret = ret | m_bytes[i];
	}

	return ret;
}