#pragma once
/*三元组类*/
template<class T1, class T2,class T3>
class Trine
{
public:
    Trine();
    Trine(T1 X, T2 Y, T3 Z) {
        x = X;
        y = Y;
        z = Z;
    }
    T1 getX();
    T2 getY();
    T3 getZ();

private:
    T1 x;
    T2 y;
    T3 z;
};

template<class T1, class T2, class T3>
inline Trine<T1, T2, T3>::Trine()
{
}

template<class T1, class T2, class T3>
inline T1 Trine<T1, T2, T3>::getX()
{
    return x;
}

template<class T1, class T2, class T3>
inline T2 Trine<T1, T2, T3>::getY()
{
    return y;
}

template<class T1, class T2, class T3>
inline T3 Trine<T1, T2, T3>::getZ()
{
    return z;
}
