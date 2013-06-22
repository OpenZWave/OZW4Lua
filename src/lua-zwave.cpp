/*
 *  Open Z-Wave for Lua
 *  Copyright (C) 2013  justin@dynam.ac
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Created on: 15 Feb, 2013
 *      Author: fish
 */


#include "config.h"
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <luabind/object.hpp>
#include <luabind/raw_policy.hpp>
#include <boost/format.hpp>
#include <Manager.h>
#include <Options.h>
#include <Notification.h>
#include <Log.h>
#include "openzwavelua.hpp"


using namespace OpenZWave;
using namespace luabind;

luabind::object luaGetAssociations(Manager *, uint32 _homeId, uint8 _nodeId, uint8 _groupIdx);

OZW4Lua *OZW4Lua::Self = NULL;


template<class T> struct stupid_ptr {
	public:
		stupid_ptr(T* p) : m_p(p) {

		}
		friend T* get_pointer(stupid_ptr const& p) {
			return p.m_p;
		}
	private:
		T* m_p;
};

namespace luabind {

	template<class T> stupid_ptr<T const> get_const_holder(stupid_ptr<T>*) {
			return 0;
	}
}



OZW4Lua::OZW4Lua() {
    this->Self = this;
    this->L = luaL_newstate();
    luaL_openlibs(this->L);
    luabind::open(this->L);
                                            


  /* Create a Entry for the Notification API */
  module(this->L, "OpenZWave")
  [
    class_<Node, stupid_ptr<Node> >("Node")
      .def(luabind::constructor<uint32, uint8>())
      .enum_("QueryStage")
      [
        value("QueryStage_ProtocolInfo", Node::QueryStage_ProtocolInfo),
        value("QueryStage_Probe", Node::QueryStage_Probe),
        value("QueryStage_WakeUp", Node::QueryStage_WakeUp),
        value("QueryStage_ManufacturerSpecific1", Node::QueryStage_ManufacturerSpecific1),
        value("QueryStage_NodeInfo", Node::QueryStage_NodeInfo),
        value("QueryStage_ManufacturerSpecific2", Node::QueryStage_ManufacturerSpecific2),
        value("QueryStage_Versions", Node::QueryStage_Versions),
        value("QueryStage_Instances", Node::QueryStage_Instances),
        value("QueryStage_Static", Node::QueryStage_Static),
        value("QueryStage_Probe1", Node::QueryStage_Probe1),
        value("QueryStage_Associations", Node::QueryStage_Associations),
        value("QueryStage_Neighbors", Node::QueryStage_Neighbors),
        value("QueryStage_Session", Node::QueryStage_Session),
        value("QueryStage_Dynamic", Node::QueryStage_Dynamic),
        value("QueryStage_Configuration", Node::QueryStage_Configuration),
        value("QueryStage_Complete", Node::QueryStage_Complete),
        value("QueryStage_None", Node::QueryStage_None)
      ]
      .def("GetCurrentQueryStage", &Node::GetCurrentQueryStage)
      .def("GetQueryStageName", &Node::GetQueryStageName)
      .def("IsNodeAlive", &Node::IsNodeAlive)
      .def("ProtocolInfoReceived", &Node::ProtocolInfoReceived)
      .def("NodeInfoReceived", &Node::NodeInfoReceived)
      .def("AllQueriesCompleted", &Node::AllQueriesCompleted)
      .def("IsListeningDevice", &Node::IsListeningDevice)
      .def("IsFrequentListeningDevice", &Node::IsFrequentListeningDevice)
      .def("IsBeamingDevice", &Node::IsBeamingDevice)
      .def("IsRoutingDevice", &Node::IsRoutingDevice)
      .def("IsSecurityDevice", &Node::IsSecurityDevice)
      .def("GetMaxBaudRate", &Node::GetMaxBaudRate)
      .def("GetVersion", &Node::GetVersion)
      .def("GetSecurity", &Node::GetSecurity)
      .def("GetNodeId", &Node::GetNodeId)
      .def("GetBasic", &Node::GetBasic)
      .def("GetGeneric", &Node::GetGeneric)
      .def("GetSpecific", &Node::GetSpecific)
      .def("GetType", &Node::GetType)
      .def("GetNeighbors", &Node::GetNeighbors)
      .def("IsController", &Node::IsController)
      //.def("GetCommandClass", &Node::GetCommandClass)
      .def("SetLevel", &Node::SetLevel)
      .def("SetNodeOn", &Node::SetNodeOn)
      .def("SetNodeOff", &Node::SetNodeOff)
      .def("CreateValueID", &Node::CreateValueID)
      //.def("GetValue", &Node::GetValue)
      .def("RemoveValue", &Node::RemoveValue)
      .def("CreateValueBool", &Node::CreateValueBool)
      .def("CreateValueButton", &Node::CreateValueButton)
      .def("CreateValueByte", &Node::CreateValueByte)
      .def("CreateValueDecimal", &Node::CreateValueDecimal)
      .def("CreateValueInt", &Node::CreateValueInt)
      //.def("CreateValueList", &Node::CreateValueList)
      //.def("CreateValueRaw", &Node::CreateValueRaw)
      .def("CreateValueSchedule", &Node::CreateValueSchedule)
      .def("CreateValueShort", &Node::CreateValueShort)
      .def("CreateValueString", &Node::CreateValueString)
      .def("RemoveValueList", &Node::RemoveValueList)
      //.def("ReadValueFromXML", &Node::ReadValueFromXML)
      //.def("CreateValueFromXML", &Node::CreateValueFromXML)
      .scope
      [
        class_<Node::CommandClassData>("CommandClassData")
          .def_readonly("m_commandClassId", &Node::CommandClassData::m_commandClassId)
          .def_readonly("m_sentCnt", &Node::CommandClassData::m_sentCnt)
          .def_readonly("m_receivedCnt", &Node::CommandClassData::m_receivedCnt)
      ]
      .scope
      [
        class_<Node::NodeData>("NodeData")
          .def_readonly("m_sentCnt", &Node::NodeData::m_sentCnt)
          .def_readonly("m_sentFailed", &Node::NodeData::m_sentFailed)
          .def_readonly("m_retries", &Node::NodeData::m_retries)
          .def_readonly("m_receivedCnt", &Node::NodeData::m_receivedCnt)
          .def_readonly("m_receivedDups", &Node::NodeData::m_receivedDups)
          .def_readonly("m_receivedUnsolicited", &Node::NodeData::m_receivedUnsolicited)
          .def_readonly("m_sentTS", &Node::NodeData::m_sentTS)
          .def_readonly("m_receivedTS", &Node::NodeData::m_receivedTS)
          .def_readonly("m_lastRequestRTT", &Node::NodeData::m_lastRequestRTT)
          .def_readonly("m_averageRequestRTT", &Node::NodeData::m_averageRequestRTT)
          .def_readonly("m_lastResponseRTT", &Node::NodeData::m_lastResponseRTT)
          .def_readonly("m_averageResponseRTT", &Node::NodeData::m_averageResponseRTT)
          .def_readonly("m_quality", &Node::NodeData::m_quality)
          .def_readonly("m_lastReceivedMessage", &Node::NodeData::m_lastReceivedMessage)
          .def_readonly("m_ccData", &Node::NodeData::m_ccData)
      ]
    ];

  module(this->L, "OpenZWave")
    [
    //class_<Manager, stupid_ptr<Manager> >("Manager")
     class_<Manager>("Manager")
      .def("WriteConfig", &Manager::WriteConfig)
      .def("GetOptions", &Manager::GetOptions)
      .def("GetControllerNodeId", &Manager::GetControllerNodeId)
      .def("GetSUCNodeId", &Manager::GetSUCNodeId)
      .def("IsPrimaryController", &Manager::IsPrimaryController)
      .def("IsStaticUpdateController", &Manager::IsStaticUpdateController)
      .def("IsBridgeController", &Manager::IsBridgeController)
      .def("GetLibraryVersion", &Manager::GetLibraryVersion)
      .def("GetLibraryTypeName", &Manager::GetLibraryTypeName)
      .def("GetSendQueueCount", &Manager::GetSendQueueCount)
      .def("LogDriverStatistics", &Manager::LogDriverStatistics)
      .def("GetControllerInterfaceType", &Manager::GetControllerInterfaceType)
      .def("GetControllerPath", &Manager::GetControllerPath)
      .def("GetPollInterval", &Manager::GetPollInterval)
      .def("SetPollInterval", &Manager::SetPollInterval)
      .def("EnablePoll", &Manager::EnablePoll)
      .def("DisablePoll", &Manager::DisablePoll)
      .def("isPolled", &Manager::isPolled)
      .def("SetPollIntensity", &Manager::SetPollIntensity)
      .def("RefreshNodeInfo", &Manager::RefreshNodeInfo)
      .def("RequestNodeState", &Manager::RequestNodeState)
      .def("RequestNodeDynamic", &Manager::RequestNodeDynamic)
      .def("IsNodeListeningDevice", &Manager::IsNodeListeningDevice)
      .def("IsNodeFrequentListeningDevice", &Manager::IsNodeFrequentListeningDevice)
      .def("IsNodeBeamingDevice", &Manager::IsNodeBeamingDevice)
      .def("IsNodeRoutingDevice", &Manager::IsNodeRoutingDevice)
      .def("IsNodeSecurityDevice", &Manager::IsNodeSecurityDevice)
      .def("GetNodeMaxBaudRate", &Manager::GetNodeMaxBaudRate)
      .def("GetNodeVersion", &Manager::GetNodeVersion)
      .def("GetNodeSecurity", &Manager::GetNodeSecurity)
      .def("GetNodeBasic", &Manager::GetNodeBasic)
      .def("GetNodeGeneric", &Manager::GetNodeGeneric)
      .def("GetNodeSpecific", &Manager::GetNodeSpecific)
      .def("GetNodeType", &Manager::GetNodeType)
      .def("GetNodeNeighbors", &Manager::GetNodeNeighbors)
      .def("GetNodeManufacturerName", &Manager::GetNodeManufacturerName)
      .def("GetNodeProductName", &Manager::GetNodeProductName)
      .def("GetNodeName", &Manager::GetNodeName)
      .def("GetNodeLocation", &Manager::GetNodeLocation)
      .def("GetNodeManufacturerId", &Manager::GetNodeManufacturerId)
      .def("GetNodeProductType", &Manager::GetNodeProductType)
      .def("GetNodeProductId", &Manager::GetNodeProductId)
      .def("SetNodeManufacturerName", &Manager::SetNodeManufacturerName)
      .def("SetNodeProductName", &Manager::SetNodeProductName)
      .def("SetNodeName", &Manager::SetNodeName)
      .def("SetNodeLocation", &Manager::SetNodeLocation)
      .def("SetNodeOn", &Manager::SetNodeOn)
      .def("SetNodeOff", &Manager::SetNodeOff)
      .def("SetNodeLevel", &Manager::SetNodeLevel)
      .def("IsNodeInfoReceived", &Manager::IsNodeInfoReceived)
      .def("GetNodeClassInformation", &Manager:: GetNodeClassInformation)
      .def("IsNodeAwake", &Manager::IsNodeAwake)
      .def("IsNodeFailed", &Manager::IsNodeFailed)
      .def("GetNodeQueryStage", &Manager::GetNodeQueryStage)
      .def("GetValueLabel", &Manager::GetValueLabel)
      .def("SetValueLabel", &Manager::SetValueLabel)
      .def("GetValueUnits", &Manager::GetValueUnits)
      .def("SetValueUnits", &Manager::SetValueUnits)
      .def("GetValueHelp", &Manager::GetValueHelp)
      .def("SetValueHelp", &Manager::SetValueHelp)
      .def("GetValueMin", &Manager::GetValueMin)
      .def("GetValueMax", &Manager::GetValueMax)
      .def("IsValueReadOnly", &Manager::IsValueReadOnly)
      .def("IsValueWriteOnly", &Manager::IsValueWriteOnly)
      .def("IsValueSet", &Manager::IsValueSet)
      .def("IsValuePolled", &Manager::IsValuePolled)
      .def("GetValueAsBool", &Manager::GetValueAsBool)
      .def("GetValueAsByte", &Manager::GetValueAsByte)
      .def("GetValueAsFloat", &Manager::GetValueAsFloat)
      .def("GetValueAsInt", &Manager::GetValueAsInt)
      .def("GetValueAsShort", &Manager::GetValueAsShort)
      .def("GetValueAsString", &Manager::GetValueAsString)
      .def("GetValueAsRaw", &Manager::GetValueAsRaw)
      .def("GetValueListSelection", (bool(Manager::*)(const ValueID &, string *))&Manager::GetValueListSelection)
      .def("GetValueListSelection", (bool(Manager::*)(const ValueID &, int32 *))&Manager::GetValueListSelection)
      .def("GetValueListItems", &Manager::GetValueListItems)
      .def("GetValueFloatPrecision", &Manager::GetValueFloatPrecision)
      .def("SetValue", (bool(Manager::*)(const ValueID &, const bool))&Manager::SetValue)
      .def("SetValue", (bool(Manager::*)(const ValueID &, const uint8))&Manager::SetValue)
      .def("SetValue", (bool(Manager::*)(const ValueID &, const float))&Manager::SetValue)
      .def("SetValue", (bool(Manager::*)(const ValueID &, const int32))&Manager::SetValue)
      .def("SetValue", (bool(Manager::*)(const ValueID &, const int16))&Manager::SetValue)
      .def("SetValue", (bool(Manager::*)(const ValueID &, const uint8 *, const uint8))&Manager::SetValue)
      .def("SetValue", (bool(Manager::*)(const ValueID &, const string &))&Manager::SetValue)
      .def("SetValueListSelection", &Manager::SetValueListSelection)
      .def("RefreshValue", &Manager::RefreshValue)
      .def("SetChangeVerified", &Manager::SetChangeVerified)
      .def("PressButton", &Manager::PressButton)
      .def("ReleaseButton", &Manager::ReleaseButton)
      .def("GetNumSwitchPoints", &Manager::GetNumSwitchPoints)
      .def("SetSwitchPoint", &Manager::SetSwitchPoint)
      .def("RemoveSwitchPoint", &Manager::RemoveSwitchPoint)
      .def("ClearSwitchPoints", &Manager::ClearSwitchPoints)
      .def("GetSwitchPoint", &Manager::GetSwitchPoint)
      .def("SwitchAllOn", &Manager::SwitchAllOn)
      .def("SwitchAllOff", &Manager::SwitchAllOff)
      .def("SetConfigParam", &Manager::SetConfigParam)
      .def("RequestConfigParam", &Manager::RequestConfigParam)
      .def("RequestAllConfigParams", &Manager::RequestAllConfigParams)
      .def("GetNumGroups", &Manager::GetNumGroups)
      /* Wrapper Function */
      .def("GetAssociations", &luaGetAssociations)
      .def("GetMaxAssociations", &Manager::GetMaxAssociations)
      .def("GetGroupLabel", &Manager::GetGroupLabel)
      .def("AddAssociation", &Manager::AddAssociation)
      .def("RemoveAssociation", &Manager::RemoveAssociation)
      .def("ResetController", &Manager::ResetController)
      .def("SoftReset", &Manager::SoftReset)
      .def("TestNetworkNode", &Manager::TestNetworkNode)
      .def("TestNetwork", &Manager::TestNetwork)
      .def("HealNetworkNode", &Manager::HealNetworkNode)
      .def("HealNetwork", &Manager::HealNetwork)
      .def("GetNumScenes", &Manager::GetNumScenes)
      .def("GetAllScenes", &Manager::GetAllScenes)
      .def("RemoveAllScenes", &Manager::RemoveAllScenes)
      .def("CreateScene", &Manager::CreateScene)
      .def("RemoveScene", &Manager::RemoveScene)
      .def("AddSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const bool))&Manager::AddSceneValue)
      .def("AddSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const uint8))&Manager::AddSceneValue)
      .def("AddSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const float))&Manager::AddSceneValue)
      .def("AddSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const int32))&Manager::AddSceneValue)
      .def("AddSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const int16))&Manager::AddSceneValue)
      .def("AddSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const string&))&Manager::AddSceneValue)
      .def("AddSceneValueListSelection", (bool(Manager::*)(const uint8, const ValueID&, const string &))&Manager::AddSceneValueListSelection)
      .def("AddSceneValueListSelection", (bool(Manager::*)(const uint8, const ValueID&, const int32))&Manager::AddSceneValueListSelection)
      .def("RemoveSceneValue", &Manager::RemoveSceneValue)
      .def("SceneGetValues", &Manager::SceneGetValues)
      .def("SceneGetValueAsBool", &Manager::SceneGetValueAsBool)
      .def("SceneGetValueAsByte", &Manager::SceneGetValueAsByte)
      .def("SceneGetValueAsFloat", &Manager::SceneGetValueAsFloat)
      .def("SceneGetValueAsInt", &Manager::SceneGetValueAsInt)
      .def("SceneGetValueAsShort", &Manager::SceneGetValueAsShort)
      .def("SceneGetValueAsString", &Manager::SceneGetValueAsString)
      .def("SceneGetValueListSelection", (bool(Manager::*)(const uint8, const ValueID&, string*))&Manager::SceneGetValueListSelection)
      .def("SceneGetValueListSelection", (bool(Manager::*)(const uint8, const ValueID&, int32*))&Manager::SceneGetValueListSelection)
      .def("SetSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const bool))&Manager::SetSceneValue)
      .def("SetSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const uint8))&Manager::SetSceneValue)
      .def("SetSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const float))&Manager::SetSceneValue)
      .def("SetSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const int32))&Manager::SetSceneValue)
      .def("SetSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const int16))&Manager::SetSceneValue)
      .def("SetSceneValue", (bool(Manager::*)(const uint8, const ValueID&, const string&))&Manager::SetSceneValue)
      .def("SetSceneValueListSelection", (bool(Manager::*)(const uint8, const ValueID&, const string&))&Manager::SetSceneValueListSelection)
      .def("SetSceneValueListSelection", (bool(Manager::*)(const uint8, const ValueID&, const int32))&Manager::SetSceneValueListSelection)
      .def("GetSceneLabel", &Manager::GetSceneLabel)
      .def("SetSceneLabel", &Manager::SetSceneLabel)
      .def("SceneExists", &Manager::SceneExists)
      .def("ActivateScene", &Manager::ActivateScene)
      .def("GetDriverStatistics", &Manager::GetDriverStatistics)
      .def("GetNodeStatistics", &Manager::GetNodeStatistics)
    ];
  module(this->L, "OpenZWave")
    [
     class_<Notification, stupid_ptr<Notification> >("Notification")
      .def("GetType", &Notification::GetType)
      .def("GetHomeId", &Notification::GetHomeId)
      .def("GetNodeId", &Notification::GetNodeId)
      .def("GetValueID", &Notification::GetValueID)
      .def("GetGroupIdx", &Notification::GetGroupIdx)
      .def("GetEvent", &Notification::GetEvent)
      .def("GetButtonId", &Notification::GetButtonId)
      .def("GetSceneId", &Notification::GetSceneId)
      .def("GetNotification", &Notification::GetNotification)
      .def("GetByte", &Notification::GetByte)
      .enum_("NotificationType")
      [
        value("Type_ValueAdded", Notification::Type_ValueAdded),
        value("Type_ValueRemoved", Notification::Type_ValueRemoved),
        value("Type_ValueChanged", Notification::Type_ValueChanged),
        value("Type_ValueRefreshed", Notification::Type_ValueRefreshed),
        value("Type_Group", Notification::Type_Group),
        value("Type_NodeNew", Notification::Type_NodeNew),
        value("Type_NodeAdded", Notification::Type_NodeAdded),
        value("Type_NodeRemoved", Notification::Type_NodeRemoved),
        value("Type_NodeProtocolInfo", Notification::Type_NodeProtocolInfo),
        value("Type_NodeNaming", Notification::Type_NodeNaming),
        value("Type_NodeEvent", Notification::Type_NodeEvent),
        value("Type_PollingDisabled", Notification::Type_PollingDisabled),
        value("Type_PollingEnabled", Notification::Type_PollingEnabled),
        value("Type_SceneEvent", Notification::Type_SceneEvent),
        value("Type_CreateButton", Notification::Type_CreateButton),
        value("Type_DeleteButton", Notification::Type_DeleteButton),
        value("Type_ButtonOn", Notification::Type_ButtonOn),
        value("Type_ButtonOff", Notification::Type_ButtonOff),
        value("Type_DriverReady", Notification::Type_DriverReady),
        value("Type_DriverFailed", Notification::Type_DriverFailed),
        value("Type_DriverReset", Notification::Type_DriverReset),
        value("Type_EssentialNodeQueriesComplete", Notification::Type_EssentialNodeQueriesComplete),
        value("Type_NodeQueriesComplete", Notification::Type_NodeQueriesComplete),
        value("Type_AwakeNodesQueried", Notification::Type_AwakeNodesQueried),
        value("Type_AllNodesQueriedSomeDead", Notification::Type_AllNodesQueriedSomeDead),
        value("Type_AllNodesQueried", Notification::Type_AllNodesQueried),
        value("Type_Notification", Notification::Type_Notification)
      ]
      .enum_("NotificationCode")
      [
        value("Code_MsgComplete", Notification::Code_MsgComplete),
        value("Code_Timeout", Notification::Code_Timeout),
        value("Code_NoOperation", Notification::Code_NoOperation),
        value("Code_Awake", Notification::Code_Awake),
        value("Code_Sleep", Notification::Code_Sleep),
        value("Code_Dead", Notification::Code_Dead),
        value("Code_Alive", Notification::Code_Alive)
      ]
    ];
  module(this->L, "OpenZWave")
    [
    class_<ValueID>("ValueID")
    	.def("GetHomeId", &ValueID::GetHomeId)
    	.def("GetNodeId", &ValueID::GetNodeId)
    	.def("GetGenre", &ValueID::GetGenre)
    	.def("GetCommandClassId", &ValueID::GetCommandClassId)
    	.def("GetInstance", &ValueID::GetInstance)
    	.def("GetIndex", &ValueID::GetIndex)
    	.def("GetType", &ValueID::GetType)
    	.def("GetId", &ValueID::GetId)

    	.enum_("ValueGenre")
    	[
    	 	 value("ValueGenre_Basic", ValueID::ValueGenre_Basic),
    	 	 value("ValueGenre_User", ValueID::ValueGenre_User),
    	 	 value("ValueGenre_Config", ValueID::ValueGenre_Config),
    	 	 value("ValueGenre_System", ValueID::ValueGenre_System),
    	 	 value("ValueGenre_Count", ValueID::ValueGenre_Count)
    	]
    	.enum_("ValueType")
    	[
    		 value("ValueType_Bool", ValueID::ValueType_Bool),
    		 value("ValueType_Byte", ValueID::ValueType_Byte),
    		 value("ValueType_Decimal", ValueID::ValueType_Decimal),
    		 value("ValueType_Int", ValueID::ValueType_Int),
    		 value("ValueType_List", ValueID::ValueType_List),
    		 value("ValueType_Schedule", ValueID::ValueType_Schedule),
    		 value("ValueType_Short", ValueID::ValueType_Short),
    		 value("ValueType_String", ValueID::ValueType_String),
    		 value("ValueType_Button", ValueID::ValueType_Button),
    		 value("ValueType_Raw", ValueID::ValueType_Raw),
    		 value("ValueType_Max", ValueID::ValueType_Max)
    	]
    ];

}

OZW4Lua::OZW4Lua(logCallBack cb) {
    this->logcb = cb;
    OZW4Lua();
}


OZW4Lua::~OZW4Lua() {
}

bool OZW4Lua::InitManager(OpenZWave::Manager *OZWManager, std::string filename) {

	object globalvars = globals(this->L);
	globalvars["OZWManager"] = OZWManager->Get();
	if (luaL_dofile(this->L, filename.c_str())) {
		this->doLog(log_warning, str(boost::format("Lua OZWQuirks.lua Error: %s")% lua_tostring(L, -1)));
		return false;
	}
	return true;
}

void OZW4Lua::setLogCB(logCallBack cb) {
    this->logcb = cb;
}

void OZW4Lua::doLog(logLevel ll, std::string message) {
    if (this->logcb)
        this->logcb(ll, message);
    else
        switch (ll) {
            case log_critical:
                Log::Write(LogLevel_Error, "%s", message.c_str());
                break;
            case log_warning:
                Log::Write(LogLevel_Warning, "%s", message.c_str());
                break;
            case log_info:
                Log::Write(LogLevel_Info, "%s", message.c_str());
                break;
            case log_debug:
                Log::Write(LogLevel_Debug, "%s", message.c_str());
                break;
        }
}

bool OZW4Lua::DoNotification(OpenZWave::Notification const *notify) {
	try {
		int ret = call_function<int>(this->L, "OZWNotification", notify);
		return ret ? true : false;
	} catch (...) {
		const char* estr = lua_tostring(this->L, -1);
		lua_pop(this->L, 1);
		this->doLog(log_warning, str(boost::format("Lua OZWNotification Error: %s")% estr));
	}
	return true;
}

luabind::object luaGetAssociations(Manager *, uint32 _homeId, uint8 _nodeId, uint8 _groupIdx) {
	uint8 *assoications;
	uint32 number;
	luabind::object result;
	number = Manager::Get()->GetAssociations(_homeId, _nodeId, _groupIdx, &assoications);
	if (number > 0) {
		result = luabind::newtable(OZW4Lua::getLuaState());
		int i;
		for (i = 0; i < number; i++) {
			result[i+1] = assoications[i];
		}
	}
	return result;
}
std::string OZW4Lua::getVersion() {
    return std::string(VERSION);
}