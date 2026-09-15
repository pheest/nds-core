#ifndef NDSREGISTER_DEVICE_H
#define NDSREGISTER_DEVICE_H

namespace nds
{

/**
 * @brief This is a class intended to be used as a static class for automatic registering of device supports
 *
 * No dll-interface: this template is instantiated by the driver module with the
 * driver's own class, so its members cannot be imported from the NDS3 library.
 */
template <class T>
class RegisterDevice
{
private:
    const std::string m_driverName;

public:
    RegisterDevice(const char *driverName) : m_driverName(std::string(driverName)) {
        nds::Factory::registerDriver(m_driverName, RegisterDevice<T>::allocateDevice, RegisterDevice<T>::deallocateDevice);
    }

    static void *allocateDevice(nds::Factory& factory, const std::string& device, const nds::namedParameters_t& parameters){
        return new T(factory, device, parameters);
    }

    static void deallocateDevice(void *device) {
        delete reinterpret_cast<T*>(device);
    }

    const char *getDriverName() const {
        return m_driverName.c_str();
    }

protected:
    RegisterDevice();

};
} /* namespace nds */


#endif /* NDSREGISTER_DEVICE_H */
