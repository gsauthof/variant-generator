#include "variant.hh"

#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Serializer {
  private:
    ostream &o;
  public:
    Serializer(ostream &o)
      : o(o)
    {
    }
    void write(const std::string &s)
    {
      o << 'S' << ' ' << s.size() << ' ' << s;
    }
    void write(uint32_t i)
    {
      o << 'U' << ' ' << i;
    }
    void write(int32_t i)
    {
      o << 'I' << ' ' << i;
    }
};

struct Serialize_Visitor {
  typedef void result_type;
  Serializer &s;
  Serialize_Visitor(Serializer &s) : s(s) {}
  template <typename T> void operator()(const T &t) { s.write(t); }
};

static void write_all(Serializer &s, const deque<Variant> &ds)
{
  Serialize_Visitor sv(s);
  for (auto &d : ds)
    d.accept(sv);
}

static deque<Variant> read_all(istream &o);

int main(int argc, char **argv)
{
  assert(argc > 2);
  string in_filename(argv[1]);
  ifstream in(in_filename, ios::in | ios::binary);
  string out_filename(argv[2]);
  ofstream out(out_filename, ios::out | ios::binary);

  auto l = read_all(in);
  Serializer s(out);
  write_all(s, l);

  return 0;
}

static deque<Variant> read_all(istream &o)
{
  deque<Variant> r;
  char c = ' ';
  while (o >> c) {
    switch (c) {
      case 'S':
        {
          size_t n = 0;
          o >> n;
          string s(n, 0);
          o.seekg(1, ostream::cur);
          o.read(&*s.begin(), n);
          r.emplace_back(std::move(s));
        }
        break;
      case 'U':
        {
          uint32_t i = 0;
          o >> i;
          r.emplace_back(i);
        }
        break;
      case 'I':
        {
          int32_t i = 0;
          o >> i;
          r.emplace_back(i);
        }
        break;
    }
  }
  return r;
}
