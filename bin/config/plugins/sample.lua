--[[
��������﷨��ʽ:

RegisterPlugin('command',{name=����,func=function(����,...)
--Ŀǰֻ֧��һ��������������Ĳ�������
--�������
--�������
--Ϊ����ALMRun����ʾһ����ͨ��������ӵ�������Ҫ����һ�� �����������
--����
--1. ���ص�������:
-- return {name=����1,desc="ALMRun ���в��",cmd = commandName,order=-1}
--2. ���ض������:
-- local t_command = {}
-- table.insert(t_command,{name=����1,desc=ע��,cmd=������,order=1099});
-- table.insert(t_command,{name=����2,desc=ע��,cmd=������2,order=1099});
-- return t_command
end})

ע: ����������Բο�ALMRun��API����addCommand(doc\config_api.md�ļ�)
--]]

 RegisterPlugin('command',{name='LUA',func = function(commandName,...)
     local t_command = {name="ִ��LUA����:"..commandName,desc="ALMRun ���ʾ��",order=1000}
     if commandName:len() > 1 then
	 t_command.cmd = function(arg)
	     MessageBox(loadstring("return "..commandName)(arg))
	 end
     end
     return t_command
 end})

--orderֵΪ����,ʹ�������������ȼ��Ƚϵ�,Ҳ���Ǳ�������������б�β��
RegisterPlugin('command',{func=function(commandName,...)
    return {name="Run "..commandName,desc="ALMRun ���в��",cmd = commandName,order=-1}
end})

--���������ʾ�������ض�����ͨ��orderֵ�������������ֵ�ߵ���ǰ��
RegisterPlugin('command',{name="CAL",func=function(commandName,...)
    local t_command = {}
    table.insert(t_command,{name=string.format("����: %s",commandName),desc="���������",order=1099});
    local _,result = pcall(loadstring("return "..commandName))
    local nresult = tonumber(result)
    --result = tonumber(result)
    if result ~= nil then
        if math.ceil(result) == result then
            table.insert(t_command,{name=string.format("    = %d(0x%X)",result,result),desc="������",order=1090})
        else
            local strresult = string.format("%f",result)
            local rightstr = string.sub(strresult,string.len(strresult),string.len(strresult)+1)
            while rightstr =="0" do
                strresult = string.sub(strresult,0,string.len(strresult)-1)
                rightstr = string.sub(strresult,string.len(strresult),string.len(strresult)+1)
            end
            table.insert(t_command,{name=string.format("    = %s",strresult),desc="������",order=1090})
        end
    end
    return t_command
end})
