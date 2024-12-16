#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <chrono>
#include "ActionHandler.h"
using namespace std;

int num=0;

void customerOrder(ActionHandler& A){
    int num=0;
    while(true){
        int foodType;
        string name;
        int price;

        cout<<"주문을 입력하세요(음식 종류 0:음료 1:식사 2:디저트, 이름, 가격): ";
        cin>>foodType>>name>>price;


        num+=1;

        Food newFood(name, price, num);
        A.addOrder(foodType, newFood);

        std::ofstream outFile("orders.txt", std::ios::app);
        if(outFile.is_open()){
            outFile<<foodType<<" "<<name<<" "<<price<<" "<<num<<endl;
            outFile.close();
        }else{
            cerr<<"파일을 열 수 없습니다."<<endl;
        }

        this_thread::sleep_for(chrono::seconds(1));

        cout<<"주문이 추가되었습니다: "<<name<<" ("<<price<<"원) "<<endl;
    }
}


int main(void){
    ActionHandler A;

    thread customerThread(customerOrder, ref(A));

    customerThread.join();

    return 0;
}