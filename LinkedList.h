#pragma once
template<class T>
/*万能的线性链表类*/
class LinkedList {
	struct Item { // 链表结点
		Item(T  d, Item* nxt = 0, Item* pre = 0) : 
			data(d), next(nxt), prev(pre) {
		}
		T  data;
		Item* next;
		Item* prev;
	};
	int len; // 长度
	Item* head; // 链表的头指针
	Item* tail; // 链表的尾指针
	LinkedList(const LinkedList& other) {
		clear();
		addAll(other);
	}
public:
	LinkedList() :
		len(0), head(0), tail(0) {
	}
	virtual ~LinkedList() {
		if (len != 0) {
			while (head->next != 0) {
				Item* item = head;
				head = item->next;
				delete item;
				item = 0;
			}
		}
	}
	bool add(T  e); // 增加元素
	int indexOf(T  e); // 返回元素位置
	bool contains(T e); //判断是否包含
	bool isEmpty(); // 判断容器是否为空
	void removeAll(LinkedList<T>* other); //删除other中包含的所有元素
	void removeAll_delete(LinkedList<T>* other);//删除并释放
	int size(); // 获取容器容量
	void add(int index, T  e); // 增加元素
	T  get(int index); // 获取元素
	void clear(); //清空表
	T  remove(int index); //删除某位置的元素
	bool remove(T  e, T e2); // 删除元素
	void swap(int x, int y); //交换两元素
	void addAll(LinkedList<T>* arr); //加入一整个表
	void addAll(int pos, LinkedList<T>* arr); //在某位置加入一整个表
	void transportAll(LinkedList<T>* arr); //加入一整个表，并把那个表掏空
	T dequeue(); //出队列
	void enqueue(T data); //入队列
	void push(T data); //入栈
	T pop(); //出栈
	// 删除元素
	class Iterator;
	friend class Iterator;
	class Iterator {
		LinkedList* ll;
		int index;
		Item* pi;
	public:
		Iterator(LinkedList* list) :
			ll(list), index(0),pi(list->head) {
		}
		bool hasNext() {
			return index < ll->len;
		}
		T  next() {
			if (hasNext()) {
				Item* tp = pi;
				index++;
				pi = pi->next;
				return tp->data;
			}
			return T();
		}
		void reset() {
			index = 0;
			pi = ll->head;
		}
	};

};

template<class T>
bool LinkedList<T>::add(T e) {// 加入到尾部
	add(len, e);
	return true;
}
template<class T>
int LinkedList<T>::indexOf(T e) {
	Item* temp = head;
	for (int i = 0; i < len; i++) {
		if (temp->data == e) {
			return i;
		}
		temp = temp->next;
	}
	return -1;
}
template<class T>
bool LinkedList<T>::contains(T e) {
	Item* temp = head;
	for (int i = 0; i < len; i++) {
		if (temp->data == e) {
			return true;
		}
		temp = temp->next;
	}
	return false;
}


template<class T>
bool LinkedList<T>::isEmpty() {
	return len == 0;
}
template<class T>
inline void LinkedList<T>::removeAll(LinkedList<T>* other)
{
	LinkedList<T>::Iterator itr(other);
	while (itr.hasNext()) {
		auto o = itr.next();
		remove(o, o);
	}
}
template<class T>
inline void LinkedList<T>::removeAll_delete(LinkedList<T>* other)
{
	LinkedList<T>::Iterator itr(other);
	while (itr.hasNext()) {
		auto o = itr.next();
		remove(o, o);
		delete o;
	}
}
template<class T>
bool LinkedList<T>::remove(T e,T e2) {
	int index = indexOf(e);
	if (index != -1) {
		remove(index);
		return true;
	}
	return false;
}
template<class T>
int LinkedList<T>::size() {
	return len;
}
template<class T>
void LinkedList<T>::add(int index, T e) { // 插入
	if (index > 0 || index <= len) {
		if (len == 0) {
			Item* first = new Item(e);
			head = first;
			tail = first;
		}
		else if (index == 0) { 
			Item* temp = new Item(e, head, 0);
			head->prev = temp;
			head = temp;
		}
		else if (index == len) { 
			Item* temp = new Item(e, 0, tail);
			tail->next = temp;
			tail = temp;
		}
		else {
			Item* itemPrev = head;
			for (int i = 1; i < index; i++) {
				itemPrev = itemPrev->next;
			}
			Item* itemNext = itemPrev->next;
			Item* newItem = new Item(e, itemNext, itemPrev);
			itemPrev->next = newItem;
			itemNext->prev = newItem;
		}
		len++;
	}
}
template<class T>
 T  LinkedList<T>::get(int index) { //
	 if (len == 0) return T();
	 if (index >= len  ) index = len - 1;
	if (index >= 0 || index < len) {
		Item* result = head;
		for (int i = 0; i < index; i++) {
			result = result->next;
		}
		return result->data;
	}
	return T();
}

 template<class T>
 void LinkedList<T>::clear() {
	 len = 0;
	 head = 0;
	 tail = head;
 }

template<class T>
 T  LinkedList<T>::remove(int index) {
	 if (len == 0||head==nullptr) return T();
	 if (index >= len) index = len - 1;
	if (index > 0 || index <= len) {
		len--;
		if (index == 0) { // 删除head引用
			Item* temp = head;
			head = temp->next;
			if(head!=nullptr) head->prev = 0;
			 T  result = temp->data; // 建立指向返回值的指针。
			delete temp;
			temp = 0;
			return result;
		}
		else if (index == len) { // 删除tail引用
			Item* temp = tail;
			tail = temp->prev;
			tail->next = 0;
			 T  result = temp->data;
			 delete temp;
			temp = 0;
			return result;
		}
		else {
			Item* item = head;
			for (int i = 0; i < index; i++) { // 通过迭代获取目标指针
				item = item->next;
			}
			Item* itemPrev = item->prev;
			Item* itemNext = item->next;
			itemPrev->next = itemNext;
			itemNext->prev = itemPrev;
			 T  result = item->data;
			delete item;
			item = 0;
			return result;
		}
	
	}
	return T();
}


template<class T>
void LinkedList<T>::swap(int x, int y)
{
	T tx = get(x);
	T ty = get(y);
	remove(x);
	add(x, ty);
	remove(y);
	add(y, tx);
}

template<class T>
void LinkedList<T>::addAll(LinkedList<T>* arr)
{
	if (arr->size() == 0) return;
	for (int i = 0; i < arr->size(); i++) {
		add(arr->get(i));
	}
}

template<class T>
void LinkedList<T>::addAll(int pos,LinkedList<T>* arr)
{
	for (int i = arr->size() - 1; i >= 0; i--) {
		add(pos,arr->get(i));
	}
}

template<class T>
void LinkedList<T>::transportAll(LinkedList<T>* arr)
{
	if (arr->size() == 0) return;

	//if (len == 0) {
	//	head = arr->head;
	//	tail = arr->tail;
	//}
	//else {
	//	tail->next = arr->head;
	//	tail = arr->head;
	//}
	//len += arr->size();
	//arr->clear();
	while (arr->size() > 0) {
		add(arr->remove(0));
	}
}

template<class T>
T  LinkedList<T>::dequeue() {
	len--;
	Item* temp = head;
	head = temp->next;
	if (head != nullptr) head->prev = 0;
	T  result = temp->data;
	delete temp;
	temp = 0;
	return result;
}


template<class T>
void  LinkedList<T>::enqueue(T data) {
	add(data);
}



template<class T>
void  LinkedList<T>::push(T data) {
	add(data);
}

template<class T>
T  LinkedList<T>::pop() {
	return remove(size() - 1);
}



