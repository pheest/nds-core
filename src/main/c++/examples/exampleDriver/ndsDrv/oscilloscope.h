#include <nds3/nds.h>
#include "channel.h"
#include "terminal.h"



class oscilloscope {

 public:


  /**
   *
   */
  oscilloscope(nds::Factory& factory, const std::string& device,
	       const nds::namedParameters_t& parameters);
  /**
   *
   */
  ~oscilloscope();

  //////////////////////////////////////////////////////////////////////////////
  /// Firmware Node ////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  /**
   *
   */
  nds::Firmware m_firmware;
  /**
   *
   */
  void switchOn_firmware();
  /**
   *
   */
  void switchOff_firmware();
  /**
   *
   */
  void start_firmware();
  /**
   *
   */
  void stop_firmware();
  /**
   *
   */
  void recover_firmware();
  /**
   *
   */
  bool allow_firmware_change(const nds::state_t,
			     const nds::state_t,
			     const nds::state_t);
  /**
   *
   */
  void pv_path_writer(const timespec& timestamp, const std::string& value);


  /**
   *
   */
  void firmware_thread_body();
  /**
   * @brief A thread that runs Firmware_thread_body().
   */
  std::thread m_firmware_thread;
  /**
   * @brief A boolean flag that stop the Firmware loop in Firmware_thread_body()
   *        when true.
   */
  volatile bool m_stop_firmware;

  ////////////////////////
  /// End Firmware Node //
  ////////////////////////

 protected:

 private:

  std::vector<std::shared_ptr<channel>> m_channels;
  std::vector<std::shared_ptr<terminal>> m_terminals;

};
