function(print_header title)
	message(STATUS "")
	message(STATUS "=====================================================================")
	message(STATUS "= ${title}")
	message(STATUS "=====================================================================")
endfunction(print_header)

function(dump_vars)
	get_cmake_property(_var_names VARIABLES)
	list(SORT _var_names)
	foreach(_var ${_var_names})
		message("${_var} = ${${_var}}")
	endforeach()
endfunction(dump_vars)
