#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <iostream>
#include <vector>
#include <mutex>
#include <iomanip>
#include "QuickManager.h"  // QuickManager 헤더 추가
#include "Food.h"          // Food 헤더 추가

class ActionHandler {
private:
    std::vector<QuickManager> A;
    QuickManager Drink;
    QuickManager Meal;
    QuickManager Dessert;
    int orderNum = 0;
    std::mutex mtx; // 동기화용 뮤텍스

public:
    ActionHandler() {
        A.resize(3);
        A[0] = Drink;
        A[1] = Meal;
        A[2] = Dessert;
    }

    void addOrder(int foodType, Food food) {
        std::lock_guard<std::mutex> lock(mtx); // 뮤텍스 잠금
        orderNum++;
        A[foodType].addOrder(food);
        cout<<"주문 추가됨 주문 번호 : "<<orderNum<<endl;
    }

    void menu() {
        //printOrderLists();
        std::cout << "1. 주문 완료 처리\n";
        std::cout << "2. 주문 취소 처리\n";
        std::cout << "3. 주문 목록 출력\n";
        std::cout << "4. 종료\n";
    }

    void action(int foodType, int index) {
        std::lock_guard<std::mutex> lock(mtx); // 뮤텍스 잠금
        A[foodType].removeOrder(index);
        updateOrdersFile();
    }

    void updateOrdersFile(){
        std::ofstream outFile("temp_orders.txt");//임시 파일 생성
        if(!outFile.is_open()){
            std::cerr<<"임시 파일을 열 수 없습니다."<<endl;
            return;
        }

        for(int i=0; i<3; ++i){
            const auto& orderList = A[i].getOrderList();
            for(const auto& food : orderList){
                outFile << i << " " << food.getName()<<" "<<food.getnum()<<endl;
            }
        }
        outFile.close();

        std::remove("orders.txt");
        std::rename("temp_orders.txt", "orders.txt");
    }

    void printOrderLists() {
        std::lock_guard<std::mutex> lock(mtx); // 뮤텍스 잠금
        cout<<"주문 목록"<<endl;
        // 주문 목록을 출력하는 코드
            const int width1 = 21;
            const int width2 = 21; 
            const int width3 = 20;
            if(orderNum == 0) {
                cout<<"주문이 없습니다."<<endl;
                return;
            }

            int i = orderNum;

            while(i-- > 0){
                cout<<"==================== ";cout<<"==================== ";cout<<"==================== "<<endl;

                if(i<A[0].getOrderList().size()) cout<<left<<setw(width1)<<A[0].getOrder(i).getnum();
                else cout<<left<<setw(width1)<<"";
                if(i<A[1].getOrderList().size()) cout<<left<<setw(width1)<<A[1].getOrder(i).getnum();
                else cout<<left<<setw(width2)<<"";
                if(i<A[2].getOrderList().size()) cout<<left<<setw(width1)<<A[2].getOrder(i).getnum();
                else cout<<left<<setw(width3)<<"";
                cout<<endl;

                if(i<A[0].getOrderList().size()) cout<<left<<setw(width1)<<A[0].getOrder(i).getName();
                else cout<<left<<setw(width1)<<"";
                if(i<A[1].getOrderList().size()) cout<<left<<setw(width1)<<A[1].getOrder(i).getName();
                else cout<<left<<setw(width2)<<"";
                if(i<A[2].getOrderList().size()) cout<<left<<setw(width1)<<A[2].getOrder(i).getName();
                else cout<<left<<setw(width3)<<"";
                cout<<endl;

                if(i<A[0].getOrderList().size()) cout<<left<<setw(width1)<<A[0].getOrder(i).getPriceWithString();
                else cout<<left<<setw(width1)<<"";
                if(i<A[1].getOrderList().size()) cout<<left<<setw(width1)<<A[1].getOrder(i).getPriceWithString();
                else cout<<left<<setw(width2)<<"";
                if(i<A[2].getOrderList().size()) cout<<left<<setw(width1)<<A[2].getOrder(i).getPriceWithString();
                else cout<<left<<setw(width3)<<"";
                cout<<endl;

                if(i<A[0].getOrderList().size()) cout<<left<<setw(width1)<<A[0].getOrder(i).getOrderTime();
                else cout<<left<<setw(width1)<<"";
                if(i<A[1].getOrderList().size()) cout<<left<<setw(width1)<<A[1].getOrder(i).getOrderTime();
                else cout<<left<<setw(width2)<<"";
                if(i<A[2].getOrderList().size()) cout<<left<<setw(width1)<<A[2].getOrder(i).getOrderTime();
                else cout<<left<<setw(width3)<<"";
                cout<<endl;
            }
            cout<<endl<<endl;
    }
};

#endif // ACTIONHANDLER_H
