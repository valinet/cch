#ifndef __STRINGVIEW_H__
#define __STRINGVIEW_H__

#include <assert.h>  // for assert()
#include <iostream>  // for ostream
#include <string.h>  // for strlen, memcmp, memchr
#include <string>

using namespace std;

// Provides an immutable view on a span of memory.
// The standard use case is to access slices
// of a backing string without any string creation.
//
class StringView {
    const char* mData;
    size_t mSize;
    std::string mOwned;

    StringView(const char* data, size_t size)
        : mData(data), mSize(size) {
    }

public:
    StringView(const char* str)
        : mData(str), mSize(strlen(str)) {
    }

    StringView(const string& str)
        : mData(str.data()), mSize(str.size()) {
    }

    // Return a subslice of this view.
    // Note: this takes start and end positions,
    //   NOT a start position and length of slice.
    StringView slice(size_t start, size_t end) const {
        assert(start <= end);
        assert(end <= mSize);
        return StringView(mData + start, end - start);
    }

    const char* data() const { return mData; }

    size_t size() const { return mSize; }

    const char& operator[](size_t i) const {
        return mData[i];
    }

    bool operator==(const StringView& str) const {
        return mSize == str.mSize
            && ::memcmp(mData, str.mData, mSize) == 0;
    }
    bool operator!=(const StringView& str) const {
        return !(*this == str);
    }

    bool operator==(const char* str) const {
        size_t i = 0;
        while (*str != '\0'
               && i < mSize
               && mData[i] == *str) {
            str++;
            i++;
        }
        return (*str == '\0' && i == mSize);
    }
    bool operator!=(const char* str) const {
        return !(*this == str);
    }

    bool find(unsigned char c, size_t* offset) const {
        const void* loc = ::memchr(mData, c, mSize);
        if (loc == NULL) {
            return false;
        }
        *offset = ((const char*)loc - mData);
        return true;
    }

    size_t find(unsigned char c) const {
        const char* loc = (const char*) ::memchr(mData, c, mSize);
        if (loc == NULL) {
            return ~0;
        }
        return (loc - mData);
    }

    size_t reverseFind(unsigned char c, size_t from) {
        for (size_t i = 1; i <= from; ++i) {
            if (mData[from - i] == c) {
                return from - i;
            }
        }
        return ~0;
    }
    bool subStrEquals(size_t begin, size_t end, const char* str) const {
        assert(begin <= end);
        assert(end <= mSize);

        size_t len = end - begin;
        size_t strLen = strlen(str);
        if (len != strLen) {
            return false;
        }
        return ::memcmp(mData + begin, str, len) == 0;
    }

    void replace(size_t begin, size_t end, const char* replaceWith, size_t replaceWithSize)
    {

        assert(begin <= end);
        assert(end <= mSize);

        size_t removed = end - begin;

        if (replaceWithSize == removed) {
            if (replaceWithSize > 0) {
                char* dest = const_cast<char*>(mData) + begin;
                ::memcpy(dest, replaceWith, replaceWithSize);
            }
            return;
        }

        string result;
        result.reserve(mSize - removed + replaceWithSize);

        if (begin > 0) {
            result.append(mData, begin);
        }

        if (replaceWithSize > 0) {
            result.append(replaceWith, replaceWithSize);
        }

        if (end < mSize) {
            result.append(mData + end, mSize - end);
        }

        mOwned.swap(result);
        mData = mOwned.data();
        mSize = mOwned.size();
    }

    void substitute(const std::string& buf) {
        mOwned = buf;
        mData = mOwned.data();
        mSize = mOwned.size();
    }

    bool StartsWith(const string& str) {
        return string(mData).starts_with(str);
    }

    string toString() const {
        return string(mData, mData + mSize);
    }
};

// Allow for writing a StringView directly to an ostream.
ostream& operator<< (ostream& out, const StringView& s);

#endif //__STRINGVIEW_H__
