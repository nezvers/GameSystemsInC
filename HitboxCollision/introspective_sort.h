#ifndef INTROSPECTIVE_SORT_H
#define INTROSPECTIVE_SORT_H

void IntroSort(int* data, int count);

#endif //INTROSPECTIVE_SORT_H

#ifdef INTROSPECTIVE_SORT_IMPLEMENTATION
#undef INTROSPECTIVE_SORT_IMPLEMENTATION

#insert "math.h"

void InsertionSort(int* data, int count) {
	int temp;
    for (int i = 1; i < count; ++i)
	{
		int j = i;

		while (j > 0)
		{
			if (data[j - 1] > data[j])
			{
				temp = data[j - 1];
                data[j - 1] = data[j];
				data[j] = temp;

				--j;
			}
			else
			{
				break;
			}
		}
	}
}

void MaxHeapify(int* data, int heapSize, int index) {
	int left = (index + 1) * 2 - 1;
	int right = (index + 1) * 2;
	int largest = 0;

	if (left < heapSize && data[left] > data[index]){
		largest = left;
    }
	else{
		largest = index;
    }

	if (right < heapSize && data[right] > data[largest]){
		largest = right;
    }

	if (largest != index)
	{
		int temp = data[index];
		data[index] = data[largest];
		data[largest] = temp;

		MaxHeapify(data, heapSize, largest);
	}
}

void HeapSort(int* data, int count) {
	int heapSize = count;

	for (int p = (heapSize - 1) / 2; p >= 0; --p){
		MaxHeapify(data, heapSize, p);
    }

	for (int i = count - 1; i > 0; --i)
	{
		int temp = data[i];
		data[i] = data[0];
		data[0] = temp;

		--heapSize;
		MaxHeapify(data, heapSize, 0);
	}
}


int Partition(int* data, int left, int right) {
	int pivot = data[right]; // value of the last
	int temp;
	int a = left;

	for (int i = left; i < right; ++i)
	{
		// if i value smaller than right value
        if (data[i] <= pivot)
		{
			// swap i with a
            temp = data[i];
			data[i] = data[a];
			data[a] = temp;
			a++;
		}
	}

	data[right] = data[a];
	data[a] = pivot;

	return a;
}

void QuickSortRecursive(int* data, int left, int right) {
	if (left < right)
	{
		int q = Partition(data, left, right);
		QuickSortRecursive(data, left, q - 1);
		QuickSortRecursive(data, q + 1, right);
	}
}

void IntroSort(int* data, int count) {
	int partitionSize = Partition(data, 0, count - 1);

	if (partitionSize < 16)
	{
		InsertionSort(data, count);
	}
	else if (partitionSize >(2 * log(count)))
	{
		HeapSort(data, count);
	}
	else
	{
		QuickSortRecursive(data, 0, count - 1);
	}
}
#endif //INTROSPECTIVE_SORT_IMPLEMENTATION

