#include "event.h"

Dispatcher::Dispatcher(SlotType&& slot)
    : observer(slot) {}


void Dispatcher::Post( const Event& event ) const
{
    observer(event);
}
