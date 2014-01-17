#pragma once


template <typename T, INT MAX_ELEM_COUNT>
class AEHashedTable {

public:

	AEHashedTable();
	T* getItem(INT index) { return table[index]; }
	T* getItemByHash(INT hashIndex) { return table[hash[hashIndex]]; }
	INT getHash(INT hashIndex) { return hash[hashIndex]; }
	INT getHashCount() { return pHash; }
	VOID add(T* t);
	VOID addAt(INT index, T* t);
	VOID remove(INT index);
	VOID clear();

private:

	T* table[MAX_ELEM_COUNT];
	INT occupied[MAX_ELEM_COUNT];
	INT hash[MAX_ELEM_COUNT];
	INT maxIndex, pHash;

};


template <typename T, INT MAX_ELEM_COUNT>
class AEConstantTable {

public:

	AEConstantTable();
	VOID addAt(INT index, T* t);
	T* get(INT index) { return table[index]; }

private:

	T* table[MAX_ELEM_COUNT];
	INT occupied[MAX_ELEM_COUNT];

};


template <typename T, INT MAX_ELEM_COUNT>
AEHashedTable<T, MAX_ELEM_COUNT>::AEHashedTable() {
	maxIndex = -1;
	pHash = 0;
	for (int i = 0; i < MAX_ELEM_COUNT; i++) {
		occupied[i] = 0;
		hash[i] = -1;
	}
}

template <typename T, INT MAX_ELEM_COUNT>
VOID AEHashedTable<T, MAX_ELEM_COUNT>::add(T* t) {
	for (INT i = 0; i <= maxIndex; i++) {
		if (!occupied[i]) {
			table[i] = t;  occupied[i] = 1;
			hash[pHash] = i;  pHash++;
			return;
		}
	}
	if (maxIndex == MAX_ELEM_COUNT - 1) {
		MessageBox(NULL, (LPCWSTR)L"On adding to hashed table: Too many items.", (LPCWSTR)L"Error", MB_OK);
	}
	else {
		maxIndex++;
		table[maxIndex] = t;  occupied[maxIndex] = 1;
		hash[pHash] = maxIndex;  pHash++;
	}
}

template <typename T, INT MAX_ELEM_COUNT>
VOID AEHashedTable<T, MAX_ELEM_COUNT>::addAt(INT index, T* t) {
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

template <typename T, INT MAX_ELEM_COUNT>
VOID AEHashedTable<T, MAX_ELEM_COUNT>::remove(INT index) {
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

template <typename T, INT MAX_ELEM_COUNT>
VOID AEHashedTable<T, MAX_ELEM_COUNT>::clear() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			remove(i);
	}
}

template <typename T, INT MAX_ELEM_COUNT>
AEConstantTable<T, MAX_ELEM_COUNT>::AEConstantTable() {
	for (INT i = 0; i < MAX_ELEM_COUNT; i++) {
		occupied[i] = 0;
	}
}

template <typename T, INT MAX_ELEM_COUNT>
VOID AEConstantTable<T, MAX_ELEM_COUNT>::addAt(INT index, T* t) {
	if (occupied[index]) {
		return;
	}
	table[index] = t;
	occupied[index] = 1;
}