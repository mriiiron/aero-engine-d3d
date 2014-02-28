#include <d3d11_1.h>
#include "AETable.h"

template <typename T>
AEHashedTable<T>::AEHashedTable(INT _maxElemCount) {
	maxElemCount = _maxElemCount;
	maxIndex = -1;
	pHash = 0;
	table = new T*[maxElemCount];
	occupied = new INT[maxElemCount];
	hash = new INT[maxElemCount];
	for (int i = 0; i < maxElemCount; i++) {
		table[i] = nullptr;
		occupied[i] = 0;
		hash[i] = -1;
	}
}

template <typename T>
AEHashedTable<T>::~AEHashedTable() {
	delete [] table;
	delete [] occupied;
	delete [] hash;
}

template <typename T>
VOID AEHashedTable<T>::add(T* t) {
	for (INT i = 0; i <= maxIndex; i++) {
		if (!occupied[i]) {
			table[i] = t;  occupied[i] = 1;
			hash[pHash] = i;  pHash++;
			return;
		}
	}
	if (maxIndex == maxElemCount - 1) {
		MessageBox(NULL, (LPCWSTR)L"On adding to hashed table: Too many items.", (LPCWSTR)L"Error", MB_OK);
	}
	else {
		maxIndex++;
		table[maxIndex] = t;  occupied[maxIndex] = 1;
		hash[pHash] = maxIndex;  pHash++;
	}
}

template <typename T>
VOID AEHashedTable<T>::addAt(INT index, T* t) {
	if (occupied[index]) {
		MessageBox(NULL, (LPCWSTR)L"On adding to hashed table: Slot occupied.", (LPCWSTR)L"Error", MB_OK);
		return;
	}
	if (index > maxIndex) {
		maxIndex = index;
	}
	table[index] = t;  occupied[index] = 1;
	hash[pHash] = maxIndex;  pHash++;
}

template <typename T>
VOID AEHashedTable<T>::remove(INT index) {
	if (!occupied[index]) {
		MessageBox(NULL, (LPCWSTR)L"On removing from hashed table: Slot empty.", (LPCWSTR)L"Error", MB_OK);
		return;
	}
	delete table[index];
	table[index] = NULL;
	occupied[index] = 0;
	while (!occupied[maxIndex] && maxIndex > 0) maxIndex--;
	for (int i = 0; i < pHash; i++) {
		if (hash[i] == index) {
			pHash--;
			hash[i] = hash[pHash];
			hash[pHash] = -1;
			break;
		}
	}
}

template <typename T>
VOID AEHashedTable<T>::clear() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i]) {
			occupied[i] = 0;
			remove(i);
		}
		hash[i] = -1;
	}
}

template <typename T>
AEConstantTable<T>::AEConstantTable(INT _maxElemCount) {
	maxElemCount = _maxElemCount;
	occupied = new INT[maxElemCount];
	for (INT i = 0; i < maxElemCount; i++) {
		occupied[i] = 0;
	}
}

template <typename T>
AEConstantTable<T>::~AEConstantTable() {
	delete [] table;
	delete [] occupied;
}

template <typename T>
VOID AEConstantTable<T>::addAt(INT index, T* t) {
	if (occupied[index]) {
		return;
	}
	table[index] = t;
	occupied[index] = 1;
}