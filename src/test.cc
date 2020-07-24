#include <loop.hpp>
#include <iostream>
#include <thread>
#include <string.h>
struct my_event: public event_base{
    enum event_type{
        EVENT_START = 1L,
        EVENT_EXIT,
        EVENT_HELLO
    };
    char data[25];
    virtual ~my_event()override{std::cout << "auto delete event!" << std::endl;}
};
int main(){
    //must “new“ and shouldn't delete
    event_loop *loop = new event_loop;
    loop->set_handler(my_event::event_type::EVENT_HELLO,[](event_base* event){
        std::cout << "Hello ,Received hello event,data="<<((my_event*)event)->data<<std::endl;
    });
    loop->run_loop();
    auto event = new my_event;
    event->id = event->EVENT_HELLO;
    strcpy(event->data,"Hello World");
    loop->push(event);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    loop->exit();
    return 0;
}