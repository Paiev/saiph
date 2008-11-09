#include "Ring.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Ring::Ring(Saiph *saiph) : Analyzer("Ring"), saiph(saiph) {
}

/* methods */
void Ring::analyze() {
	if (saiph->inventory_changed)
		wearRing();
}

void Ring::parseMessages(const string &messages) {
	if (sequence == 0 && saiph->world->question && (messages.find(MESSAGE_WHAT_TO_PUT_ON, 0) != string::npos || messages.find(MESSAGE_WHAT_TO_REMOVE, 0) != string::npos)) {
		/* put on or remove a ring */
		++sequence;
	} else if (sequence == 1 && saiph->world->question && messages.find(MESSAGE_WHICH_RING_FINGER, 0) != string::npos) {
		/* need to specify which finger the ring should go on */
		++sequence;
	} else if (sequence > 0) {
		/* if sequence > 0 and the above checks failed,
		 * it probably means we put on/removed a ring */
		sequence = -1;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	}
}

bool Ring::request(const Request &request) {
	if (request.request == REQUEST_RING_WEAR) {
		/* player wish to wear this ring */
		WearRing wr;
		wr.beatitude = request.status;
		wr.name = request.data;
		rings.push_back(wr);
		return true;
	}
	return false;
}

/* private methods */
void Ring::wearRing() {
	/* find out which rings we should wear */
	unsigned char ring_on_right = 0;
	int ring_on_right_pri = INT_MAX;
	bool ring_on_right_cursed = false;
	unsigned char ring_on_left = 0;
	int ring_on_left_pri = INT_MAX;
	bool ring_on_left_cursed = false;
	unsigned char best_key = 0;
	int best_ring = INT_MAX;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional.find("on right ", 0) == 0) {
			ring_on_right = i->first;
			if (i->second.beatitude == CURSED)
				ring_on_right_cursed = true;
		} else if (i->second.additional.find("on left ", 0) == 0) {
			ring_on_left = i->first;
			if (i->second.beatitude == CURSED)
				ring_on_left_cursed = true;
		}
		for (vector<WearRing>::size_type w = 0; w < rings.size(); ++w) {
			if (ring_on_right != 0 && ring_on_right_pri == INT_MAX && rings[w].name == i->second.name)
				ring_on_right_pri = w;
			if (ring_on_left != 0 && ring_on_left_pri == INT_MAX && rings[w].name == i->second.name)
				ring_on_left_pri = w;
			if (rings[w].name != i->second.name || (int) w >= best_ring)
				continue;
			else if ((rings[w].beatitude & i->second.beatitude) == 0)
				continue;
			best_key = i->first;
			best_ring = w;
		}
	}
	if (ring_on_right_cursed && ring_on_left_cursed)
		return; // both rings cursed, return for now
	else if (best_key == 0)
		return; // we got no ring to put on
	else if (ring_on_right_pri <= best_ring && ring_on_left_pri <= best_ring)
		return; // already wearing just as good or better rings
	if (ring_on_right != 0 && ring_on_left != 0) {
		/* must remove one ring before we can put on the new ring */
		setCommand(0, RING_WEAR_PRIORITY, REMOVE);
		/* in case we get question about which ring to remove */
		if (ring_on_right_pri < ring_on_left_pri)
			setCommand(1, PRIORITY_CONTINUE_ACTION, string(1, ring_on_left));
		else
			setCommand(1, PRIORITY_CONTINUE_ACTION, string(1, ring_on_right));
		sequence = 0;
	} else {
		/* no need to remove any rings, just put on the new ring */
		setCommand(0, RING_WEAR_PRIORITY, PUT_ON);
		setCommand(1, PRIORITY_CONTINUE_ACTION, string(1, best_key));
		/* in case we get question about which hand to put ring on */
		setCommand(2, PRIORITY_CONTINUE_ACTION, "l");
		sequence = 0;
	}
}
