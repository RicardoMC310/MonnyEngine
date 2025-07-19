function onSetup()
    createWindow("Amo minha namorada", 640, 320)
end

function onUpdate(dt)

end

function onDestroy()
end

function onKeyUp(key)
    if key == "Escape" then
        exit(0)
    end
end