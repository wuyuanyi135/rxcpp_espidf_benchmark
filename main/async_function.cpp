//
// Created by wuyua on 3/7/2022.
//

#include "async_function.h"
rx::observable<WaitThenReportStatesPtr> wait_then_report_delay(uint64_t udelay, const rx::identity_one_worker& c) {
  auto state = std::make_shared<WaitThenReportStates>();
  //  // pass bool catch shared
  return rx::sources::just<bool>(true, c)
      | ops::map([state](const bool& _) {
           state->first_time = std::chrono::system_clock::now();
           return _;
         })
      | ops::delay(chrono::microseconds(udelay), c)
      | ops::map([state](const bool& _) {
           state->second_time = std::chrono::system_clock::now();
           return state;
         });

  //  // Ptr reference not copying
  //  return rx::sources::just<WaitThenReportStatesPtr>(std::move(state), c)
  //      | ops::map([](const WaitThenReportStatesPtr& state) {
  //           state->first_time = std::chrono::system_clock::now();
  //           return state;
  //         })
  //      | ops::delay(chrono::microseconds(udelay), c)
  //      | ops::map([](const WaitThenReportStatesPtr& state) {
  //           state->second_time = std::chrono::system_clock::now();
  //           return state;
  //         });

  //  // pass shared and copy shared
//  return rx::sources::just<WaitThenReportStatesPtr>(std::move(state), c)
//      | ops::map([](WaitThenReportStatesPtr state) {
//           state->first_time = std::chrono::system_clock::now();
//           return state;
//         })
//      | ops::delay(chrono::microseconds(udelay), c)
//      | ops::map([](WaitThenReportStatesPtr state) {
//           state->second_time = std::chrono::system_clock::now();
//           return state;
//         });
}
rx::observable<WaitThenReportStates> wait_then_report_delay_copy(uint64_t udelay, const rx::identity_one_worker& c) {
  auto state = WaitThenReportStates();
  return rx::sources::just<WaitThenReportStates>(std::move(state), c)
      | ops::map([](WaitThenReportStates state) {
           state.first_time = std::chrono::system_clock::now();
           return state;
         })
      | ops::delay(chrono::microseconds(udelay), c)
      | ops::map([](WaitThenReportStates state) {
           state.second_time = std::chrono::system_clock::now();
           return state;
         });
}
WaitThenReportStates::~WaitThenReportStates() {
  //  std::cout << "Destructed WaitThenReportStates" << std::endl;
}
WaitThenReportStates::WaitThenReportStates() {
  //  std::cout << "Constructed WaitThenReportStates" << std::endl;
}
