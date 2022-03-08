#include <driver/uart.h>
#include <esp_netif.h>
#include <esp_vfs_dev.h>

#include "async_function.h"

// WDT
#include "soc/timer_group_reg.h"
#include "soc/timer_group_struct.h"

using duration_type = rxcpp::schedulers::scheduler::clock_type::time_point::duration;
extern "C" [[noreturn]] void app_main(void) {
  esp_netif_init();

  ESP_ERROR_CHECK(uart_driver_install(0, 256, 256, 10, NULL, 0));
  esp_vfs_dev_uart_use_driver(0);

  rx::schedulers::run_loop rl;
  rx::identity_one_worker worker(rx::schedulers::make_run_loop(rl));

  rx::sources::interval(chrono::milliseconds(500), worker)
      .subscribe([](int v) {
        TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
        TIMERG0.wdt_feed = 1;
        TIMERG0.wdt_wprotect = 0;
      });

  rx::sources::interval(chrono::milliseconds(200), worker)
      .subscribe([](int v) {
        std::cout << "Free heap: " << xPortGetFreeHeapSize() << "\t"
                  << "Stack watermark: " << uxTaskGetStackHighWaterMark(NULL)
                  << std::endl;
      });

  rx::sources::interval(chrono::milliseconds(10), worker)
      .map([](int v) {
        return v * 50;
      })
      .take_while([](int i) {
        return i < 12000;
      })
      .subscribe([&](int i) {
        // No copy
        wait_then_report_delay(i, worker)
            .subscribe(
                [i](const WaitThenReportStatesPtr& states) {
                  std::cout << i << ","
                            << chrono::duration_cast<chrono::microseconds>(states->second_time - states->first_time).count() - i
                            << std::endl;
                });

        // Copy
        //        wait_then_report_delay_copy(i, worker)
        //            .subscribe([i](const WaitThenReportStates& states) {
        //              std::cout << i << ","
        //                        << chrono::duration_cast<chrono::microseconds>(states.second_time - states.first_time).count() - i
        //                        << std::endl;
        //            });
      });

  while (true) {
    rl.dispatch();
  }
}
