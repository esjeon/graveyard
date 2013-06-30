/* kmedian.cpp - Finds k-median. A template function with comparator support.
 *               uses C++11
 *
 * Copyright © 2013 Eon S. Jeon <esjeon@live.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
struct Cmp {
    bool operator() (T p, T q) const { return p < q; }
};

template <typename T>
struct PtrCmp {
    bool operator() (T *p, T *q) const { return *p < *q; }
};

template <class T, class ElemCmp>
struct ArrayElemCmp {
    ElemCmp cmp;
    int idx;
    ArrayElemCmp (int idx) : cmp(), idx(idx) {}

    bool operator() (const T &p, const T &q) const
    {
        return cmp (p[idx], q[idx]);
    }
};

template <typename T>
inline T& self (T inst)
{
    T *p = &inst;
    return *p;
}

template <class Comparator, typename T, template<typename...> class Container, typename... Args>
T KMedian (const Container<T, Args...> &A, int K, const Comparator &cmp = self(Comparator()))
{
    const int mid = A.size()/2;
    auto &p = A[mid];

    Container<T, Args...> S, B;
    for (auto i = 0; i < A.size(); i++)
    {
        if (i == mid) continue;

        if (cmp (A[i], p))
            S.push_back (A[i]);
        else 
            B.push_back (A[i]);
    }

    if (S.size() == K-1)
        return p;
    else if (S.size() < K-1)
        return KMedian<Comparator> (B, K - S.size() - 1, cmp);
    else
        return KMedian<Comparator> (S, K, cmp);
}

int main ()
{
    {
        vector<int> A = {6, 4, 5, 2, 1, 3, 7};
        cout << KMedian<Cmp<int>> (A, (A.size()+1)/2) << endl;
    }

    {
        vector<int*> A = {new int(6), new int(4), new int(5), new int(2), new int(1), new int(3), new int(7)};
        cout << *(KMedian<PtrCmp<int>> (A, (A.size()+1)/2)) << endl;
    }

    {
        vector<vector<int>> A = { {6,1}, {4,2}, {5,3}, {2,4}, {1,5}, {3,6}, {7,7} };
        auto result = KMedian (A, (A.size()+1)/2, ArrayElemCmp<vector<int>,Cmp<int>>(0));
        cout << '{' << result[0] << ", " << result[1] << '}' << endl;
    }
}

