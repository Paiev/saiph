#ifndef ACTION_CALL_H
#define ACTION_CALL_H

#include "Inventory.h"
#include "World.h"
#include "Actions/Action.h"

namespace action {

	class Call : public Action {
	public:
		static const int ID;

		Call(analyzer::Analyzer* analyzer, unsigned char item, const std::string& call) : Action(analyzer, false), _do_call("#name\n", PRIORITY_NAME_ITEM), _call_individual("n", PRIORITY_CONTINUE_ACTION), _call_item(item, PRIORITY_CONTINUE_ACTION), _set_call(call + "\n", PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Call() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_call;

			case 1:
				return _call_individual;

			case 2:
				return _call_item;

			case 3:
				Inventory::update();
				return _set_call;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_NAME_INDIVIDUAL_OBECT) != std::string::npos || (World::menu() && messages.find(MESSAGE_WHAT_TO_NAME_TYPE) != std::string::npos)) {
				_sequence = 1;
			} else if (messages.find(MESSAGE_ITEM_TO_CALL) != std::string::npos) {
				_sequence = 2;
			} else if (messages.find(MESSAGE_WHAT_TO_CALL_ITEM) != std::string::npos) {
				_sequence = 3;
			} else if (_sequence == 3) {
				/* we need to mark inventory as not updated */
				Inventory::update();
				_sequence = 4;
			}
		}

	private:
		const Command _do_call;
		const Command _call_individual;
		const Command _call_item;
		const Command _set_call;
	};
}
#endif
