// Copied and modified code from "IfcPlusPlus" library.
// This library is available under the OpenSceneGraph Public License. Original copyright notice:

/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and / or modified under
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
* (at your option) any later version.The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* OpenSceneGraph Public License for more details.
*/

/* This file has been automatically generated using the TUM Open Infra Platform
Early Binding EXPRESS Generator. Any changes to this file my be lost in the future. */

#pragma once

#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <visit_struct/visit_struct.hpp>
#include "OpenInfraPlatform/IfcAlignment1x1/model/Object.h"

namespace OpenInfraPlatform
{
	namespace IfcAlignment1x1
	{
		// TYPE IfcActionSourceTypeEnum = ENUMERATION OF(DEAD_LOAD_GCOMPLETION_G1LIVE_LOAD_QSNOW_SWIND_WPRESTRESSING_PSETTLEMENT_UTEMPERATURE_TEARTHQUAKE_EFIREIMPULSEIMPACTTRANSPORTERECTIONPROPPINGSYSTEM_IMPERFECTIONSHRINKAGECREEPLACK_OF_FITBUOYANCYICECURRENTWAVERAINBRAKESUSERDEFINEDNOTDEFINED;
		class IfcActionSourceTypeEnum : public IfcAlignment1x1Type, public IfcAlignment1x1AbstractEnum
		{
		public:
			enum IfcActionSourceTypeEnumEnum
			{
				 ENUM_DEAD_LOAD_G,
				 ENUM_COMPLETION_G1,
				 ENUM_LIVE_LOAD_Q,
				 ENUM_SNOW_S,
				 ENUM_WIND_W,
				 ENUM_PRESTRESSING_P,
				 ENUM_SETTLEMENT_U,
				 ENUM_TEMPERATURE_T,
				 ENUM_EARTHQUAKE_E,
				 ENUM_FIRE,
				 ENUM_IMPULSE,
				 ENUM_IMPACT,
				 ENUM_TRANSPORT,
				 ENUM_ERECTION,
				 ENUM_PROPPING,
				 ENUM_SYSTEM_IMPERFECTION,
				 ENUM_SHRINKAGE,
				 ENUM_CREEP,
				 ENUM_LACK_OF_FIT,
				 ENUM_BUOYANCY,
				 ENUM_ICE,
				 ENUM_CURRENT,
				 ENUM_WAVE,
				 ENUM_RAIN,
				 ENUM_BRAKES,
				 ENUM_USERDEFINED,
				 ENUM_NOTDEFINED
			};

			IfcActionSourceTypeEnum();
			IfcActionSourceTypeEnum(IfcActionSourceTypeEnumEnum value);
			virtual ~IfcActionSourceTypeEnum();
			virtual const char* classname() const { return "IfcActionSourceTypeEnum"; }
			virtual void getStepParameter(std::stringstream& stream, bool is_select_type = false) const;
			static std::shared_ptr<IfcActionSourceTypeEnum> readStepData(const std::string& arg);
			IfcActionSourceTypeEnumEnum m_enum;
		}; // end class IfcActionSourceTypeEnum
	} // end namespace IfcAlignment1x1
} // end namespace OpenInfraPlatform

VISITABLE_STRUCT(OpenInfraPlatform::IfcAlignment1x1::IfcActionSourceTypeEnum, m_enum);
