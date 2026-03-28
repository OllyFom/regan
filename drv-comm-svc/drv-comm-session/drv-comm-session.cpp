#include "drv-comm-session.hpp"

DrvCommSession::DrvCommSession(const RegUnitSnap& reg_snap)
    : reg_unit_info_(std::move(std::make_unique<RegUnitSnap>(reg_snap)))
{
    OpenSession();
}

DrvCommSession::~DrvCommSession()
{
    CloseSession();
}

void DrvCommSession::OpenSession()
{
}
void DrvCommSession::CloseSession()
{
}

void DrvCommSession::Start()
{
}

void DrvCommSession::EventProcessing()
{
}

void DrvCommSession::EventHandling()
{
}