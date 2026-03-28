#include <memory>
#include <thread>

#include "../accessory/reg_unit_snapshot.hpp"

#define DriverNotificationPort "Some_Port_Name"

// TODO: обеспечить единственность объекта класса сессии с драйвером

class DrvCommSession final
{
public:
    DrvCommSession() = delete;
    DrvCommSession(const RegUnitSnap& reg_snap);

    DrvCommSession operator=(DrvCommSession &&) = delete;
    DrvCommSession operator=(const DrvCommSession &) = delete;
    DrvCommSession(const DrvCommSession &) = delete;
    DrvCommSession(DrvCommSession &&) = delete;

    ~DrvCommSession();

    void Start();

private:

    void OpenSession();
    void CloseSession();

    void EventProcessing();
    /// @brief получение дополнительной информации о событии, пришедшем из драйвера (получение имени и пути до исполняемого файла, который поставила на автозагрузку)
    /// + добавить данные в БД
    void EventHandling();
    
private:
    // some threadsafety queue

    // Cache registry class: .../Run info for compare timestamp
    std::unique_ptr<RegUnitSnap> reg_unit_info_;

    std::thread event_processing_thread_;
    std::thread event_handling_thread_;
};