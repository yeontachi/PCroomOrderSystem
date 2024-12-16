#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <chrono>
#include <iomanip>
using namespace std;

class Food{
    private:
        string name="";
        int price=-1;
        int num=0;//주문 번호
        string orderTime;
    public:
        Food(string name, int price, int num){
            setName(name);
            setPrice(price);
            setnum(num);

            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss<<std::put_time(std::localtime(&currentTime),"%H:%M:%S");

            orderTime = ss.str();//객체가 생성된 시간 저장(==음식이 주문된 시간으로 저장)
        }

        void setnum(int num){
            this->num = num;
        }
        int getnum(){
            return num;
        }

        string getOrderTime(){
            return orderTime;
        }

        void setName(string name){
            this->name = name;
        }
        string getName(){
            if(name == ""){
                return "0";
            }
            return name;
        }
        void setPrice(int price){
            this->price = price;
        }
        int getPrice(){
            if(price == -1){
                return 0;
            }
            return price;
        }

        string getPriceWithString(){
            string str = to_string(price) + "won";
            return str;
        }

        const std::string& getName() const { return name; }
        int getPrice() const { return price; }
        int getnum() const { return num; }

};
#endif