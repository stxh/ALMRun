--everything�����ѯ�ļ�����
RegisterPlugin('command',{name="EV",func=function(commandName,...)
    local t_command = {}
    if commandName:len() > 0 then
        local files = Everything(commandName);
        for k,v in pairs(files) do
            local pre = "*"
            if v["TYPE"]=="FILE" then
                pre = ""
            end
            table.insert(t_command,{name=pre..v["NAME"],desc=v["PATH"],order=1090,cmd = v["FULL_PATH"]})
        end
    else
        table.insert(t_command,{name="��Everything�����ļ�",desc="��Everything�����ļ�",order=1099});
    end
    return t_command
end})
