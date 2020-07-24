#include "loop.hpp"
#include <thread>



event_loop::~event_loop(){
     auto thread_loop = (std::thread*)this->loop_thread;
    if(this->handler_map){
        delete this->handler_map;
        this->handler_map = nullptr;
    }
    if(this->event_queue){
        event_queue_unsafe *queue = (*this->event_queue);
        delete queue;
        delete this->event_queue;
        this->event_queue = nullptr;
    }
    
    delete thread_loop;
}

event_loop::event_loop(){
    this->handler_map = new handler_map_type;
    this->event_queue = new event_queue_type;
    (*this->event_queue) = new event_queue_unsafe;
    auto event_init = new event_base;
    event_init->id = event_init->EVENT_START;
    this->push(event_init);

    this->set_handler(event_base::EVENT_EXIT,[&](event_base *event){});
}

void event_loop::run_loop(){

    this->loop_thread = (void*) new std::thread([this](){
        this->loop();
    });
}

void event_loop::push(event_base * event){
    event_queue_unsafe*queue = (*this->event_queue);
    queue->push(event);
}

void event_loop::remove(){
    event_queue_unsafe *queue = (*this->event_queue);
    delete queue->front();
    queue->pop();
}

void event_loop::loop(){
    event_queue_unsafe* queue = (*this->event_queue);
    event_base * event_now;
    bool should_run_again = true;
    while(should_run_again){
        if(!queue->empty()){
            event_now = queue->front();
            if(event_now->id == event_now->EVENT_EXIT) should_run_again = false;
            //if handler exists
            if(this->handler_map->find(event_now->id) != this->handler_map->end()){
                (*this->handler_map)[event_now->id](event_now);
            }
            this->remove();
        }
    }
}

void event_loop::set_handler(event_base::event_id event_id, event_loop::handler handler){
    (*this->handler_map)[event_id] = handler;
}

void event_loop::remove_handler(event_base::event_id event_id){
    if(this->handler_map->find(event_id)!=this->handler_map->end()){
        this->handler_map->erase(event_id);
    }
}

void event_loop::exit(){
    auto event_exit = new event_base;
    event_exit->id = event_exit->EVENT_EXIT;
    this->push(event_exit);

    auto thread_loop = (std::thread*)this->loop_thread;
    thread_loop->join();
    delete this;
}