#pragma once
/*二元组类*/
template<class T1,class T2>
class Pair
{
public:
	Pair();
	Pair(T1 x, T2 y);
	Pair(const Pair& other);
	T1 getX();
	T2 getY();
private:
	T1 x;
	T2 y;
};

template<class T1, class T2>
inline Pair<T1, T2>::Pair()
{
	
}

template<class T1, class T2>
inline Pair<T1, T2>::Pair(T1 x, T2 y)
{
	this->x = x;
	this->y  = y;
}

template<class T1, class T2>
inline Pair<T1, T2>::Pair(const Pair& other)
{
	x = other.x;
	y = other.y;
}

template<class T1, class T2>
inline T1 Pair<T1, T2>::getX()
{
	return x;
}

template<class T1, class T2>
inline T2 Pair<T1, T2>::getY()
{
	return y;
}
