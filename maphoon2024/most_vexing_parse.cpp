
template<bool> struct a_t;

template<> struct a_t<true> {
    template<int> struct b {};
};

template<> struct a_t<false> {
    enum { b };
};

typedef a_t<sizeof(void*)==sizeof(int)> a;

enum { c, d };
int main() {
    a::b<c>d; // declaration or expression?
}


