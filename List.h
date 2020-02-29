
template <class T>
class ListNode{
  public:
  ListNode<T>* next = NULL;
  T value;

  ListNode(T v):value(v){
  }

};

template <class T>
class Iterator{
  private:
  ListNode<T> *current = NULL;
  ListNode<T> *start;

  ListNode<T> *_current(){
    return current!=NULL?current:start;
  }

  public:
   
  Iterator(ListNode<T>* node){
    start = node;
  }

  bool hasNext(){
    return (current!=NULL && current->next!=NULL) ||  (current==NULL && start!=NULL);
  }

  T next(){
    if(current==NULL){
      current = start;
    }
    else {
      current = current->next;
    }

    return current->value;
  }
//  template <class T>
//  friend class List;

};

template <class T>
class List{
  private:
  ListNode<T> * begin = NULL;
  ListNode<T> * end = NULL;
  int size = 0;

  public:
  List(){

  }
    

  ~List(){
    ListNode<T> *p = begin;
    while(p!=NULL){
      ListNode<T>* old = p;
      p = p->next;
      delete old;
    }
  }

  void add(T a){
    ListNode<T> * node = new ListNode<T>(a);


    if(end!=NULL){
      end->next = node;
      end = node;
    }
    else {
      begin = end = node;
    }
    size++;
  }

  bool remove(T a){
    if(begin == NULL){
      return false;
    }
    if(begin->value==a){
      ListNode<T>*p = begin;
      begin = begin->next;
      delete p;
      size--;
      return true;
    }

    ListNode<T> *prev = begin;
    ListNode<T> *p = prev->next;
    while(p!=NULL){
      if(p->value==a){
        prev->next=p->next;
        delete p;
        size--;
        return true;
      }
    }
    return false;
  }

  typedef void (*foreach_func)(T& value);
  void foreach(foreach_func f){
    ListNode<T> *p = begin;
    while(p!=NULL){
      f(p->value);
      p = p->next;
    }
  }
  T operator[](int index){
    int i=0;
    ListNode<T> * p = begin;
    while(p!=NULL && i<index){
      p=p->next;
      i++;
    }
    if(p!=NULL){
      return p->value;
    }
    else {
//      throw "List index error";
        return NULL;
    }
  }

  Iterator<T>* iterator(){
    return new Iterator<T>(begin);
  }

  int length(){
    return size;
  }

};
