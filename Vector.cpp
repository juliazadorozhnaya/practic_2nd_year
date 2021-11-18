#include <iostream>
#include <cmath>

using namespace std;

class MyVector{
private:
    int *v;
    int len;
public:
  
    MyVector(int len)
    {
    this->len = len;
    this->v = new int[len];
    }//метод

    MyVector()
    {
    this->len = 5;
    this->v = new int[len];
    for(int i=0;i<len;i++)
        this->v[i] = i;
    }//конструктор по умолчанию

    MyVector(const MyVector &v){
    this->len = v.len;
    this->v = new int[len];
    for(int i=0;i<len;i++)
    {
        this->v[i] = v.v[i];
    }
    };//конструктор копирования

    ~MyVector(){
    delete[] v;
    } //деструктор

    void set(int arg, int coord)
    {
        this->v[coord] = arg;
    }

    int get(int coord){
        return this->v[coord];
    }

    double euc_norm()
    {
        int norm = 0;
        for(int i=0;i<len;i++)
        {
            norm += pow(this->v[i], 2);
        }
        return sqrt(norm);
    }

    int max_norm()
    {
        int max = abs(this->v[0]);
        for(int i=1;i<len;i++)
        {
            if(this->v[i] > max)
                max = abs(this->v[i]);
        }
        return max;
    }

    void print()
    {
        for(int i=0;i<len;i++)
        {
            cout << this->v[i] << ",";
        }
    }
};

int main() {
    int size;
    cout << "size = ";
    cin >> size;
    MyVector vec(size);
    cout << "Input " << size << " elements: " << endl;
    for(int i=0;i<size;i++)
    {
        int arg;
        cin >> arg;
        vec.set(arg, i);
    }
    cout << "Print vec: ";
    vec.print();
    cout << endl;
    cout << "euc_norm = " << vec.euc_norm() << endl;
    cout << "max_norm = " << vec.max_norm() << endl;

    return 0;
}