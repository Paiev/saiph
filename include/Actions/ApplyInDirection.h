#ifndef ACTION_APPLY_IN_DIRECTION_H
#define ACTION_APPLY_IN_DIRECTION_H

#include "Inventory.h"
#include "Actions/Action.h"

namespace action {

	class ApplyInDirection : public Action {
	public:
		static const int ID;

		ApplyInDirection(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority, bool update_inventory = true) : Action(analyzer), _do_apply("a", priority), _apply_item(key, PRIORITY_CONTINUE_ACTION), _apply_direction(direction, PRIORITY_CONTINUE_ACTION), _update_inventory(update_inventory) {
		}

		virtual ~ApplyInDirection() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_apply;

			case 1:
				return _apply_item;

			case 2:
				return _apply_direction;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
				_sequence = 1;
			} else if (messages.find(MESSAGE_DIG_DIRECTION) != std::string::npos) {
				_sequence = 2;
			} else {
				/* mark inventory dirty if update_inventory is true */
				if (_update_inventory)
					Inventory::update();
				if (_sequence != 2)
					failed();
				_sequence = 3;
			}
		}

	private:
		const Command _do_apply;
		const Command _apply_item;
		const Command _apply_direction;
		const bool _update_inventory;
	};
}
#endif
