local bit = require("bit")
local eventTable = {}
local PluginTable = {}

function RegisterPlugin(Name,plugin)
    if plugin.func == nil then
	return
    end
    if not PluginTable[Name] then
	PluginTable[Name] = {}
    end
    local _plugins = PluginTable[Name]
    table.insert(_plugins, plugin)
    if not _G["plugin_"..Name] then
	_G["plugin_"..Name] = function(commandName,...)
	    local _eventResult = {}
	    for _,_plugin in ipairs(_plugins) do
		local __name
		if _plugin.name == nil or #_plugin.name==0 then
		    __name = commandName
		else
		    local l_len = _plugin.name:len()+1
		    __name = commandName:sub(1,l_len)
            __name = string.gsub(__name," ","")
		    if _plugin.name:upper():find(__name:upper()) then
			__name,l_len = commandName:gsub("%S+%s+(.+)","%1")
			if l_len == 0 then
			    __name = ""
			end
		    else
			__name = nil
		    end
		end
		if __name then
		    local _func = _plugin.func
		    local result = _func(__name,...)
		    if result then
			if #result == 0 then--���ص������
			    if _plugin.name ~= nil then
				result.name = _plugin.name .. "\1" .. result.name
			    end
			    table.insert(_eventResult,result)
			else
			    for _,__ in ipairs(result) do
			    	if _plugin.name ~= nil then
				    __.name = _plugin.name .. "\1" .. __.name
				end
				table.insert(_eventResult,__)
			    end
			end
		    end
		end
	    end
	    return _eventResult
	end
    end
end

function addEventHandler(eventName, eventHandler)
	if not eventTable[eventName] then
		eventTable[eventName] = {}
	end
	local events = eventTable[eventName] 
	table.insert(events, eventHandler)
	if not _G[eventName] then
		_G[eventName] = function(...)
			for _, _eventHandler in ipairs(events) do
				_eventHandler(...)
			end
		end
	end
end

function enterString(str)
	local t = {}
	str:gsub('.', function(c)
		table.insert(t, c)
	end)
	enterKey(table.unpack(t))
end

function trim(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end

function get_name(file)
	return file:match("[^\\]+$")
end

-- �Զ�ɨ��ָ��Ŀ¼��ָ����չ�������б���
function scan_dir(path,ext,sub)
    if ext == nil then
	ext = "*.exe"
    end
    if sub == nil then
	sub = -1
    end
    path = path:gsub("%%(%S+)%%",os.getenv)
    local Files = ListDir(path,ext,sub)
    if Files == nil then 
	return
    end
    for key,value in pairs(Files) do  
	m_name = get_name(value)
	addCommand{ name = m_name,cmd = value}
    end
end

-- Events
addEventHandler('onClose', function()
    if curHideWindow then
	showWindow(curHideWindow, 'normal')
    end
end)


function SplitCmd(cmdLine)
   cmdLine = trim(cmdLine)
   if cmdLine:sub(1,12):lower():find("tps?://") then -- ��ַhttp://��ftp://��https://
	return cmdLine
    end
   local pos = cmdLine:find(":::") -- ����ʹ��:::�ָ�����
   local cmd,arg
    if pos then
	cmd = cmdLine:sub(1,pos-1)
	arg = cmdLine:sub(pos+3)
	return cmd,arg
    end
    --�����ŵ��ļ���
    cmd = cmdLine:match("^\"(.-)\"")
    if cmd then
	arg = cmdLine:match("^\".-\"%s*(.*)")
	return cmd,arg
    end
    --����������г���" -"��" /",Ҳ��Ϊ�ǲ�������,Ҫ�ָ�����.
    cmd = cmdLine:match("^(.-)%s+[%-|/]%a")
    if cmd then
	arg = cmdLine:match("%s+[%-|/]%a.*")
	return cmd,arg
    end
    cmd = cmdLine:match(".-%.[^%s]+")
    if cmd then
	arg = cmdLine:sub(cmd:len()+1)
	return cmd,arg
    end
    return cmdLine
end

-- Ĭ�ϵ�������ú���(Ĭ�ϲ�����)
__CmdCallFunc = function(cmdLine,cmdArg,workDir)
    -- �Զ������滻{%p}
    if cmdLine:match("{%%[pP]%+?}") then
	cmdLine = cmdLine:gsub("{%%[pP]%+?}",cmdArg)
	cmdArg = nil
    end
    -- �滻{%c}Ϊ����������
    if cmdLine:match("{%%c}") then
	cmdLine = cmdLine:gsub("{%%c}",GetClipboardData())
    end
    -- �滻{%wt}Ϊ�������
    if cmdLine:match("{%%wt}") then
	cmdLine = cmdLine:gsub("{%%wt}",GetWindowText(getForegroundWindow()))
    end
    -- ����{%p},{%c},{%wt}��Ϊ�˼���ALTRUN��������
    cmdLine = cmdLine:gsub("%%(%S+)%%",os.getenv) --ϵͳ����������չ
    local cmd,arg = SplitCmd(cmdLine)
    if arg then
	if cmdArg then
	    cmdArg = arg..' '..cmdArg
	else
	    cmdArg = arg
	end
    end
-- ��ȡ��ִ�г�������Ŀ¼
    local DestDir = workDir
    if DestDir == nil or #DestDir==0 then
	DestDir = cmd:match("^(.-)[^\\|/]+$")
    end
    return shellExecute(cmd,cmdArg,DestDir)
end

function read_altrun_config (file)
  local fp = assert(io.open (file))
  local csv = {}
  for line in fp:lines() do
    local row = {}
    for value in line:gmatch("[^|]+") do
      row[#row+1] = trim(value)
    end
    if not (row[1] == nil or row[1] == "") then
	row[5] = line:gsub("[^|]+|","",4)
	csv[#csv+1] = row
    end
  end
	fp:close()
  return csv
end

-- ֱ�ӵ���ALTRUN������,^_^
function altrun_config(file)
    local FileName = file:match("[^\\]+$")
    local DestDir = file:sub(1,-FileName:len()-1)
    if not FileExists(file) then--�ļ�������
	return
    end
    local altrun = read_altrun_config(file)
    for i=1,#altrun do
	if altrun[i][5]:sub(1,1) == "." then
	    altrun[i][5] = DestDir .. altrun[i][5]
	end
	if altrun[i][5]:sub(1,2) == "@." then
	    altrun[i][5] = "@" .. DestDir .. altrun[i][5]:sub(2)
	end
	addCommand{ name = altrun[i][3],desc=altrun[i][4],cmd = altrun[i][5],flags = bit.bor(CMDS_FLAG_ALTRUN,bit.lshift(i,1))}
    end
end
