#ifndef ACTION_ENHANCE_H
#define ACTION_ENHANCE_H

#include "Actions/Action.h"

namespace action {
	class Enhance : public Action {
	public:
		static const int ID;

		Enhance(analyzer::Analyzer* analyzer);
		virtual ~Enhance();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _do_enhance;
		const Command _enhance_a;
	};
}
#endif
