#include <functional>
#include <iostream>

#include <nds3/nds.h>
#include "terminal.h"


terminal::terminal(const std::string& name, nds::Node& parent):
  m_Ntimestamps(0) {


  m_timestamping = parent.addChild(nds::Timestamping<nds::timestamp_t>
	   ("Timestamping", 4,
	    std::bind(&terminal::switchon_timestamping, this),
	    std::bind(&terminal::switchoff_timestamping, this),
	    std::bind(&terminal::start_timestamping, this),
	    std::bind(&terminal::stop_timestamping, this),
	    std::bind(&terminal::recover_timestamping, this),
	    std::bind(&terminal::allow_timestamping_change, this,
		      std::placeholders::_1,
		      std::placeholders::_2,
		      std::placeholders::_3),
	    std::bind(&terminal::pv_enable_writer, this,
		      std::placeholders::_1, std::placeholders::_2),
	    std::bind(&terminal::pv_edge_writer, this,
		      std::placeholders::_1, std::placeholders::_2),
	    std::bind(&terminal::pv_clearoverflow_writer, this,
		      std::placeholders::_1, std::placeholders::_2)));


  // INICIALIZAR TIMING

  // Add Timing node
  m_timing = parent.addChild(nds::Timing("Timing",
	   std::bind(&terminal::switchon_timing, this),
	   std::bind(&terminal::switchoff_timing, this),
	   std::bind(&terminal::start_timing, this),
	   std::bind(&terminal::stop_timing, this),
	   std::bind(&terminal::recover_timing, this),
	   std::bind(&terminal::allow_timing_change, this,
		     std::placeholders::_1,
		     std::placeholders::_2,
		     std::placeholders::_3),
	   std::bind(&terminal::pv_timing_reader, this,
		     std::placeholders::_1,
		     std::placeholders::_2)));

}



terminal::~terminal() {

}

/* Timing methods. */

void terminal::switchon_timing() {
  // Call API HW to set clock grequency.
  m_timing.setClkFrequency(m_timing.getTime(),100.001);
  // Call API HW to set Clock multiplier.
  m_timing.setClkMultiplier(m_timing.getTime(),2);
  // Call API HW to set synching status.
  m_timing.setSyncStatus(m_timing.getTime(),1 /* SYNCING */);
  // Call API HW to set seconds since last sync.
  m_timing.setSecsLastSync(m_timing.getTime(), 0);
  // Call API HW to set reference time base.
  m_timing.setRefTimeBase(m_timing.getTime(), m_timing.getTime());
}

void terminal::switchoff_timing() {

  // Call API HW to set synching status.
  m_timing.setSyncStatus(m_timing.getTime(),0 /* NOT_SYNC */);
  // Call API HW to set seconds since last sync.
  m_timing.setSecsLastSync(m_timing.getTime(), 10);
}

void terminal::start_timing() {

  // Call API HW to set synching status.
  m_timing.setSyncStatus(m_timing.getTime(),2 /* SYNCED */);

  m_stop_timing = false; //< We will set to true to stop the Timing thread
  /**
   *   Start the Timing thread.
   *   We don't need to check if the thread was already started because the state
   *   machine guarantees that the start handler is called only while the state
   *   is ON.
   */
  m_timing_thread = std::thread(std::bind(&terminal::timing_thread_body, this));
}

void terminal::stop_timing() {

  m_stop_timing = true;
  m_timing_thread.join();
}

void terminal::recover_timing() {

  throw nds::StateMachineRollBack("Cannot recover");
}

bool terminal::allow_timing_change(const nds::state_t,
				   const nds::state_t,
				   const nds::state_t) {

  return true;

}

void terminal:: pv_timing_reader(timespec * timestamp, timespec * value) {

  // Get clock grequency.
  double clk_freq = m_timing.getClkFrequency();
  // Get Clock multiplier.
  std::int32_t clk_mult = m_timing.getClkMultiplier();
  // Get synching status.
  std::int32_t sync_status = m_timing.getSyncStatus();
  // Get seconds since last sync.
  std::int32_t secs = m_timing.getSecsLastSync();
  // Get reference time base.
  timespec time_base = m_timing.getRefTimeBase();

  std::cout << "Timing support information:" << std::endl;
  std::cout << "\tClock frequency = " << clk_freq << std::endl;
  std::cout << "\tClock multiplier = " << clk_mult << std::endl;
  std::cout << "\tSync. status = " << sync_status << std::endl;
  std::cout << "\tSeconds since last sync. = " << secs << std::endl;
  std::cout << "\tReference time base = {" << time_base.tv_sec
	    << ", " << time_base.tv_nsec << "}" << std::endl;

  // Run until the state machine stops us
  while(!m_stop_timing){

    // Get Self-Test enable
    timespec time_val = m_timing.getTime();
    m_timing.setTime(m_timing.getTime(), time_val);
    ::usleep(1000000);
  }
}

/* End Timing Methods. */


/* Timestamping methods. */

/**
 * @brief called when the Timestamping node has to be switched on
 */
void terminal::switchon_timestamping() {
  m_timestamping.setEnable(m_timing.getTime(), 0 /* OFF */);
  m_timestamping.setEdge(m_timing.getTime(), 1 /* FALLING */);
  m_timestamping.setMaxTimestamps(m_timing.getTime(), 5);
  m_timestamping.setOverflow(m_timing.getTime(), 0 /* NO */);
}

/**
 * @brief called when the Timestamping node has to be switched off
 */
void terminal::switchoff_timestamping() {
  m_timestamping.setEnable(m_timing.getTime(), 0 /* OFF */);
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
  m_stop_timestamping = true;
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
  m_timestamping.setEnable(timestamp, value);
}

void terminal::pv_edge_writer(const timespec& timestamp, const std::int32_t& value) {
  // This function shall interact with api hardware.
  m_timestamping.setEdge(timestamp, value);
}

void terminal::pv_clearoverflow_writer(const timespec& timestamp, const std::int32_t& value) {
  // This function may have to interact with api hardware.
  m_Ntimestamps = 0;
  m_timestamping.setOverflow(m_timing.getTime(), 0 /* NO */);
}



void terminal::timestamping_thread_body() {
 /// BEEEP y aqui que hacemos
}

/* End Timestmaping methods. */
