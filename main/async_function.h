//
// Created by wuyua on 3/7/2022.
//

#ifndef ASYNC_FUNCTION_H_
#define ASYNC_FUNCTION_H_

#include <rx.hpp>
namespace rx = rxcpp;
namespace ops = rxcpp::rxo;
namespace chrono = std::chrono;
using duration = chrono::system_clock::duration;

struct WaitThenReportStates {
 public:
  ~WaitThenReportStates();
  WaitThenReportStates();

  std::thread::id first_entrant_thread;
  std::thread::id second_entrant_thread;

  chrono::system_clock::time_point first_time;
  chrono::system_clock::time_point second_time;


};

using WaitThenReportStatesPtr = std::shared_ptr<WaitThenReportStates>;


rx::observable<WaitThenReportStatesPtr> wait_then_report_delay(uint64_t udelay, const rx::identity_one_worker& c);
rx::observable<WaitThenReportStates> wait_then_report_delay_copy(uint64_t udelay, const rx::identity_one_worker& c);

#endif  // ASYNC_FUNCTION_H_
