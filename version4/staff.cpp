#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <chrono>
#include "ActionHandler.h"
using namespace std;

void loadOrdersFromFile(ActionHandler& A){
    ifstream inFile("orders.txt");
    if(!inFile.is_open()){
        cerr<<"파일을 열 수 없습니다."<<endl;
        return;
    }

    int foodType, price, num;
    string name;

    while(inFile>>foodType>>name>>price>>num){
        Food newFood(name, price, num);
        A.addOrder(foodType, newFood);
    }

    inFile.close();
}

void staffProcessOrders(ActionHandler& A){

    loadOrdersFromFile(A);

    while(true){
        //loadOrdersFromFile(A);
        A.printOrderLists();
        A.menu();
        int a;
        cout<<"입력: ";
        cin>>a;

        if(a==1){
            cout<<"주문을 완료(주문 번호와 음식 종류를 입력해주세요)"<<endl;
        }
        else if(a==2){
            cout<<"주문을 취소(주문 번호와 음식 종류를 입력해주세요)"<<endl;
        }
        else if(a==3){
            loadOrdersFromFile(A);
            //A.printOrderLists();
            continue;
        }
        else if(a==4){
            cout<<"프로그램 종료"<<endl;
            exit(0);
        }

        int foodType, orderIndex;
        cout<<"음식 종류 0:음료 1:식사 2:디저트, 주문 번호: ";
        cin>>foodType>>orderIndex;

        A.action(foodType, orderIndex);

        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main(void){
    ActionHandler A;

    thread staffThread(staffProcessOrders, ref(A));

    staffThread.join();

    return 0;
}