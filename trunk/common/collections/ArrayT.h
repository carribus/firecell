#ifndef _ARRAYT_H_
#define _ARRAYT_H_

template <class T>
class ArrayT
{
public:
	ArrayT(int nInitialSize = 1, int nGrowBySize = 1)
		: m_pData(NULL)
		, m_nSize(0)
		, m_nElemCount(0)
		, m_nGrowBySize(nGrowBySize)
	{
		GrowBy(nInitialSize);
	}

	//////////////////////////////////////////////////////////////////

	ArrayT(ArrayT<T>& src)
	{
		*this = operator=(src);
	}

	//////////////////////////////////////////////////////////////////

	virtual ~ArrayT()
	{
		if ( m_pData )
		{
			delete [] m_pData;
			m_pData = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////

	void GrowBy(int nGrowBySize)
	{
		if ( nGrowBySize < 0 )
			return;

		int nNewSize = m_nSize + nGrowBySize;
		T* pNewArray = new T[nNewSize];

		if ( m_pData )
		{
			for ( int i = 0; i < m_nSize; i++ )
			{
				pNewArray[i] = m_pData[i];
			}
			
			delete [] m_pData;
			m_pData = NULL;
		}

		m_nSize = nNewSize;
		m_pData = pNewArray;
	}

	//////////////////////////////////////////////////////////////////

	T& GetAt(int nIndex) const
	{
		return m_pData[nIndex];
	}

	//////////////////////////////////////////////////////////////////

	const T& GetAt(int nIndex)
	{
		return m_pData[nIndex];
	}

	//////////////////////////////////////////////////////////////////

	int Add(T& elem)
	{
		if ( m_nElemCount == m_nSize )
			GrowBy(m_nGrowBySize);

		m_pData[m_nElemCount++] = elem;

		return m_nElemCount;
	}

	//////////////////////////////////////////////////////////////////

	void RemoveAt(int nIndex)
	{
		if ( nIndex > 0 && nIndex < m_nElemCount )
		{
			int nTargetNdx = 0;
			int nNewSize = m_nSize-1;
			int nNewElemCount = m_nElemCount-1;
			T* pNewArray = new T[nNewSize];

			for ( int i = 0; i < m_nElemCount; i++ )
			{
				if ( i == nIndex )
					continue;

				pNewArray[nTargetNdx++] = m_pData[i];
			}

			delete [] m_pData;
			m_pData = pNewArray;
			m_nSize = nNewSize;
			m_nElemCount = nNewElemCount;
		}
	}

	//////////////////////////////////////////////////////////////////

	void Copy(ArrayT<T>& src)
	{
		if ( m_pData )
		{
			delete [] m_pData;
			m_pData = NULL;
		}

		m_nSize = src.m_nSize;
		m_nGrowBySize = src.m_nGrowBySize;
		m_pData = new T[m_nSize];

		for ( int i = 0; i < src.m_nElemCount; i++ )
		{
			m_pData[i] = src.m_pData[i];
		}

		m_nElemCount = src.m_nElemCount;
	}

	//////////////////////////////////////////////////////////////////

	int GetCount()
	{
		return m_nElemCount;
	}

	//////////////////////////////////////////////////////////////////

	ArrayT<T>& operator=(ArrayT<T>& src)
	{
		Copy(src);
		
		return *this;
	}

protected:

	T*			m_pData;
	int			m_nSize;
	int			m_nElemCount;
	int			m_nGrowBySize;
};

#endif//_ARRAYT_H_