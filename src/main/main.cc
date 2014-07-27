#include <iostream>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <thread>

#include "world.h"
#include "zmq_worker.h"
#include "usb_worker.h"

using namespace roboime;

int main(void)
{
    try
    {
        config conf;
        world w;

        zmq_worker worker(conf, 
            [&w] (boost::property_tree::ptree pt) { 
                w.update(pt);
            }
        );
        std::cout << "Initializing tx" << std::endl;

        usb_worker tx(conf);

        std::cout << "RoboIME control module!" << std::endl;

        while (1)
        {
            //boost::shared_lock<boost::shared_mutex> lock(w.access_lock);
            if (conf.get<bool>("control_blue"))
                tx.async_process_action(w.actions_blue);
            if (conf.get<bool>("control_yellow"))
                tx.async_process_action(w.actions_yellow);
            //std::this_thread::sleep_for(std::chrono::milliseconds(0));
        }
        getchar();
    }
    catch (std::exception& e)
    {
        std::cerr << __FUNCTION__ << " [ERROR] " << e.what() << std::endl;
    }
}
