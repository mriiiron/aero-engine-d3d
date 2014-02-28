#pragma once


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
	INT maxElemCount, currMaxIndex, pHash;

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