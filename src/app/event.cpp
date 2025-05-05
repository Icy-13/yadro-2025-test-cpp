#include "event.h"
#include <utility>

namespace impl {

    event::event(std::string time, int event_id, std::string data) :
            time(std::move(time)), event_id(event_id), data(std::move(data)) {}

    event::event(std::string time, int event_id, std::string data, int table_id) :
            event(std::move(time), event_id, std::move(data)) {
        this->table_id = table_id;
    }

    std::istream &operator>>(std::istream &in, event &e) {
        in >> e.time >> e.event_id >> e.data;
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const event &e) {
        out << e.time << " " << e.event_id << " " << e.data;
        if (e.table_id) {
            out << " " << *e.table_id + 1;
        }
        return out;
    }
}