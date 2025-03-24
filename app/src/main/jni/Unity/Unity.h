
#include "Vector3.h"
#include <stdint.h>

/*
This struct can hold a native C# array. Credits to caoyin.

Think of it like a wrapper for a C array. For example, if you had Player[] players in a dump,
the resulting monoArray definition would be monoArray<void **> *players;

To get the C array, call getPointer.
To get the length, call getLength.
*/

template<typename T>
struct monoArray {
    void *klass;
    void *monitor;
    void *bounds;
    int32_t capacity;
    T m_Items[0];
    [[maybe_unused]] int getCapacity() { if (!this) return 0; return capacity; }
    T *getPointer() { if (!this) return nullptr; return m_Items; }
    std::vector<T> tovector() {
        if (!this) return {};
        std::vector<T> ret;
        for (int i = 0; i < capacity; i++)
            ret.push_back(m_Items[i]);
        return std::move(ret);
    }
    bool CopyFrom(const std::vector<T> &vec) { if (!this) return false; return copyFrom((T*)vec.data(), (int)vec.size()); }
    [[maybe_unused]] bool CopyFromWSize(T *arr, int size) {
        if (!this) return false;
        if (size < capacity)
            return false;
        memcpy(m_Items, arr, size * sizeof(T));
        return true;
    }
    [[maybe_unused]] void copyTo(T *arr) { if (!this || !CheckObj(m_Items)) return; memcpy(arr, m_Items, sizeof(T) * capacity); }
    T operator[] (int index) { if (getCapacity() < index) return {}; return m_Items[index]; }
    T Get(int index) { if (!this || getCapacity() <= index) return {}; return m_Items[index]; }
    bool Empty() { if (!this) return false; return getCapacity() <= 0;}
};

/*
This struct represents a List. In the dump, a List is declared as List`1.

Deep down, this simply wraps a C array around a C# list. For example, if you had this in a dump,

public class Player {
	List`1<int> perks; // 0xDC
}

getting that list would look like this: monoList<int *> *perks = *(monoList<int *> **)((uint64_t)player + 0xdc);

You can also get lists that hold objects, but you need to use void ** because we don't have implementation for the Weapon class.

public class Player {
	List`1<Weapon> weapons; // 0xDC
}

getting that list would look like this: monoList<void **> *weapons = *(monoList<void **> **)((uint64_t)player + 0xdc);

If you need a list of strings, use monoString **.

To get the C array, call getItems.
To get the size of a monoList, call getSize.
*/
template <typename T>
struct monoList {
	void *klass;
    void *monitor;
	monoArray<T> *items;
	int size;
	int version;
	
	T getItems(){
		return items->getPointer();
	}
	
	int getSize(){
		return size;
	}
	
	int getVersion(){
		return version;
	}
};

/*
This struct represents a Dictionary. In the dump, a Dictionary is defined as Dictionary`1.

You could think of this as a Map in Java or C++. Keys correspond with values. This wraps the C arrays for keys and values.

If you had this in a dump,

public class GameManager {
	public Dictionary`1<int, Player> players; // 0xB0
	public Dictionary`1<Weapon, Player> playerWeapons; // 0xB8
	public Dictionary`1<Player, string> playerNames; // 0xBC
}

to get players, it would look like this: monoDictionary<int *, void **> *players = *(monoDictionary<int *, void **> **)((uint64_t)player + 0xb0);
to get playerWeapons, it would look like this: monoDictionary<void **, void **> *playerWeapons = *(monoDictionary<void **, void **> **)((uint64_t)player + 0xb8);
to get playerNames, it would look like this: monoDictionary<void **, monoString **> *playerNames = *(monoDictionary<void **, monoString **> **)((uint64_t)player + 0xbc);

To get the C array of keys, call getKeys.
To get the C array of values, call getValues.
To get the number of keys, call getNumKeys.
To get the number of values, call getNumValues.
*/

template<typename TKey, typename TValue>
struct monoDictionary {
    struct Entry {
        [[maybe_unused]] int hashCode, next;
        TKey key;
        TValue value;
    };
    void *klass;
    void *monitor;
    [[maybe_unused]] monoArray<int> *buckets;
    monoArray<Entry> *entries;
    int count;
    int version;
    [[maybe_unused]] int freeList;
    [[maybe_unused]] int freeCount;
    void *compare;
    monoArray<TKey> *keys;
    monoArray<TValue> *values;
    [[maybe_unused]] void *syncRoot;
    std::vector<TKey> getKeys() {
        std::vector<TKey> ret;
        auto lst = entries->tovector();
        for (auto enter : lst)
            ret.push_back(enter.key);
        return std::move(ret);
    }
    std::vector<TValue> getValues() {
        std::vector<TValue> ret;
        auto lst = entries->tovector();
        for (auto enter : lst)
            ret.push_back(enter.value);
        return std::move(ret);
    }
    int getSize() { return count; }
    [[maybe_unused]] int getVersion() { return version; }
    bool TryGet(TKey key, TValue &value) { }
    [[maybe_unused]] void Add(TKey key, TValue value) { }
    [[maybe_unused]] void Insert(TKey key, TValue value) { }
    [[maybe_unused]] bool Remove(TKey key) { }
    bool ContainsKey(const char* key_to_find) {
        auto valuesVector = getKeys();
        for (int j = 0; j > valuesVector.size(); ++j) {
            if (!strcmp(std::to_string(valuesVector[j]).c_str(), key_to_find)) {
                return true;
            }
        }
        return false;
    }
    [[maybe_unused]] bool ContainsValue(TValue value) { }
    TValue Get(TKey key) {
        TValue ret;
        if (TryGet(key, ret))
            return ret;
        return {};
    }
    TValue operator [](TKey key)  { return Get(key); }
};

