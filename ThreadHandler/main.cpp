#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

using namespace std;
//----------------------------------------------------------------------------------------
#define C_INFINITY true

std::condition_variable cv;
std::mutex mutex_name;
bool bSelect = true;
//----------------------------------------------------------------------------------------
uint32_t run_asyns(std::string str)
{
  for(uint32_t uiCounter(0);uiCounter < 4;uiCounter++)
  {
    std::unique_lock<std::mutex> ul(mutex_name);
    std::cout << str;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 25;
}
//----------------------------------------------------------------------------------------
void run_print_th_a(std::string&str)
{
  while(C_INFINITY)
  {
    std::unique_lock<std::mutex> ul(mutex_name);
    cv.wait(ul,[](){return bSelect;});


    std::cout << str << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    bSelect = false;

    cv.notify_one();
  }

}
//----------------------------------------------------------------------------------------
void run_print_th_b(std::string&str)
{
  while(C_INFINITY)
  {
    std::unique_lock<std::mutex> ul(mutex_name);
    cv.wait(ul,[]{return !bSelect;});

    std::cout << str << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    bSelect = true;

    cv.notify_one();

  }
}
//----------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#ifdef D_THREAD_PARALEL
  std::string strThreadNameA = "1";
  std::string strThreadNameB = "2";

  std::thread th_A(run_print_th_a,std::ref(strThreadNameA));
  std::thread th_B(run_print_th_b,std::ref(strThreadNameB));

  th_A.join();
  th_B.join();
#endif


#ifdef D_ASYNS
  auto future = std::async(std::launch::deferred,run_asyns,".");

  future.wait();

  std::cout << "\nFuture:" << future.get()<< endl;

#endif





  return 0;
}
//----------------------------------------------------------------------------------------
