enum IE_CmpRes
{
	Eq,
	Gt,
	Lt
}

class IE_Cmp<Class T>
{
	static IE_CmpRes cmp(T e1, T e2)
	{
		return IE_CmpRes.Eq;
	}
}

class IE_CmpInt: IE_Cmp<int>
{
	override static IE_CmpRes cmp(int e1, int e2)
	{
		if (e1 < e2)
		{
			return IE_CmpRes.Lt;
		}
		else if (e1 > e2)
		{
			return IE_CmpRes.Gt;
		}
		else
		{
			return IE_CmpRes.Eq;
		}
	}
}

class IE_CmpFloat: IE_Cmp<float>
{
	override static IE_CmpRes cmp(float e1, float e2)
	{
		if (e1 < e2)
		{
			return IE_CmpRes.Lt;
		}
		else if (e1 > e2)
		{
			return IE_CmpRes.Gt;
		}
		else
		{
			return IE_CmpRes.Eq;
		}
	}
}

class IE_CmpStr: IE_Cmp<string>
{
	override static IE_CmpRes cmp(string e1, string e2)
	{
		if (e1 < e2)
		{
			return IE_CmpRes.Lt;
		}
		else if (e1 > e2)
		{
			return IE_CmpRes.Gt;
		}
		else
		{
			return IE_CmpRes.Eq;
		}
	}
}

class IE_SearchMatch<Class T>
{
	T m_value;

	void IE_SearchMatch(T value)
	{
		m_value = value;
	}

	IE_CmpRes check(T e1)
	{
		return IE_Cmp<T>.cmp(m_value, e1);
	}
}

class IE_BinSearchResult
{
	bool success;
	int idx;
	
	void IE_BinSearchResult(bool _success, int _idx)
	{
		success = _success;
		idx = _idx;
	}
}

class IE_ArrayUtils<Class T>
{
	// Assumes a sorted array
	// Returns an array with two elements [success, idx]
	// If success = 1, idx is the index where the element was found.
	// If success = 0, idx is the index where the element should be inserted to keep the array sorted.
    static IE_BinSearchResult BinarySearch(array<T> arr, IE_SearchMatch<T> cmp)
	{
		return BinarySearch(arr, cmp, 0, arr.Count());
	}
	
    private static IE_BinSearchResult BinarySearch(array<T> arr, IE_SearchMatch<T> cmp, int range_min, int range_max)
	{
		int distance = range_max - range_min;
		if (distance == 0)
		{
			return new IE_BinSearchResult(false, range_max);
		}

		int idx = Math.Floor(distance / 2) + range_min;
		IE_CmpRes checkRes = cmp.check(arr[idx]);
		if (checkRes == IE_CmpRes.Eq)
		{
			return new IE_BinSearchResult(true, idx);
		}
		else if (checkRes == IE_CmpRes.Lt) {
			return BinarySearch(arr, cmp, range_min, idx);
		}
		else {
			return BinarySearch(arr, cmp, idx + 1, range_max);
		}
	}
}