#pragma once
template<class T>
/*���ܵ�����������*/
class LinkedList {
	struct Item { // ������
		Item(T  d, Item* nxt = 0, Item* pre = 0) : 
			data(d), next(nxt), prev(pre) {
		}
		T  data;
		Item* next;
		Item* prev;
	};
	int len; // ����
	Item* head; // �����ͷָ��
	Item* tail; // �����βָ��
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
	bool add(T  e); // ����Ԫ��
	int indexOf(T  e); // ����Ԫ��λ��
	bool contains(T e); //�ж��Ƿ����
	bool isEmpty(); // �ж������Ƿ�Ϊ��
	void removeAll(LinkedList<T>* other); //ɾ��other�а���������Ԫ��
	void removeAll_delete(LinkedList<T>* other);//ɾ�����ͷ�
	int size(); // ��ȡ��������
	void add(int index, T  e); // ����Ԫ��
	T  get(int index); // ��ȡԪ��
	void clear(); //��ձ�
	T  remove(int index); //ɾ��ĳλ�õ�Ԫ��
	bool remove(T  e, T e2); // ɾ��Ԫ��
	void swap(int x, int y); //������Ԫ��
	void addAll(LinkedList<T>* arr); //����һ������
	void addAll(int pos, LinkedList<T>* arr); //��ĳλ�ü���һ������
	void transportAll(LinkedList<T>* arr); //����һ�����������Ǹ����Ϳ�
	T dequeue(); //������
	void enqueue(T data); //�����
	void push(T data); //��ջ
	T pop(); //��ջ
	// ɾ��Ԫ��
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
bool LinkedList<T>::add(T e) {// ���뵽β��
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
void LinkedList<T>::add(int index, T e) { // ����
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
		if (index == 0) { // ɾ��head����
			Item* temp = head;
			head = temp->next;
			if(head!=nullptr) head->prev = 0;
			 T  result = temp->data; // ����ָ�򷵻�ֵ��ָ�롣
			delete temp;
			temp = 0;
			return result;
		}
		else if (index == len) { // ɾ��tail����
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
			for (int i = 0; i < index; i++) { // ͨ��������ȡĿ��ָ��
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



