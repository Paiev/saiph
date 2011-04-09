#ifndef ACTION_NAME_H
#define ACTION_NAME_H

#include "Inventory.h"
#include "World.h"
#include "Actions/Action.h"

namespace action {

	class Name : public Action {
	public:
		static const int ID;

		Name(analyzer::Analyzer* analyzer, unsigned char item, const std::string& name) : Action(analyzer, false), _do_name("#name\n", PRIORITY_NAME_ITEM), _name_individual(YES, PRIORITY_CONTINUE_ACTION), _name_item(item, PRIORITY_CONTINUE_ACTION), _set_name(name + "\n", PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Name() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_name;

			case 1:
				return _name_individual;

			case 2:
				return _name_item;

			case 3:
				Inventory::update();
				return _set_name;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_NAME_INDIVIDUAL_OBECT) != std::string::npos || (World::menu() && messages.find(MESSAGE_WHAT_TO_NAME_TYPE) != std::string::npos)) {
				_sequence = 1;
			} else if (messages.find(MESSAGE_ITEM_TO_NAME) != std::string::npos) {
				_sequence = 2;
			} else if (messages.find(MESSAGE_WHAT_TO_NAME_ITEM) != std::string::npos) {
				_sequence = 3;
			} else if (_sequence == 3) {
				/* we need to mark inventory as not updated */
				Inventory::update();
				_sequence = 4;
			}
		}

	private:
		const Command _do_name;
		const Command _name_individual;
		const Command _name_item;
		const Command _set_name;
	};
}
#endif
