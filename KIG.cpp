#include "ThreadSafeQueue.h"
#include "ThreadPool.h"
#include "ProcessMonitor.h"
#include "FileSystemMonitor.h"
#include "RegMonitor.h"
#include <memory>
//#include "IMonitor.h"
//#include <iostream>
//#include <thread>
//#include <chrono>
//#include <ctime>

ThreadSafeQueue queue;

int bal;

//void producerA()
//{
//    int i = 0;
//    while (i < 5) {
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        queue.push(Event(EventType::ProcessCreated, "Random Process", i));
//        //std::cout << "ProducerA Index: " << i << " \n";
//        i++;
//    }
//}
//
//void producerB()
//{
//    int i = 0;
//    while (i < 5) {
//        std::this_thread::sleep_for(std::chrono::seconds(3));
//        queue.push(Event(EventType::ProcessCreated, "Random File", i));
//        //std::cout << "\nProducerB Index: " << i << " \n ";
//        i++;
//    }
//}
//
//void consumer(int id, Logger &_logger, ThreadSafeQueue &_queue) {
//    while (true) {
//        std::cout << "Worker " << id << " processed:\n";
//       /* Event last = queue.pop(queue.getLast());
//        _logger.write(last);
//        std::cout << last.getType() << " | Id: " << last.getId() << '\n';*/
//    }
//}

void sendFund(std::string student, int amt) {

    if (bal > amt) {
        bal -= amt;
    }
}

int main()
{
    //std::thread req1(sendFund, "Emmanuel", 50);
    //std::thread req2(sendFund, "Emmanuel", 50);
    //std::thread req3(sendFund, "Emmanuel", 50);
    //std::thread req4(sendFund, "Emmanuel", 50);


    //req1.join();
    //req2.join();
    //req3.join();
    //req4.join();
    //std::cout << "Balance: " << bal;
    //std::thread user1(sendFund, "Samuel", 50);
    //ProcessMonitor processMonitor(queue);
    //FileSystemMonitor fileSystemMonitor(queue);
    std::vector<std::unique_ptr<IMonitor>> monitors;

    //monitors.emplace_back(std::make_unique<ProcessMonitor> (queue));
    //monitors.emplace_back(std::make_unique<FileSystemMonitor> (queue));
    //Software\\Microsoft\\Windows\\CurrentVersion\\Run
    monitors.emplace_back(std::make_unique<RegMonitor> (queue, HKEY_CURRENT_USER, L"Software\\Opera Software"));
    ////monitors.emplace_back(std::make_unique<RegMonitor> (queue, HKEY_LOCAL_MACHINE, L"Software"));
    ////monitors.emplace_back(std::make_unique<RegMonitor> (queue, HKEY_CURRENT_CONFIG, L"Software"));
    ////monitors.emplace_back(std::make_unique<RegMonitor> (queue, HKEY_CLASSES_ROOT, L"Software"));
    Logger logger;
    ///*std::thread t(producerA);
    //std::thread t3(producerB);*/

    for(const auto &monitor : monitors) monitor->start();
    //Fmonitor.start();

    ThreadPool pool(queue, logger); 

    std::cout << "Hello Program Just Started.\n";

    pool.start();

    /*t.join();
    t3.join();*/
    //t2.join();
    std::cout <<"Total Count: "<< queue.getQueueSize();

    std::this_thread::sleep_for(std::chrono::seconds(240));

    for(const auto& monitor : monitors) monitor->stop();
    //pool.stop();
    /*monitor.stop();
    Fmonitor.stop();*/
    return 0;
}





//#include <mutex>
//#include <condition_variable>
//#include <queue>
//std::queue<Event> events;
//std::mutex m;
//std::condition_variable cv;

     //   {
     //       //std::lock_guard<std::mutex> lock(queue.m);
     //       queue.lockMutex();

     //   }
        //std::cout << "Process Created!\n";
     //   //queue.cv.notify_one();
     //   queue.notifyOne();

    //    {
    //        //std::lock_guard<std::mutex> lock(queue.m);
    //        queue.lockMutex();
    //        std::cout << "File Modified!\n";
    //    }

    //queue.notifyOne()
        //;   //queue.cv.notify_one();


        //std::unique_lock<std::mutex> lock(queue.m);
       /* auto lock = queue.uniqueLockMutex();

        queue.cv.wait(lock, [] {
            return queue.getQueueSize() != 0;
        });
        std::string last = queue.getFront().getType();
        int id = queue.getFront().getId();


//std::cout << "ProducerB Index: " << i << '\n';

        lock.unlock();*/
        /*std::thread t1(producerA);
        std::thread t4(producerB);*/

        /*t1.join();
        t4.join();*/


        /*if (events.empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            if(events.empty()) return;
        }*/

        //bool jobReady = false;

        //struct Event
        //{
        //    std::string type;
        //    int id;
        //};


        //void producerA()
        //{
        //    //std::unique_lock<std::mutex> lock(m);
        //
        //    /*std::cout << "Worker: Waiting for a job...\n";
        //
        //    cv.wait(lock, []()
        //        {
        //            return jobReady;
        //        });
        //    */
        //  
        // 
        // 
        //    //{
        //    //cv.wait(lock, []() 
        //        //{
        //        //    std::lock_guard<std::mutex> lock(m);
        //        //    std::this_thread::sleep_for(std::chrono::seconds(1));
        //        //    std::cout << "Process Created!\n";
        //        //    events.push("Process Created!");
        //        //    jobReady = true;
        //        //    //}
        //        //}//);
        //    //cv.notify_one();
        //
        //}



           // {
           //     std::lock_guard<std::mutex> lock(m);

           //std::this_thread::sleep_for(std::chrono::seconds(3));
           //     std::cout << "Main: Creating job...\n";

           //     //jobReady = true;
           // }

           // cv.notify_one();