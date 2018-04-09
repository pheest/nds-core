#include <functional>

#include <nds3/nds.h>
#include "terminal.h"


terminal::terminal(const std::string& name, nds::Node& parent) {

  //  m_Timestamping = parent.addChild(nds::Timestamping<std::vector<std::int32_t>>
  //				     ("Timestamping", 4,
  //				      std::bind(&terminal::switchon_timestamping, this),
  //				      std::bind(&terminal::switchoff_timestamping, this),
  //				      std::bind(&terminal::start_timestamping, this),
  //				      std::bind(&terminal::stop_timestamping, this),
  //				      std::bind(&terminal::recover_timestamping, this),
  //				      std::bind(&terminal::allow_timestamping_change, this,
  //						std::placeholders::_1,
  //						std::placeholders::_2,
  //						std::placeholders::_3),
  //				      std::bind(&terminal::pv_enable_writer, this,
  //						std::placeholders::_1, std::placeholders::_2),
  //				      std::bind(&terminal::pv_edge_writer, this,
  //						std::placeholders::_1, std::placeholders::_2),
  //				      std::bind(&terminal::pv_clearoverflow_writer, this,
  //						std::placeholders::_1, std::placeholders::_2)));


  // INICIALIZAR TIMING

}



terminal::~terminal() {

}

/* Timing methods. */

void terminal::switchon_timing() {

}

void terminal::switchoff_timing() {

}

void terminal::start_timing() {

}

void terminal::stop_timing() {

}

void terminal::recover_timing() {

}

bool terminal::allow_timing_change(const nds::state_t,
			 const nds::state_t,
			 const nds::state_t) {

  return true;

}

void terminal:: pv_timing_reader(timespec * timestamp, timespec * value) {

}

/* End Timing Methods. */


/* Timestamping methods. */

/**
 * @brief called when the Timestamping node has to be switched on
 */
void terminal::switchon_timestamping() {
  m_Timestamping.setEnable(getCurrentTime(), 0 /* OFF */);
  m_Timestamping.setEdge(getCurrentTime(), 1 /* FALLING */);
  m_Timestamping.setMaxTimestamps(getCurrentTime(), 5);
  m_Timestamping.setOverflow(getCurrentTime(), 0 /* NO */);
}

/**
 * @brief called when the Timestamping node has to be switched off
 */
void terminal::switchoff_timestamping() {
  m_Timestamping.setEnable(getCurrentTime(), 0 /* OFF */);
}

/**
 * @brief Called when the Timestamping node has to start working.
 *        We start the Timestamping thread.
 */
void terminal::start_timestamping() {
  m_stop_timestamping = false; //< We will set to true to stop the Timestamping
				//  thread.
  /**
   *  Start the Timestamping thread. This function is called when the state
   *  machine goes from on to running. We don't need to check if the thread was
   *  already started because the state machine guarantees that the start handler
   *  is called only while the state is RUNNING.
   */
  m_timestamping_thread =
    std::thread(std::bind(&terminal::timestamping_thread_body, this));
}

// Stop the Timestamping node thread
void terminal::stop_timestamping() {
  m_stop_Timestamping = true;
  m_timestamping_thread.join();
}

// A failure during a state transition will cause the state machine to switch to
// the failure state. For now we don't plan for this and every time the
// state machine wants to recover we throw StateMachineRollBack to force the
// state machine to stay on the failure state.
void terminal::recover_timestamping() {
  throw nds::StateMachineRollBack("Cannot recover");
}

// Always allow for change state.
bool terminal::allow_timestamping_change(const nds::state_t,
			       const nds::state_t,
			       const nds::state_t) {
  return true;
}

// Timestamping setters

void terminal::pv_enable_writer(const timespec& timestamp, const std::int32_t& value) {
  // This function shall interact with hardware api to enable or disbale.
  m_Timestamping.setEnable(timestamp, value);
}

void terminal::pv_edge_writer(const timespec& timestamp, const std::int32_t& value) {
  // This function shall interact with api hardware.
  m_Timestamping.setEdge(timestamp, value);
}

void terminal::pv_clearoverflow_writer(const timespec& timestamp, const std::int32_t& value) {
  // This function may have to interact with api hardware.
  m_Ntimestamps = 0;
  m_Timestamping.setOverflow(getCurrentTime(), 0 /* NO */);
}



void terminal::timestamping_thread_body() {
 /// BEEEP y aqui que hacemos 
}

/* End Timestmaping methods. */
