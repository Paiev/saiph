#ifndef EVENT_EAT_ITEM_H
#define EVENT_EAT_ITEM_H

#include "Event.h"

namespace event {

	class EatItem : public Event {
	public:
		static const int ID;

		EatItem() : Event("EatItem"), _key(0), _priority(0) {
		}

		virtual ~EatItem() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const unsigned char& key() {
			return _key;
		}

		virtual const int& priority() {
			return _priority;
		}

	private:
		unsigned char _key;
		int _priority;
	};
}
#endif