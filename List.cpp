#include <iostream>
#include <list>
using namespace std;

//list

template<typename T>
class List
{
public: 
    List();
    ~List();
    void push_back(T data);
    int GetSize() {return size;}
    T& operator [] (const int index);

private:
    template<typename U> //если захотим что-то менять, то пришлось менять много кода, используя шаблон - не надо
    class Node
    {
    public:
        Node *pNext;
        U data;

        Node (U data=U(), Node *pNext=nullptr)
        {
            this->data = data;
            this->pNext = pNext;
        }
    };

    int size; //количество элементов в односвязном списке
    Node<T> *head;
};

template<typename T>
List<T>::List() //конструктор по умолчанию
{
    size = 0;
    head = nullptr;
}

template<typename T>
List<T>::~List()
{
    
}

template<typename T>
 void List<T>::push_back(T data)
{
    //проверяем на пустоту
    if (head == nullptr)
    {
        head = new Node<T>(data);
    }
    else 
    {
        Node<T> *current = this->head; 

        while (current->pNext != nullptr)
        {
            current = current->pNext;
        }

        current->pNext = new Node <T>(data);

    }
    size++;

}

template<typename T> T & List<T>::operator[](const int index)
{
    int counter = 0;
    Node<T> *current = this->head; 

    while (current != nullptr)
    {
       
            if (counter == index)
            {
                return current->data;
            }
            current = current -> pNext;
            counter++;

    }
}

int main()
{
    List<int> lst;
    lst.push_back(5);
    lst.push_back(10);
    lst.push_back(20);

    cout << lst.GetSize() << endl;
    cout << lst[2] <<endl;

    return 0;
}