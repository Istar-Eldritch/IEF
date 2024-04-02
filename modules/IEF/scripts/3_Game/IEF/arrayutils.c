enum IE_CmpRes
{
	Eq,
	Gt,
	Lt
}

enum IE_SortOrd
{
	Increasing,
	Decreasing
}

class IE_Cmp<Class T>
{
	IE_CmpRes cmp(T e1, T e2)
	{
		return IE_CmpRes.Eq;
	}
}

class IE_CmpArrayInt: IE_Cmp<array<int>>
{
	override IE_CmpRes cmp(array<int> e1, array<int> e2)
	{
		if (e1.Count() == e2.Count())
		{
			for(int i = 0; i < e1.Count(); i++)
			{
				if (e1.Get(i) > e2.Get(i))
				{
					return IE_CmpRes.Gt;
				}
				else if (e1.Get(i) < e2.Get(i))
				{
					return IE_CmpRes.Lt;
				}
			}
			return IE_CmpRes.Eq;
		}
		else if (e1.Count() > e2.Count())
		{
			return IE_CmpRes.Gt;
		}
		else
		{
			return IE_CmpRes.Lt;
		}
	}
}

class IE_CmpInt: IE_Cmp<int>
{
	override IE_CmpRes cmp(int e1, int e2)
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
	override IE_CmpRes cmp(float e1, float e2)
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
	override IE_CmpRes cmp(string e1, string e2)
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
	ref IE_Cmp<T> m_comparator;

	void IE_SearchMatch(T value, IE_Cmp<T> comparator)
	{
		m_value = value;
		m_comparator = comparator;
	}

	IE_CmpRes check(T e1)
	{
		return m_comparator.cmp(m_value, e1);
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
    static IE_BinSearchResult BinarySearch(array<T> arr, IE_SearchMatch<T> cmp)
	{
		return BinarySearch(arr, cmp, 0, arr.Count() - 1);
	}
	
	private static IE_BinSearchResult BinarySearch(array<T> arr, IE_SearchMatch<T> cmp, int low, int high)
	{
		// Repeat until the pointers low and high meet each other
		int idx = 0;
		while (low <= high) {
			idx = low + (high - low) / 2;
			IE_CmpRes checkRes = cmp.check(arr[idx]);
			if (checkRes == IE_CmpRes.Eq)
			{
				return new IE_BinSearchResult(true, idx);
			}
			if (checkRes == IE_CmpRes.Gt)
			{
			  	low = idx + 1;
			}
			else
			{
			  	high = idx - 1;
			}
		}
		
		return new IE_BinSearchResult(false, idx);
	}

	// Iterative function to perform merge sort
	static void MergeSort(array<T> arr, IE_Cmp<T> comparator, IE_SortOrd ord) {
		int n = arr.Count();
	    int currentSize;  // Current size of subarrays to be merged
	    int leftStart;    // Starting index of left subarray
	    array<T> temp = new array<T>;      // Temporary array for merging
		temp.Resize(n);

	    // Merge subarrays in bottom-up manner
	    for (currentSize = 1; currentSize <= n - 1; currentSize = 2 * currentSize) {
	        // Pick starting point of different subarrays of current size
	        for (leftStart = 0; leftStart < n - 1; leftStart += 2 * currentSize) {
	            // Find ending point of left subarray
	            int middle = leftStart + currentSize - 1;
	
	            // Find ending point of right subarray
	            int rightEnd;
				if (leftStart + 2 * currentSize - 1 < n - 1)
				{
					rightEnd = leftStart + 2 * currentSize - 1;
				}
				else
				{
					rightEnd = n - 1;
				}
	
	            // Merge subarrays arr[leftStart..middle] and arr[middle+1..rightEnd]
	            Merge(arr, temp, leftStart, middle, rightEnd, comparator, ord);
	        }
	    }
	}

	// Function to merge two sorted subarrays arr[left..middle] and arr[middle+1..right]
	static private void Merge(array<T> arr, array<T> temp, int left, int middle, int right, IE_Cmp<T> comparator, IE_SortOrd ord) 
	{
	    int i = left;    // Index for left subarray
	    int j = middle + 1;  // Index for right subarray
	    int k = left;    // Index for merged subarray
	
	    // Merge elements from both subarrays into temp array
	    while (i <= middle && j <= right) {
			IE_CmpRes cmpRes = comparator.cmp(arr[i], arr[j]);
			
			bool insertFirst = (ord == IE_SortOrd.Increasing && cmpRes == IE_CmpRes.Lt) || (ord == IE_SortOrd.Decreasing && cmpRes == IE_CmpRes.Gt);
	        if (insertFirst)
	            temp[k++] = arr[i++];
	        else
	            temp[k++] = arr[j++];
	    }
	
	    // Copy remaining elements from left subarray to temp array
	    while (i <= middle)
	        temp[k++] = arr[i++];
	
	    // Copy remaining elements from right subarray to temp array
	    while (j <= right)
	        temp[k++] = arr[j++];
	
	    // Copy elements from temp back to original array
	    for (i = left; i <= right; i++)
	        arr[i] = temp[i];
	}
}

// #define TEST_ARRAY_UTILS

#ifdef TEST_ARRAY_UTILS
class TestArrayUtils
{
	static bool RunTests()
	{
		Print("RUNING ARRAY UTILS TESTS");
		EqEmptyArrays();
		EqArrays();
		NqArrays();
		BinarySearchEmpty();
		BinarySearch();
		SortEmpty();
		SortNonEmptyOdd();
		SortNonEmptyEven();
		SortNonEmptyOddDec();
		SortNonEmptyEvenDec();
		Print("FINALISED ARRAY UTILS TESTS");
		return true;
	}
	
