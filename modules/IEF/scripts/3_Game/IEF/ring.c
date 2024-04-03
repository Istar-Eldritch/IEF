class IE_Ring<Class T>
{
	const int HEAD_IDX = 0;
	const int TAIL_IDX = 1;

	int m_head;
	int m_tail;
	int count;

	ref array<T> m_elements;

	void IE_Ring(int length = 10)
	{
		m_elements = new array<T>;
		if (length < 3)
			length = 3;
		m_elements.Resize(length);
		m_head = 0;
		m_tail = length - 1;
	}

	void MakeSpace()
	{
		int oldlen = m_elements.Count();
		int newlen = oldlen * 2;
		m_elements.Resize(newlen);

		for (int i = (oldlen - 1); i >= m_tail; i--)
		{
			T item = m_elements.Get(i);
			m_elements.Set(newlen - 1 - (oldlen - i), item);
		}
		m_tail = newlen - 1 - (oldlen - 1 - m_tail);
	}

	void PushHead(T elem)
	{
		m_elements.Set(m_head, elem);
		count++;
		IncreaseIdx(HEAD_IDX);
		if (m_head == m_tail)
		{
			MakeSpace();
		}
	}

	int Count()
	{
		return count;
	}

	T PopHead()
	{
		T elem;
		DecreaseIdx(HEAD_IDX);
		elem = m_elements.Get(m_head);
		count--;
		return elem;
	}

	void PushTail(T elem)
	{
		m_elements.Set(m_tail, elem);
		count++;
		DecreaseIdx(TAIL_IDX);
		if (m_head == m_tail)
		{
			MakeSpace();
		}
	}

	T PopTail()
	{
		T elem;
		IncreaseIdx(TAIL_IDX);
		elem = m_elements.Get(m_tail);
		count--;
		return elem;
	}

	private void IncreaseIdx(int idx)
	{
		switch (idx)
		{
			case TAIL_IDX:
				if (m_tail == m_elements.Count() - 1)
					m_tail = 0;
				else
					m_tail++;
				break;
			case HEAD_IDX:
				if (m_head == m_elements.Count() - 1)
					m_head = 0;
				else
					m_head++;
				break;
		}
	}

	private void DecreaseIdx(int idx)
	{
		switch (idx)
		{
			case TAIL_IDX:
				if (m_tail == 0)
					m_tail = m_elements.Count() - 1;
				else
					m_tail--;
				break;
			case HEAD_IDX:
				if (m_head == 0)
					m_head = m_elements.Count() - 1;
				else
					m_head--;
				break;
		}
	}
}


// #define TEST_RING

#ifdef TEST_RING
class RingTest
{
	static bool RunTests()
	{
		Print("RUNING RING TESTS");
		TestRingLenght();
		TestPushPop();
		Print("FINALISED RING TESTS");
		return true;
	}

	static void TestRingLenght()
	{
		IE_Ring<int> ring = new IE_Ring<int>(3);

		IE_CmpArrayInt cmpArray = new IE_CmpArrayInt;
		IE_Assert(ring.m_elements.Count() == 3, "[ERROR] Expected ring to be initialized to size");

		IE_Assert(ring.Count() == 0, "[ERROR] Expected ring size to be 0 without items: " + ring.Count());

		ring.PushHead(1);
		IE_Assert(ring.m_elements.Count() == 3, "[ERROR] Expected ring to be initialized to same size after insert");
		IE_Assert(ring.Count() == 1, "[ERROR] Expected ring size to be 1 with one items: " + ring.Count());

		ring.PushTail(2);
		IE_Assert(ring.m_elements.Count() == 6, "[ERROR] Expected ring to be initialized to double size");
		IE_Assert(ring.Count() == 2, "[ERROR] Expected ring size to be 2 with two items: " + ring.Count());
	}

	static void TestPushPop()
	{
		IE_Ring<int> ring = new IE_Ring<int>(3);

		ring.PushHead(1);
		ring.PushHead(3);
		IE_Assert(ring.PopHead() == 3, "[ERROR] Expected head element to be 3");
		IE_Assert(ring.PopHead() == 1, "[ERROR] Expected head element to be 1");

		ring.PushHead(4);
		ring.PushHead(5);
		IE_Assert(ring.PopTail() == 4, "[ERROR] Expected tail element to be 4");
		IE_Assert(ring.PopTail() == 5, "[ERROR] Expected tail element to be 5");

		ring.PushTail(6);
		ring.PushTail(7);
		IE_Assert(ring.PopHead() == 6, "[ERROR] Expected head element to be 6");
		IE_Assert(ring.PopHead() == 7, "[ERROR] Expected head element to be 7");

		ring.PushTail(8);
		ring.PushTail(9);
		ring.PushTail(10);
		IE_Assert(ring.PopHead() == 8, "[ERROR] Expected head element to be 8");
		IE_Assert(ring.PopTail() == 10, "[ERROR] Expected tail element to be 10");
		IE_Assert(ring.PopHead() == 9, "[ERROR] Expected head element to be 9");
	}

}

static bool ringTestResults = RingTest.RunTests();

#endif