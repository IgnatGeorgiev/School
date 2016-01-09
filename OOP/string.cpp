#include <iostream>
#include <cstring>
#include <ostream>

using namespace std;
class IndexOutOfBoundsError {};

class String {
    char* buffer_;
    int size_;
    int capacity_;
    static const int CHUNK_ = 10;

    friend ostream& operator << (ostream&, String&);

    void ensure_capacity (int resize_length)
    {
        if(size_ + resize_length > capacity_)
        {
            resize(resize_length + CHUNK_);
        }
    }
public:
    String(const char* buffer) {
        size_ = strlen(buffer);
        capacity_ = size_ + CHUNK_;
        buffer_ = new char[capacity_];
        strcopy(buffer_,buffer);
    }
    
}