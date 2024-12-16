#ifndef QUICKMANAGER_H
#define QUICKMANAGER_H

#include <iostream>
#include <list>
#include "Food.h"

class QuickManager{
    private:
        list<Food> Odlist; //실제 데이터들을 처리하는 리스트
    public:
        QuickManager(){
            Odlist = list<Food>();
        }

        void addOrder(Food food){
            Odlist.push_back(food);
        }

        const list<Food>& getOrderList(){
            return Odlist;
        }

        Food& getOrder(int index){
            auto it = Odlist.begin();
            advance(it, index);//index만큼 반복자를 이동
            return *it;//반복자가 가리키는 요소 반환
        }

        void removeOrder(int index){//주문 완료 처리 및 주문 취소 처리
            auto it = Odlist.begin();
            advance(it, index);
            if(it != Odlist.end()){
                Odlist.erase(it);
            }
        }       
};

#endif