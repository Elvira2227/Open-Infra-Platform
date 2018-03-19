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
		// TYPE IfcSpaceTypeEnum = ENUMERATION OF(SPACEPARKINGGFAINTERNALEXTERNALUSERDEFINEDNOTDEFINED;
		class IfcSpaceTypeEnum : public IfcAlignment1x1Type, public IfcAlignment1x1AbstractEnum
		{
		public:
			enum IfcSpaceTypeEnumEnum
			{
				 ENUM_SPACE,
				 ENUM_PARKING,
				 ENUM_GFA,
				 ENUM_INTERNAL,
				 ENUM_EXTERNAL,
				 ENUM_USERDEFINED,
				 ENUM_NOTDEFINED
			};

			IfcSpaceTypeEnum();
			IfcSpaceTypeEnum(IfcSpaceTypeEnumEnum value);
			virtual ~IfcSpaceTypeEnum();
			virtual const char* classname() const { return "IfcSpaceTypeEnum"; }
			virtual void getStepParameter(std::stringstream& stream, bool is_select_type = false) const;
			static std::shared_ptr<IfcSpaceTypeEnum> readStepData(const std::string& arg);
			IfcSpaceTypeEnumEnum m_enum;
		}; // end class IfcSpaceTypeEnum
	} // end namespace IfcAlignment1x1
} // end namespace OpenInfraPlatform

VISITABLE_STRUCT(OpenInfraPlatform::IfcAlignment1x1::IfcSpaceTypeEnum, m_enum);
