#include "Analyzer.h"
#include "Globals.h"

using namespace std;

/* constructors/destructor */
Analyzer::Analyzer(string name) : name(name), commands(1), sequence(-1) {
	/* methods will be called in this order:
	 * 0. init (once, after all analyzers have been created)
	 * 1. parseMessages
	 * 2. inspect (unless question, menu or engulfed)
	 * 3. analyze (unless question or menu)
	 * 3. complete/fail (only called for analyzer that got its command through)
	 *
	 * additionally, requests may come at any time */
}

Analyzer::~Analyzer() {
	/* we must have a virtual destructor.
	 * if we don't then the destructor of classes inheriting
	 * this class won't be called upon destruction */
}

/* methods */
void Analyzer::analyze() {
}

void Analyzer::complete() {
}

void Analyzer::fail() {
}

void Analyzer::init() {
}

void Analyzer::inspect(const Point &point) {
}

void Analyzer::parseMessages(const string &messages) {
}

bool Analyzer::request(const Request &request) {
	return false;
}

bool Analyzer::setCommand(int index, int priority, const string &data) {
	if (index < 0 || index + 1 > (int) commands.size())
		return false;
	if (index < (int) commands.size())
		commands[index].setCommand(priority, data);
	else
		commands.push_back(Command(priority, data));
	return true;
}
