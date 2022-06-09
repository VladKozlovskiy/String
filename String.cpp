#include "String.hpp"

String::String(size_t size, char character = '\0') {
  size_ = size;
  capacity_ = size_;
  string_ = new char[size_];
  memset(string_, character, size_ * sizeof(char));
}

String::String(const String& s) {
  size_ = s.size_;
  capacity_ = size_;
  string_ = new char[size_];
  memcpy(string_, s.string_, size_ * sizeof(char));
}

String::String(const char* s) {
  size_ = strlen(s);
  capacity_ = size_ + 1;
  string_ = new char[capacity_];
  memcpy(string_, s, (capacity_) * sizeof(char));
}

void String::Swap(String& s) {
  std::swap(size_, s.size_);
  std::swap(capacity_, s.capacity_);
  std::swap(s.string_, string_);
}

String& String::operator=(String s) {
  Swap(s);
  return *this;
}

void String::Resize(size_t new_size, char character) {
  if (size_ > new_size) {
    size_ = new_size;
    memset(string_ + size_, '\0', (capacity_ - size_) * sizeof(char));
  } else if (size_ < new_size) {
    if (capacity_ > new_size) {
      memset(string_ + size_, '\0', (new_size - size_) * sizeof(char));
      size_ = new_size;
    } else {
      String tmp(2 * new_size, character);
      memcpy(tmp.string_, string_, size_ * sizeof(char));
      Swap(tmp);
      capacity_ = 2 * new_size;
      size_ = new_size;
    }
  }
}

void String::PushBack(char character) {
  if (capacity_ == size_) {
    Reserve(2 * capacity_ + 1);
  }
  string_[size_] = character;
  ++size_;
}

void String::PopBack() {
  if (!Empty()) {
    string_[size_ - 1] = '\0';
    --size_;
  }
}

bool String::Empty() const { return size_ == 0; }

size_t String::Capacity() const { return capacity_; }

size_t String::Size() const { return size_; }

const char* String::Data() const { return string_; }

void String::Reserve(size_t new_cap) {
  if (new_cap > capacity_) {
    String tmp;
    tmp.size_ = size_;
    tmp.capacity_ = new_cap;
    tmp.string_ = new char[new_cap];
    memcpy(tmp.string_, string_, size_ * sizeof(char));
    Swap(tmp);
  }
}

void String::ShrinkToFit() {
  if (capacity_ != size_) {
    String tmp;
    tmp.size_ = size_;
    tmp.capacity_ = size_;
    tmp.string_ = new char[size_];
    memcpy(tmp.string_, string_, size_ * sizeof(char));
    Swap(tmp);
  }
}

void String::Clear() {
  memset(string_, '\0', size_ * sizeof(char));
  size_ = 0;
}

String::~String() { delete[] string_; }

bool String::operator==(const String& s) const {
  return !(*this > s || s > *this);
}
bool String::operator<(const String& s) const {
  int cnt = 0;
  while ((cnt != size_) && (cnt != s.size_) &&
         *(s.string_ + cnt) == *(string_ + cnt)) {
    ++cnt;
  }
  if (cnt == size_ && cnt == s.size_) {
    return false;
  }
  if (cnt == size_) {
    return true;
  }
  if (cnt == s.size_) {
    return false;
  }
  return s.string_[cnt] > string_[cnt];
}

bool String::operator>(const String& s) const { return (s < *this); }

bool String::operator!=(const String& s) const {
  return ((*this < s) || (*this > s));
}

bool String::operator<=(const String& s) const {
  return (*this < s) || (*this == s);
}

bool String::operator>=(const String& s) const {
  return (*this > s) || (*this == s);
}

char& String::operator[](size_t i) { return *(string_ + i); }

char String::operator[](size_t i) const { return *(string_ + i); }

char& String::Front() { return *string_; }

char String::Front() const { return *string_; }

char& String::Back() { return *(string_ + size_ - 1); }

char String::Back() const { return *(string_ + size_ - 1); }

String& String::operator+=(const String& s) {
  size_t old_size = size_;
  Resize(s.size_ + size_);
  memcpy((string_ + old_size), s.string_, s.size_ * sizeof(char));
  return *this;
}

String operator+(const String& left, const String& right) {
  String res = left;
  res += right;
  return res;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> substrings;
  String substr;
  size_t i = 0;
  while (size_ != 0 && i <= size_ - delim.size_) {
    String tmp(delim.size_);
    memcpy(tmp.string_, string_ + i, delim.size_ * sizeof(char));
    if (tmp == delim) {
      substrings.push_back(substr);
      substr.Clear();
      i += delim.size_;
    } else {
      String new_char(1);
      memcpy(new_char.string_, string_ + i, sizeof(char));
      substr += new_char;
      ++i;
    }
  }
  if (i != Size()) {
    String tmp(Size() - i);
    memcpy(tmp.string_, string_ + i, (Size() - i) * sizeof(char));
    if (tmp != delim) {
      substr += tmp;
    }
  }
  substrings.push_back(substr);
  return substrings;
}

String& String::operator*=(size_t n) {
  String tmp;
  for (size_t i = 0; i < n; ++i) {
    tmp += *this;
  }
  Swap(tmp);
  return *this;
}

String operator*(const String& str, size_t n) {
  String tmp = str;
  tmp *= n;
  return tmp;
}

String String::Join(const std::vector<String>& strings) const {
  String res;
  if (strings.empty()) {
    return res;
  }
  for (size_t i = 0; i < strings.size() - 1; ++i) {
    res += strings[i];
    res += *this;
  }
  res += strings.back();
  return res;
}

std::ostream& operator<<(std::ostream& ostream, const String& str) {
  for (int i = 0; i < str.Size(); ++i) {
    ostream << str.Data()[i];
  }
  return ostream;
}

std::istream& operator>>(std::istream& istream, String& str) {
  str.Clear();
  char c = '0';
  while (istream.get(c) && c != '\n') {
    str.PushBack(c);
  }
  return istream;
}
