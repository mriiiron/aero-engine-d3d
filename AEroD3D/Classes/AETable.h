#pragma once

#include "AEGameControl.h"


template <typename T>
class AEHashedTable {

public:

	AEHashedTable(INT _maxElemCount);
	~AEHashedTable();

	T* getItem(INT index) { return table[index]; }
	T* getItemByHash(INT hashIndex) { return table[hash[hashIndex]]; }
	INT getHash(INT hashIndex) { return hash[hashIndex]; }
	INT getHashCount() { return pHash; }
	VOID add(T* t);
	VOID addAt(INT index, T* t);
	VOID remove(INT index);
	VOID clear();

protected:

	T** table;
	BOOLEAN* occupied;
	INT* hash;
	INT maxIndex, maxElemCount, pHash;

};


template <typename T>
class AEConstantTable {

public:

	AEConstantTable(INT _maxElemCount);
	~AEConstantTable();

	VOID addAt(INT index, T* t);
	T* getItem(INT index) { return table[index]; }
	INT getMaxElemCount() { return maxElemCount; }
	BOOLEAN isOccupied(INT index) { return occupied[index]; }

protected:

	T** table;
	BOOLEAN* occupied;
	INT maxElemCount;

};


template <typename T>
AEHashedTable<T>::AEHashedTable(INT _maxElemCount) {
	maxElemCount = _maxElemCount;
	maxIndex = -1;
	pHash = 0;
	table = new T*[maxElemCount];
	occupied = new BOOLEAN[maxElemCount];
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
		AENSGameControl::exitGame("On adding to hashed table: Too many items.");
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
		AENSGameControl::exitGame("On adding to hashed table: Slot occupied.");
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
		AENSGameControl::exitGame("On removing from hashed table: Slot empty.");
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
	table = new T*[maxElemCount];
	occupied = new BOOLEAN[maxElemCount];
	for (INT i = 0; i < maxElemCount; i++) {
		table[i] = nullptr;
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
		AENSGameControl::exitGame("On adding to constant table: Slot occupied.");
	}
	table[index] = t;
	occupied[index] = 1;
}