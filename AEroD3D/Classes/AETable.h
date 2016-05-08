#pragma once

#include "AEGameControl.h"


#define MEMORY_RELEASE 0
#define MEMORY_NOT_RELEASE 1


template <typename T>
class AEHashedTable {

public:

    AEHashedTable(INT _maxElemCount);
    ~AEHashedTable();

    T* getItem(INT index) { return table[index]; }
    T* getItemByHash(INT hashIndex) { return table[hash[hashIndex]]; }
    INT getHash(INT hashIndex) { return hash[hashIndex]; }
    INT getHashCount() { return pHash; }
    INT getSize() { return maxElemCount; }
    INT add(T* t);
    VOID addAt(INT index, T* t);
    VOID removeItem(INT index, INT memoryOption = MEMORY_RELEASE);
    VOID removeItemByHash(INT hashIndex, INT memoryOption = MEMORY_RELEASE);
    VOID clear();

protected:

    T** table;
    BOOLEAN* occupied;
    INT* hash;
    INT maxIndex, maxElemCount, pHash;

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
INT AEHashedTable<T>::add(T* t) {
    for (INT i = 0; i <= maxIndex; i++) {
        if (!occupied[i]) {
            table[i] = t;  occupied[i] = 1;
            hash[pHash] = i;  pHash++;
            return i;
        }
    }
    if (maxIndex >= maxElemCount - 1) {
        AENSGameControl::exitGame("On adding to hashed table: Too many items.");
        return -1;
    }
    else {
        maxIndex++;
        table[maxIndex] = t;  occupied[maxIndex] = 1;
        hash[pHash] = maxIndex;  pHash++;
        return maxIndex;
    }
}

template <typename T>
VOID AEHashedTable<T>::addAt(INT index, T* t) {
    if (occupied[index]) {
        AENSGameControl::exitGame("On adding to hashed table: Slot occupied.");
        return -1;
    }
    if (index >= maxElemCount) {
        AENSGameControl::exitGame("On adding to hashed table: Exceeding limit.");
        return -1;
    }
    if (index > maxIndex) {
        maxIndex = index;
    }
    table[index] = t;  occupied[index] = 1;
    hash[pHash] = maxIndex;  pHash++;
}

template <typename T>
VOID AEHashedTable<T>::removeItem(INT index, INT memoryOption) {
    if (!occupied[index]) {
        AENSGameControl::exitGame("On removing from hashed table: Slot empty.");
    }
    if (memoryOption == MEMORY_RELEASE) delete table[index];  // DANGER!!! MAKE SURE YOUR MEMORY IS PROPERLY RELEASED!!! 
    table[index] = nullptr;
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
VOID AEHashedTable<T>::removeItemByHash(INT hashIndex, INT memoryOption) {
    removeItem(getHash(hashIndex), memoryOption);
}

template <typename T>
VOID AEHashedTable<T>::clear() {
    for (int i = 0; i <= maxIndex; i++) {
        if (occupied[i]) {
            removeItem(i);
            occupied[i] = 0;
        }
        table[i] = nullptr;
        hash[i] = -1;
    }
    maxIndex = -1;
    pHash = 0;
}


template <typename T>
class AEConstantTable {

public:

    AEConstantTable(INT _maxElemCount);
    ~AEConstantTable();

    INT add(T* t);
    VOID addAt(INT index, T* t);
    T* getItem(INT index) { return table[index]; }
    INT getMaxElemCount() { return maxElemCount; }
    BOOLEAN isOccupied(INT index) { return occupied[index]; }

protected:

    T** table;
    BOOLEAN* occupied;
    INT maxElemCount, maxIndex;

};


template <typename T>
AEConstantTable<T>::AEConstantTable(INT _maxElemCount) {
    maxElemCount = _maxElemCount;
    maxIndex = -1;
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
INT AEConstantTable<T>::add(T* t) {
    for (INT i = 0; i <= maxIndex; i++) {
        if (!occupied[i]) {
            table[i] = t;  occupied[i] = TRUE;
            return i;
        }
    }
    if (maxIndex >= maxElemCount - 1) {
        AENSGameControl::exitGame("On adding to constant table: Too many items.");
        return -1;
    }
    else {
        maxIndex++;
        table[maxIndex] = t;  occupied[maxIndex] = 1;
        return maxIndex;
    }
}

template <typename T>
VOID AEConstantTable<T>::addAt(INT index, T* t) {
    if (occupied[index]) {
        AENSGameControl::exitGame("On adding to constant table: Slot occupied.");
    }
    if (index >= maxElemCount || index < 0) {
        AENSGameControl::exitGame("On adding to constant table: Index out of range.");
    }
    table[index] = t;
    occupied[index] = 1;
    if (index > maxIndex) {
        maxIndex = index;
    }
}