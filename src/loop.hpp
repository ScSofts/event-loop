#pragma once
#include <functional>
#include <unordered_map>
#include <atomic>
#include <queue>

struct event_base{
public:
    using event_id = unsigned long;
    enum event_type
    {
        EVENT_START = 1L,
        EVENT_EXIT
    };
    virtual ~event_base() = default;
    event_id id = event_type::EVENT_START;
};

class event_loop
{
public:
    event_loop();

    void run_loop();
    void exit();


    void push(event_base * event);

    using handler = std::function<void(event_base *)>;
    void set_handler(event_base::event_id event_id, handler handler);
    void remove_handler(event_base::event_id event_id);

protected:
    template <class keyType, class valueType>
    using map = std::unordered_map<keyType, valueType>;
    void loop();
    void remove();

    using handler_map_type =  map<event_base::event_id,handler>;
    handler_map_type *handler_map = nullptr;
    
    void *loop_thread;

    virtual ~event_loop();
    
    using event_queue_unsafe = std::queue<event_base*>;

    using event_queue_type = std::atomic<event_queue_unsafe*>;
    event_queue_type* event_queue = nullptr;
};