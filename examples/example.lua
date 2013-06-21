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


-- Change the standard error function so that it shows the debug traceback.
local error = error
_G.error = function(m, l)
    error("\nerror msg: "..m.."\n"..debug.traceback(), (l or 1) + 1)
end
print("Initializing")
-- This contains variables that we can store for specific nodes. 
NodeData = { nodes = { } }
-- This contains binding functions that are checked when new nodes are added. 
-- Binding functions recieve nofifications for that node, and can do something
NodeBinding = {}
-- This table contains functions that are used to determine if a Node should 
-- be Bound NodeBinding. 
NodeFilters = {}


local Counters = {}
local Names = {}

local function hook ()
	local f = debug.getinfo(2, "f").func
        if Counters[f] == nil then    -- first time `f' is called?
        	Counters[f] = 1
                Names[f] = debug.getinfo(2, "Sn")
        else  -- only increment the counter
                Counters[f] = Counters[f] + 1
        end
end
--debug.sethook(hook, "c")  -- turn on the hook

function getname (func)
        local n = Names[func]
        if n.what == "C" then
	        return n.name
        end
        local loc = string.format("[%s]:%s",
        	n.short_src, n.linedefined)
        if n.namewhat ~= "" then
        	return string.format("%s (%s)", loc, n.name)
        else
        	return string.format("%s", loc)
        end
end

function OZWNotification(msg)
	for l,d in pairs(NodeBinding) do
		d(msg)
	end
--    	for func, count in pairs(Counters) do
--          	print(getname(func), count)
--        end
              
	return 1
end

function VitrumReportBug(msg)
	if (msg:GetType() == OpenZWave.Notification.Type_NodeEvent) then
		local valID = msg:GetValueID()
		if (NodeData.nodes[valID:GetNodeId()].RefreshPending == false) then 
			if (valID:GetGenre() == OpenZWave.ValueID.ValueGenre_Basic) then
				NodeData.nodes[valID:GetNodeId()].RefreshPending = true
				print("Refreshing Dynamic Values:", valID:GetHomeId(), valID:GetNodeId())
				OZWManager:RequestNodeDynamic(valID:GetHomeId(), valID:GetNodeId())
				-- Don't Continue Processing
				return 0
			end
		else
			print("Refresh is Pending on ", valID:GetNodeId())
			-- Refresh is Pending. Don't continue
			return 0
		end
	elseif (msg:GetType() == OpenZWave.Notification.Type_ValueChanged) then
		local valID = msg:GetValueID()
		if (NodeData.nodes[valID:GetNodeId()].RefreshPending == true) then
			-- As Long as the instance is bigger than one, we can canx this flag
			print("Instance: ", valID:GetInstance())
			if (valID:GetInstance() > 1) then
				NodeData.nodes[valID:GetNodeId()].RefreshPending = false
			end
		end
		-- what ever happens, continue
		return 1
	elseif (msg:GetType() == OpenZWave.Notification.Type_NodeAdded) then
		table.insert(NodeData["nodes"], msg:GetNodeId(), {RefreshPending = false, GroupsUpdated = false})
	elseif (msg:GetType() == OpenZWave.Notification.Type_Group) then 
		local associations
		if (NodeData.nodes[msg:GetNodeId()].GroupsUpdated == false) then 
			print("Associations Group ", msg:GetGroupIdx(), "For Node ", msg:GetNodeId())
			associations = OZWManager.GetAssociations(OZWManager, msg:GetHomeId(), msg:GetNodeId(), msg:GetGroupIdx())
			local addNode1 = true;
			if (associations) then
				for i,v in ipairs(associations) do 
					if (v == 1) then addNode1 = false end
					print("Node: ",v) 
				end
			end
			if (addNode1 == true) then
				print("Adding Node 1 to Group ", msg:GetGroupIdx())
				OZWManager:AddAssociation(msg:GetHomeId(), msg:GetNodeId(), msg:GetGroupIdx(), 1);
			end
		if(OZWManager:GetNumGroups(msg:GetHomeId(), msg:GetNodeId()) == msg:GetGroupIdx()) then
			print("All Associations on Node ", msg:GetNodeId(), " Updated")
			NodeData.nodes[msg:GetNodeId()].GroupsUpdated = true
		end
		end
	end
end

table.insert(NodeBinding, VitrumReportBug)