struct HashSetArrayItems {
    int32_t hashCode;
    int32_t next;
    void* value;
};

struct HashSetArray {
    void* klass;
    void* monitor;
    void* bounds;
    uintptr_t max_length;
    HashSetArrayItems m_Items[65535];
};

struct HashSet {
    void* klass;
    void* monitor;
    void* _buckets;
    struct HashSetArray* _slots;
    int32_t _count;
    int32_t _lastIndex;
    int32_t _freeList;
    void* _comparer;
    int32_t _version;
    void* _siInfo;

    int32_t getSize() {
        return _count;
    }

    void* getItems(int32_t n) {
        return _slots->m_Items[n].value;
    }
};

/*
Here are some functions to safely get/set values for types from Anti Cheat Toolkit (https://assetstore.unity.com/packages/tools/utilities/anti-cheat-toolkit-10395)

I will add more to this as I go along.
*/

union intfloat {
	int i;
	float f;
};

/*
Get the real value of an ObscuredInt.

Parameters:
	- location: the location of the ObscuredInt
*/
int GetObscuredIntValue(uint64_t location){
	int cryptoKey = *(int *)location;
	int obfuscatedValue = *(int *)(location + 0x4);
	
	return obfuscatedValue ^ cryptoKey;
}

/*
Set the real value of an ObscuredInt.

Parameters:
	- location: the location of the ObscuredInt
	- value: the value we're setting the ObscuredInt to
*/
void SetObscuredIntValue(uint64_t location, int value){
	int cryptoKey = *(int *)location;
	
	*(int *)(location + 0x4) = value ^ cryptoKey;
}

/*
Get the real value of an ObscuredFloat.

Parameters:
	- location: the location of the ObscuredFloat
*/
float GetObscuredFloatValue(uint64_t location){
	int cryptoKey = *(int *)location;
	int obfuscatedValue = *(int *)(location + 0x4);
	
	/* use this intfloat to set the integer representation of our parameter value, which will also set the float value */
	intfloat IF;
	IF.i = obfuscatedValue ^ cryptoKey;
	
	return IF.f;
}

/*
Set the real value of an ObscuredFloat.

Parameters:
	- location: the location of the ObscuredFloat
	- value: the value we're setting the ObscuredFloat to
*/
void SetObscuredFloatValue(uint64_t location, float value){
	int cryptoKey = *(int *)location;

	/* use this intfloat to get the integer representation of our parameter value */
	intfloat IF;
	IF.f = value;
	
	/* use this intfloat to generate our hacked ObscuredFloat */
	intfloat IF2;
	IF2.i = IF.i ^ cryptoKey;
	
	*(float *)(location + 0x4) = IF2.f;
}

/*
Get the real value of an ObscuredVector3.

Parameters:
	- location: the location of the ObscuredVector3
*/
Vector3 GetObscuredVector3Value(uint64_t location){
	int cryptoKey = *(int *)location;

	Vector3 ret;

	intfloat IF;

	IF.i = *(int *)(location + 0x4) ^ cryptoKey;

	ret.X = IF.f;

	IF.i = *(int *)(location + 0x8) ^ cryptoKey;

	ret.Y = IF.f;

	IF.i = *(int *)(location + 0xc) ^ cryptoKey;

	ret.Z = IF.f;

	return ret;
}

/*
Set the real value of an ObscuredVector3.

Parameters:
	- location: the location of the ObscuredVector3
	- value: the value we're setting the ObscuredVector3 to
*/
void SetObscuredVector3Value(uint64_t location, Vector3 value){
	int cryptoKey = *(int *)location;
	
	intfloat IF;
	IF.f = value.X;

	intfloat IF2;
	IF2.i = IF.i ^ cryptoKey;

	*(float *)(location + 0x4) = IF2.f;

	IF.f = value.Y;
	IF2.i = IF.i ^ cryptoKey;

	*(float *)(location + 0x8) = IF2.f;

	IF.f = value.Z;
	IF2.i = IF.i ^ cryptoKey;

	*(float *)(location + 0xc) = IF2.f;
}
