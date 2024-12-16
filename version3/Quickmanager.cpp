#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>

using namespace std;

class Food {
    private:
        string name = "";
        int price = -1;
        int num = 0;
        string orderTime;

    public:
        Food(string name, int price, int num) {
            setName(name);
            setPrice(price);
            setnum(num);

            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss << std::put_time(std::localtime(&currentTime), "%H:%M:%S");

            orderTime = ss.str();
        }

        void setnum(int num) { this->num = num; }
        int getnum() { return num; }
        string getOrderTime() { return orderTime; }
        void setName(string name) { this->name = name; }
        string getName() { return name == "" ? "0" : name; }
        void setPrice(int price) { this->price = price; }
        int getPrice() { return price == -1 ? 0 : price; }
        string getPriceWithString() { return to_string(price) + "won"; }
};

class QuickManager {
    private:
        list<Food> Odlist;
        mutex mtx;

    public:
        void addOrder(Food food) {
            lock_guard<mutex> lock(mtx);
            Odlist.push_back(food);
        }

        const list<Food>& getOrderList() {
            lock_guard<mutex> lock(mtx);
            return Odlist;
        }

        Food& getOrder(int index) {
            auto it = Odlist.begin();
            advance(it, index);
            return *it;
        }

        void removeOrder(int index) {
            lock_guard<mutex> lock(mtx);
            auto it = Odlist.begin();
            advance(it, index);
            Odlist.erase(it);
        }
};

class ActionHandler {
    public:
        vector<QuickManager> A;
        QuickManager Drink, Meal, Dessert;
        int orderNum = 0;
        mutex actionMutex;

        ActionHandler() {
            A.resize(3);
            A[0] = Drink;
            A[1] = Meal;
            A[2] = Dessert;
        }

        void addOrder(int foodtype, Food food) {
            lock_guard<mutex> lock(actionMutex);
            A[foodtype].addOrder(food);
            orderNum++;
        }

        void menu() {
            printOrderLists();
            cout << "1. 주문 완료 처리" << endl;
            cout << "2. 주문 취소 처리" << endl;
            cout << "3. 자동 주문 추가 시작" << endl;
            cout << "4. 자동 주문 추가 멈춤" << endl;
            cout << "5. 종료" << endl;
        }

        void action(int c, condition_variable &cv, bool &runAutoOrder) {
            if (c == 1) {
                int x, y;
                cout << "주문 완료 처리할 주문 번호 입력: ";
                cin >> x >> y;
                A[x].removeOrder(y);
            } else if (c == 2) {
                int x, y;
                cout << "주문 취소할 주문 번호 입력: ";
                cin >> x >> y;
                A[x].removeOrder(y);
            } else if (c == 3) {
                {
                    unique_lock<mutex> lock(actionMutex);
                    runAutoOrder = true;
                }
                cv.notify_all();
            } else if (c == 4) {
                unique_lock<mutex> lock(actionMutex);
                runAutoOrder = false;
            } else if (c == 5) {
                cout << "프로그램 종료" << endl;
                exit(0);
            }
        }

        void printOrderLists() {
            const int width1 = 21;
            const int width2 = 21;
            const int width3 = 20;
            int a = (orderNum % 3 == 0) ? orderNum / 3 : orderNum / 3 + 1;

            lock_guard<mutex> lock(actionMutex);
            for (int i = 0; i < a; i++) {
                cout << "==================== " << "==================== " << "==================== " << endl;

                if (i < A[0].getOrderList().size()) cout << left << setw(width1) << A[0].getOrder(i).getnum();
                else cout << left << setw(width1) << "";
                if (i < A[1].getOrderList().size()) cout << left << setw(width1) << A[1].getOrder(i).getnum();
                else cout << left << setw(width2) << "";
                if (i < A[2].getOrderList().size()) cout << left << setw(width1) << A[2].getOrder(i).getnum();
                else cout << left << setw(width3) << "";
                cout << endl;

                if (i < A[0].getOrderList().size()) cout << left << setw(width1) << A[0].getOrder(i).getName();
                else cout << left << setw(width1) << "";
                if (i < A[1].getOrderList().size()) cout << left << setw(width1) << A[1].getOrder(i).getName();
                else cout << left << setw(width2) << "";
                if (i < A[2].getOrderList().size()) cout << left << setw(width1) << A[2].getOrder(i).getName();
                else cout << left << setw(width3) << "";
                cout << endl;

                if (i < A[0].getOrderList().size()) cout << left << setw(width1) << A[0].getOrder(i).getPriceWithString();
                else cout << left << setw(width1) << "";
                if (i < A[1].getOrderList().size()) cout << left << setw(width1) << A[1].getOrder(i).getPriceWithString();
                else cout << left << setw(width2) << "";
                if (i < A[2].getOrderList().size()) cout << left << setw(width1) << A[2].getOrder(i).getPriceWithString();
                else cout << left << setw(width3) << "";
                cout << endl;

                if (i < A[0].getOrderList().size()) cout << left << setw(width1) << A[0].getOrder(i).getOrderTime();
                else cout << left << setw(width1) << "";
                if (i < A[1].getOrderList().size()) cout << left << setw(width1) << A[1].getOrder(i).getOrderTime();
                else cout << left << setw(width2) << "";
                if (i < A[2].getOrderList().size()) cout << left << setw(width1) << A[2].getOrder(i).getOrderTime();
                else cout << left << setw(width3) << "";
                cout << endl;
            }
            cout << endl << endl;
        }
};

void addRandomOrders(ActionHandler& handler, condition_variable &cv, bool &runAutoOrder) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> priceDist(1000, 5000);
    uniform_int_distribution<> numDist(1, 10);

    vector<string> foodNames = {"Americano", "Latte", "Mocha", "ShinRamen", "KimchiRamen", "Jjampong", "Icecream", "Cake", "Cookie"};

    while (true) {
        unique_lock<mutex> lock(handler.actionMutex);
        cv.wait(lock, [&] { return runAutoOrder; });

        int type = rand() % 3;
        int price = priceDist(gen);
        int num = numDist(gen);
        string name = foodNames[rand() % foodNames.size()];
        handler.addOrder(type, Food(name, price, num));
        lock.unlock();

        this_thread::sleep_for(chrono::seconds(3));
    }
}

int main() {
    ActionHandler handler;
    condition_variable cv;
    bool runAutoOrder = true;

    thread orderThread(addRandomOrders, ref(handler), ref(cv), ref(runAutoOrder));

    while (true) {
        int choice;
        handler.menu();
        cin >> choice;
        handler.action(choice, cv, runAutoOrder);
    }

    orderThread.join();
    return 0;
}
