#pragma once
#include <string.h>
#include <sstream>
#include "LinkedList.h"
#include <algorithm>
#include <functional>
#include <vector>
#include  "cocos2d\external\win32-specific\icon\include\iconv.h"
#pragma execution_character_set("utf-8")
using namespace std;

#pragma warning(disable:4996)
using namespace std;

/*模仿java的String类，对C++的string改造*/
class mString :
	public string
{
public:
	mString() :string() {}
	mString(const mString &A) :string(A) {}
	mString(const char * S) :string(S) {}
	mString(const string & A) :string(A) {}
	//类型转换构造函数， 把string对象转换成String 使得string 的+ 可以用  和最后的（）可以用
	mString operator()(int n, int m) {
		return substr(n, m);
	}

	//LinkedList<mString>* supersplit(mString radix)
	//{
	//	LinkedList<mString>* res = new LinkedList<mString>;
	//	std::string::size_type pos1, pos2;
	//	size_t len = length();
	//	pos2 = find(radix);
	//	pos1 = 0;
	//	while (std::string::npos != pos2)
	//	{
	//		res->add(substr(pos1, pos2 - pos1));

	//		pos1 = pos2 + radix.size();
	//		pos2 = find(radix, pos1);
	//	}
	//	if (pos1 != len)
	//		res->add(substr(pos1));
	//	return res;
	//}
	LinkedList<mString>* supersplit(mString radix)
	{
		LinkedList<mString>* res = new LinkedList<mString>;
		std::string::size_type pos1, pos2;
		size_t len = length();
		pos2 = find(radix);
		pos1 = 0;
		while (std::string::npos != pos2)
		{
			mString x = substr(pos1, pos2 - pos1);
			if(x.length()>0)res->add(x);

			pos1 = pos2 + radix.size();
			pos2 = find(radix, pos1);
		}
		if (pos1 != len) {
			mString y = substr(pos1);
			if (y.length() > 0)res->add(y);
		}

		return res;
	}
	bool startswith(mString start)
	{
		int srclen = size();
		int startlen = start.size();
		if (srclen >= startlen)
		{
			mString temp = substr(0, startlen);
			if (temp == start)
				return true;
		}
		return false;
	}

	bool endswith(mString end)
	{
		int srclen = size();
		int endlen = end.size();
		if (srclen >= endlen)
		{
			mString temp = substr(srclen - endlen, endlen);
			if (temp == end)
				return true;
		}

		return false;
	}

	bool contains(mString x) {
		return find(x) != x.npos;
	}

	int asInt() {
		istringstream ss(*this);
		int x;
		ss >> x;
		return x;
	}

	static mString parseInt(int x) {
		stringstream ss;
		ss<<x;
		return ss.str();
	}

	string str() {
		string s = *this;
		return s;
	}

	const char* c_str() {
		return str().c_str();
	}

	mString subString(int begin, int end) {
		string s = *this;
		string s1 = s.substr(begin, end - begin + 1);
		return s1;
	}

	//int  indexOf(mString s) {
	//	return find(s);
	//}

	int  indexOf(mString s,int from) {
		return this->find(s,from);
	}
	mString replaceAll(mString org_str, mString rep_str) // 把org_str 替换为rep_str; 
	{
		auto   delimVec = this->supersplit(org_str); 
		if (delimVec->size() <= 0)
		{
			return (*this);
		}
		string target("");
		for (int i = 0; i < delimVec->size(); i++) {
			target = target + delimVec->get(i) + rep_str;
		}
		*this = target;
		return *this;
	}


	mString getStrWithMaxEms( int length)
	{
		if (length <= 0) return "";
		length += length % 4;
	    mString str = *this;
		unsigned int beginPos = 0;  //字符串的初始位置  
		mString resultStr="";      //返回的字符串  


		vector<mString> str_vec;  //创建一个字符串类型的顺序容器  
		do
		{
			str_vec.push_back(str.subString(beginPos, beginPos+length-1)); //substr函数的作用类似剪刀，将str中从beginPos到length之间的字符串剪下来，单独放入容器中   
			if (beginPos + length > str.length())
			{
				break;  //当要裁剪的长度超出str的长度，则退出循环  
			}
			else
			{
				beginPos += length;
			}

		} while (true);

		for (unsigned int i = 0; i < str_vec.size(); ++i)
		{
			mString radix = i == str_vec.size() - 1 ? "" : " \n";
			resultStr.append(str_vec.at(i)).append(radix); //从容器逐一取出之前裁剪好的一段段字符串，分别在字符串后面加上换行符。append()类似胶水，将\n粘到字符串后面   
		}
		return resultStr;
	}


	LinkedList<int>* indexesOf(mString s) {
		int a = 0;
		LinkedList<int> next;
		LinkedList<int>* get = new LinkedList<int>;
		//获得next
		next.add(-1);
		int b = -1;
		while (a < (int)s.length()) {
			if (b == -1 || s.at(a) == s.at(b)) {
				a++;
				b++;
				next.add(b);
			}
			else {
				b = next.get(b);
			}
		}
		int i = 0; int j = 0;
		while (i < (int)this->length()) {
			while ((i < (int)this->length()) && (j < (int)s.length())) {
				if (j == -1 || this->at(i) == s.at(j)) {
					i++;
					j++;
				}
				else {
					j = next.get(j);
				}
			}
			if (j >= (int)s.length()) {
				get->add(i - s.length());
				i = i - s.length() + 1;
				j = 0;
			}
		}
		return get;
	}
	int  indexOf(mString s) {
		int a = 0;
		LinkedList<int> next;
		next.add(-1);
		int b = -1;
		while (a < (int)s.length()) {
			if (b == -1 || s.at(a) == s.at(b)) {
				a++;
				b++;
				next.add(b);
			}
			else {
				b = next.get(b);
			}
		}
		int i = 0; int j = 0;
		while ((i < (int)this->length()) && (j < (int)s.length())) {
			if (j == -1 || this->at(i) == s.at(j)) {
				i++;
				j++;
			}
			else {
				j = next.get(j);
			}
		}
		if (j >= (int)s.length()) {
			return i - s.length();
		}
		else {
			return -1;
		}
	}

};

