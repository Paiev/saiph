#ifndef ANALYZER_WISH_H
#define ANALYZER_WISH_H

#include <string>
#include <vector>
#include "Analyzers/Analyzer.h"

#define WISH_DEBUG_NAME "Wish] "

class Saiph;

namespace analyzer {
	class Wish : public Analyzer {
	public:
		Wish();

		void parseMessages(const std::string& messages);
		void analyze();
	private:
		bool MRarmor, MRcloak, haveMR;
		bool reflectionArmor, reflectionAmulet, reflectionShield, haveReflection;
		unsigned char wand_of_wishing_key;
		bool zapping_wand;
		std::string selectWish();
		unsigned long long extrinsics;
		unsigned long long currentExtrinsics();
		bool wearing(const std::string& name);
	};
}
#endif
