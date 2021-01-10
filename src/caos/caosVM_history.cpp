/*
 *  caosVM_history.cpp
 *  openc2e
 *
 *  Created by Alyssa Milburn on Mon Dec 19 2005.
 *  Copyright (c) 2005 Alyssa Milburn. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

#include "caos_assert.h"
#include "caosVM.h"
#include "historyManager.h"
#include "World.h"

/*
 * TODO:
 *
 * monikers are auto-created when referenced by name, so the hasMoniker() asserts should be removed
 */

/**
 HIST CAGE (integer) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_CAGE(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	vm->result.setInt(m.events[event].stage);
}

/**
 HIST COUN (integer) moniker (string)
 %status maybe
*/
void v_HIST_COUN(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	if (world.history->hasMoniker(moniker)) {
		monikerData &m = world.history->getMoniker(moniker);
		vm->result.setInt(m.events.size());
	} else {
		vm->result.setInt(0);
	}
}

/**
 HIST CROS (integer) moniker (string)
 %status maybe
*/
void v_HIST_CROS(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	vm->result.setInt(world.history->getMoniker(moniker).no_crossover_points);
}

/**
 HIST EVNT (command) moniker (string) type (integer) relatedmoniker1 (string) relatedmoniker2 (string)
 %status maybe

 Fire a life event of the specified type with the specified moniker.
*/
void c_HIST_EVNT(caosVM *vm) {
	VM_PARAM_STRING(relatedmoniker2)
	VM_PARAM_STRING(relatedmoniker1)
	VM_PARAM_INTEGER(type)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	m.addEvent(type, relatedmoniker1, relatedmoniker2);
}

/**
 HIST FIND (integer) moniker (string) event (integer) from (integer)
 %status maybe
*/
void v_HIST_FIND(caosVM *vm) {
	VM_PARAM_INTEGER(from)
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	for (int i = from + 1; i >= 0 && (unsigned int)i < m.events.size(); i++) {
		if (m.events[i].eventno == (unsigned int)event) {
			vm->result.setInt(i);
			return;
		}
	}

	vm->result.setInt(-1);
}

/**
 HIST FINR (integer) moniker (string) event (integer) from (integer)
 %status maybe
*/
void v_HIST_FINR(caosVM *vm) {
	VM_PARAM_INTEGER(from)
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	if (from == -1) from = m.events.size();

	for (int i = from - 1; i >= 0 && (unsigned int)i < m.events.size(); i--) {
		if (m.events[i].eventno == (unsigned int)event) {
			vm->result.setInt(i);
			return;
		}
	}

	vm->result.setInt(-1);
}

/**
 HIST FOTO (string) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_FOTO(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setString(m.events[event].photo);
}

/**
 HIST FOTO (command) moniker (string) event (integer) photo (string)
 %status maybe
*/
void c_HIST_FOTO(caosVM *vm) {
	VM_PARAM_STRING(photo)
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	// TODO: handle magic (ie, atticing old photo)
	m.events[event].photo = photo;	
}

/**
 HIST GEND (integer) moniker (string)
 %status maybe
*/
void v_HIST_GEND(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	vm->result.setInt(m.gender);
}

/**
 HIST GNUS (integer) moniker (string)
 %status maybe
*/
void v_HIST_GNUS(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	vm->result.setInt(m.genus);
}

/**
 HIST MON1 (string) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_MON1(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setString(m.events[event].monikers[0]);
}

/**
 HIST MON2 (string) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_MON2(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setString(m.events[event].monikers[1]);
}

/**
 HIST MUTE (integer) moniker (string)
 %status maybe
*/
void v_HIST_MUTE(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	vm->result.setInt(m.no_point_mutations);
}

/**
 HIST NAME (string) moniker (string)
 %status maybe

 Return the name of the creature with the given moniker.
*/
void v_HIST_NAME(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	vm->result.setString(m.name);
}