	static void EqEmptyArrays()
	{
		array<int> arr1 = {};
		array<int> arr2 = {};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_CmpRes res = cmpArray.cmp(arr1, arr2);
		IE_Assert(res == IE_CmpRes.Eq, "[ERROR] Expected empty arrays to be equal");
	}
	
	static void EqArrays()
	{
		array<int> arr1 = {1, 2, 3};
		array<int> arr2 = {1, 2, 3};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_CmpRes res = cmpArray.cmp(arr1, arr2);
		IE_Assert(res == IE_CmpRes.Eq, "[ERROR] Expected arrays to be equal");
	}
	
	static void NqArrays()
	{
		array<int> arr1 = {1, 2, 4};
		array<int> arr2 = {1, 2, 3};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_CmpRes res = cmpArray.cmp(arr1, arr2);
		IE_Assert(res != IE_CmpRes.Eq, "[ERROR] Expected arrays to be non equal");
	}
	
	static void BinarySearchEmpty()
	{
		array<int> arr = {};
		auto search =  new IE_SearchMatch<int>(2, new IE_CmpInt);
		IE_BinSearchResult res = IE_ArrayUtils<int>.BinarySearch(arr, search);
		IE_Assert(!res.success, "[ERROR] Expected search on empty array to fail");
		IE_Assert(res.idx == 0, "[ERROR] Expected entry point on empty array to be 0");
	}
	
	static void BinarySearch()
	{
		array<int> arr = {1, 2, 3, 4, 5, 6};
		auto search =  new IE_SearchMatch<int>(2, new IE_CmpInt);
		IE_BinSearchResult res = IE_ArrayUtils<int>.BinarySearch(arr, search);
		IE_Assert(res.success, "[ERROR] Expected search to succeed");
		IE_Assert(res.idx == 1, "[ERROR] Expected entry point on empty array to be 1 but was " + res.idx);
	}

	static void BinarySearchNoMatch()
	{
		array<int> arr = {1, 2, 3, 5, 6};
		auto search =  new IE_SearchMatch<int>(4, new IE_CmpInt);
		IE_BinSearchResult res = IE_ArrayUtils<int>.BinarySearch(arr, search);
		IE_Assert(!res.success, "[ERROR] Expected search to succeed");
		IE_Assert(res.idx == 3, "[ERROR] Expected entry point on empty array to be 1 but was " + res.idx);
	}
	
	static void SortEmpty()
	{
		array<int> arr = {};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_ArrayUtils<int>.MergeSort(arr, new IE_CmpInt, IE_SortOrd.Increasing);
		IE_Assert(cmpArray.cmp(arr, {}) == IE_CmpRes.Eq, "[ERROR] Sort empty array");
	}
	
	static void SortNonEmptyOdd()
	{
		array<int> arr = {3, 1, 2};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_ArrayUtils<int>.MergeSort(arr, new IE_CmpInt, IE_SortOrd.Increasing);
		IE_Assert(cmpArray.cmp(arr, {1, 2, 3}) == IE_CmpRes.Eq, "[ERROR] Sort non empty odd array\nres: " + arr);
	}
	
	static void SortNonEmptyEven()
	{
		array<int> arr = {5, 3, 4, 1};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_ArrayUtils<int>.MergeSort(arr, new IE_CmpInt, IE_SortOrd.Increasing);
		IE_Assert(cmpArray.cmp(arr, {1, 3, 4, 5}) == IE_CmpRes.Eq, "[ERROR] Sort non empty even array\nres: " + arr);
	}
	
	static void SortNonEmptyOddDec()
	{
		array<int> arr = {3, 1, 2};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_ArrayUtils<int>.MergeSort(arr, new IE_CmpInt, IE_SortOrd.Decreasing);
		IE_Assert(cmpArray.cmp(arr, {3, 2, 1}) == IE_CmpRes.Eq, "[ERROR] Sort dec non empty odd array\nres: " + arr);
	}
	
	static void SortNonEmptyEvenDec()
	{
		array<int> arr = {5, 3, 4, 1};
		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_ArrayUtils<int>.MergeSort(arr, new IE_CmpInt, IE_SortOrd.Decreasing);
		IE_Assert(cmpArray.cmp(arr, {5, 4, 3, 1}) == IE_CmpRes.Eq, "[ERROR] Sort dec non empty even array\nres: " + arr);
	}
}

static bool arrayTestUtilsResults = TestArrayUtils.RunTests();

#ifdef