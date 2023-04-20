#include <thread>
#include <memory>
#include <ctime>
#include <chrono>
#include <iomanip>
#include "CSVBarDataFeed.h"

using namespace otterbot;

int main()
{
  std::shared_ptr<CSVBarDataFeed> pdf(new CSVBarDataFeed("AAPL", "aapl.csv"));
  std::thread t1(&CSVBarDataFeed::subscribe, pdf);
  std::thread t2([&] {
    std::cout << "start: " << std::endl;
    while (true) {
      BarData bar = pdf->getData();
      std::time_t t = std::chrono::system_clock::to_time_t(bar.timestamp);
      std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") 
                << " " << bar.last << std::endl;
    }
  });
  t1.join();
  t2.join();
  return 0;
}