/**
 HIST NAME (command) moniker (string) name (string)
 %status maybe
*/
void c_HIST_NAME(caosVM *vm) {
	VM_PARAM_STRING(name)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	m.name = name;
}

/**
 HIST NETU (string) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_NETU(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setString(m.events[event].networkid);
}

/**
 HIST NEXT (string) moniker (string)
 %status stub
*/
void v_HIST_NEXT(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	vm->result.setString(""); // TODO
}

/**
 HIST PREV (string) moniker (string)
 %status stub
*/
void v_HIST_PREV(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	vm->result.setString(""); // TODO
}

/**
 HIST RTIM (integer) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_RTIM(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setInt(m.events[event].timestamp);
}

/**
 HIST TAGE (integer) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_TAGE(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setInt(m.events[event].tage);
}

/**
 HIST TYPE (integer) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_TYPE(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setInt(m.events[event].eventno);
}

/**
 HIST UTXT (command) moniker (string) event (integer) value (string)
 %status maybe
*/
void c_HIST_UTXT(caosVM *vm) {
	VM_PARAM_STRING(value)
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	m.events[event].usertext = value;
}

/**
 HIST UTXT (string) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_UTXT(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setString(m.events[event].usertext);
}

/**
 HIST VARI (integer) moniker (string)
 %status maybe
*/
void v_HIST_VARI(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	vm->result.setInt(m.variant);
}

/**
 HIST WIPE (command) moniker (string)
 %status stub
*/
void c_HIST_WIPE(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	// TODO
}

/**
 HIST WNAM (string) moniker (string) event (integer)
 %status stub
*/
void v_HIST_WNAM(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setString(m.events[event].worldname); // TODO
}

/**
 HIST WTIK (integer) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_WTIK(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setInt(m.events[event].worldtick);
}

/**
 HIST WUID (string) moniker (string) event (integer)
 %status maybe
*/
void v_HIST_WUID(caosVM *vm) {
	VM_PARAM_INTEGER(event)
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);
	caos_assert(event >= 0 && (unsigned int)event < m.events.size());
	
	vm->result.setString(m.events[event].worldmoniker);
}

/**
 HIST WVET (integer) moniker (string)
 %status maybe
*/
void v_HIST_WVET(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	caos_assert(world.history->hasMoniker(moniker));
	monikerData &m = world.history->getMoniker(moniker);

	if (m.warpveteran)
		vm->result.setInt(1);
	else
		vm->result.setInt(0);
}

/**
 OOWW (integer) moniker (string)
 %status maybe

 Return the current status of the given moniker.
 0 is unknown (never existed?), 1 is in slot, 2 is NEW: CREA creature, 3 is BORN creature, 4 is exported, 5 is dead, 6 is gone (no body), 7 is unreferenced.
*/
void v_OOWW(caosVM *vm) {
	VM_PARAM_STRING(moniker)

	vm->result.setInt(0);

	if (world.history->hasMoniker(moniker)) {
		monikerData &m = world.history->getMoniker(moniker);

		vm->result.setInt((int)m.getStatus());
	}
}

/**
 EVNT (command) object (agent)
 %status stub
 %variants c1 c2
*/
void c_EVNT(caosVM *vm) {
	VM_PARAM_VALIDAGENT(object)

	// TODO
}

/**
 RMEV (command) object (agent)
 %status stub
 %variants c1 c2
*/
void c_RMEV(caosVM *vm) {
	VM_PARAM_VALIDAGENT(object)

	// TODO
}

/**
 DDE: NEGG (command)
 %status stub
 %variants c1 c2
*/
void c_DDE_NEGG(caosVM*) {
	// TODO
}

/**
 DDE: DIED (command)
 %status stub
 %variants c1 c2
*/
void c_DDE_DIED(caosVM*) {
	// TODO
}

/**
 DDE: LIVE (command)
 %status stub
 %variants c1 c2
*/
void c_DDE_LIVE(caosVM*) {
	// TODO
}


/* vim: set noet: */
