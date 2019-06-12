for i = 0, 20 do
	print(".global isr" .. i .. "\
.type isr" .. i .. ", @function\
isr" .. i .. ":\
pushq $0 /* error code */\
pushq $" .. i .. " /* vector */\
jmp interrupt_generic\n
\n")
end