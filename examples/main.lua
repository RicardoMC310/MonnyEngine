function onSetup()

end

function onUpdate(deltaTime)
    if input.is_down("move_left") then
        print("Andando para esquerda")
    elseif input.is_down("move_right") then
        print("Andando para direita")
    end

    if input.is_down("move_up") then
        print("Andando para cima")
    elseif input.is_down("move_down") then
        print("Andando para direta")
    end

    if input.is_pressed("attack") then
        print("Atacando")
    end

    if input.is_released("jump") then 
        print("Pulando")
    end

    if input.is_pressed("special") then
        print("Ataque especial")
    end
end

function onStop()

end