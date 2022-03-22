#include "../Framework/XCoreApplication.h"
#include "../Framework/XEvent/XTimerEvent.h"
#include <iostream>
int main(int argc, char *argv[])
{
    XCoreApplication a(argc, argv);
    XTimerEvent timer;
    timer.setTimer(5000,[=](){
        std::cout << "timer1" << std::endl;
    });
    timer.start();


    XTimerEvent timer2;

    timer2.setTimer(1000,[=](){
        std::cout << "timer2" << std::endl;
    });
    timer2.start();



    XTimerEvent timer3;

    timer3.setTimer(2000,[=](){
        std::cout << "timer3" << std::endl;
    });
    timer3.start();


    a.exec();
}
