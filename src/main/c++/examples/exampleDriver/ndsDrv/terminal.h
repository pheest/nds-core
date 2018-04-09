#include <nds3/nds.h>

// POR QUE TODO PUBLICO? 

class terminal {

 public:

  /**
   *
   */
  terminal(const std::string& name, nds::Node& parent);
  /**
   *
   */
  ~terminal();

  //////////////////////////////////////////////////////////////////////////////
  /// Timing Node //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  /**
   *
   */
  nds::Timing m_timing;

  /**
   *
   */
  void switchon_timing();
  /**
   *
   */
  void switchoff_timing();
  /**
   *
   */
  void start_timing();
  /**
   *
   */
  void stop_timing();
  /**
   *
   */
  void recover_timing();
  /**
   *
   */
  bool allow_timing_change(const nds::state_t,
			   const nds::state_t,
			   const nds::state_t);
  /**
   *
   */
  void  pv_timing_reader(timespec * timestamp, timespec * value);
  /**
   *
   */
  std::thread m_timing_thread;
  /**
   *
   */
  volatile bool m_stop_timing;

  //////////////////////////
  // End Timing ////////////
  //////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  /// Timestamping Node ////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Timestamping node
   */
  nds::Timestamping<nds::timestamp_t> m_Timestamping;

  /**
   * Methods to control timestamping  state machine
   */
  void switchon_timestamping();    ///< Called to switch on the Timestamping node.
  void switchoff_timestamping();   ///< Called to switch off the Timestamping node.
  void start_timestamping();       ///< Called to start the Timestamping node.
  void stop_timestamping();        ///< Called to stop the Timestamping node.
  void recover_timestamping();     ///< Called to recover the Timestamping node from a failure.
  bool allow_timestamping_change(const nds::state_t,
                                  const nds::state_t,
				 const nds::state_t); // Called to verify if a state change is allow 
  /**
   *  Timestamping setters 
   */
  void pv_enable_writer(const timespec& timestamp, const std::int32_t& value);
  void pv_edge_writer(const timespec& timestamp, const std::int32_t& value);
  void pv_clearoverflow_writer(const timespec& timestamp, const std::int32_t& value);


  /**
   *  @brief Cuerpo de la funcion que se corre en el thread CAMBIAR ESTO 
   */
  void timestamping_thread_body();
  /**
   * @brief A thread that runs timestamping_thread_body()
   */
  std::thread m_timestamping_thread;
  /**
   * @brief A boolean flag that stops the Timestamping loop in timestamping_thread_body()
   */
  volatile bool m_stop_timestamping;

  //////////////////////////
  // End Timestamping //////
  //////////////////////////

 protected:

 private:

};
